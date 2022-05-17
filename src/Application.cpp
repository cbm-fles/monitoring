// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020-2022 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#include "Application.hpp"

#include "ChronoHelper.hpp"
#include "Exception.hpp"
#include "PThreadHelper.hpp"
#include "SysCallException.hpp"

#include "fmt/format.h"

#include <algorithm>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <errno.h>
#include <execinfo.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

namespace cbm {
using namespace std;

/*! \class Application
  \brief Execution application of a \glos{CBMmain} program.

  This class holds the complete execution application of a \glos{CBMmain}:
  - controls the initialization and instantiates
    - the Logger facility
    - the Monitor facility
  - and last but not least provides a signal handler for _well documented
    crash_ via `SIGSEGV` and `SIGBUS` (see SignalCatcher())

  \note On \ref objectownership
    - is owned by \glos{CBMmain}
    - owns Logger
    - owns Monitor
 */

//-----------------------------------------------------------------------------
// local helper functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//! \brief Prints last chance error messages to `cout`

static void SysCallErr(const char* what) {
  cerr << "Cbm Application::SignalCatcher: "s << what << " FAILED: "
       << strerror(errno) << endl;
}

//-----------------------------------------------------------------------------
/*! \brief Destructor

  This is the central place for the controlled shutdown of a \glos{CBMmain}. It
  - this stops all concurrent activities like queued and scheduled actions
  - destructs the Monitor
  - and finally destructs the Logger
 */

Application::~Application() {
  // control destruction sequence
  if (fpLogger) CBMLOGNOT1("cid=__Application", "CBM-end") // the last log message
                  << "CBM finished";
  cout << "Cbm finished" << endl;           // stdout message (for systemd)

  timespec dt{0,200000000};                 // allow logger + monitor to process
  ::nanosleep(&dt, nullptr);                //   pending messages
}

//-----------------------------------------------------------------------------
/*! \brief Constructor

  \param argc  argument count passed from `main` program
  \param argv  argument vector passed from `main` program

  Only a few \glos{CBMmain} operation parameters are set via Init().
  These operation parameters are determined (in that order)
  - by a default value
  - by a command line option of the form: `--<option> <value>`
  - by an environment variable with a name: `CBM_<OPTION>`

  The Init sequence is
  - set process-wide signal block mask (see note below)
  - startup Logger (which starts "Cbm:logger" thread)
  - setup signal catcher for `SIGSEGV` and `SIGBUS` (see SignalCatcher())
  - process startup options
  - startup Monitor (which starts "Cbm:monitor" thread)

  If any of these steps fails, Init() will write a message with severity
  `Fatal` to Logger and return with a return code of 1. The only thing
  the \glos{CBMmain} program can do in that case is to return with an exit
  code 1.

  \note
    Signals to be handled later via `signalfd` in Master must be blocked in
    all threads of the program. When a thread is created it will inherit
    the signal block mask. Therefore the blocking of signals handled later
    via `signalfd` must be done _before_ any threads are started, e.g. in
    Logger and ZMQ application.
    Therefore the de-facto process wide signal block mask is set here
    as the very first thing of Init(), before the Logger is started,
 */

Application::Application(int argc, char* argv[]) :
  fpLogger(),
  fpMonitor(),
  fOptMapOpen(),
  fOptMapDone()
{
  // setup signal block mask -------------------------------
  //  All threads will inherit this configuration !
  //  This MUST be done BEFORE any other threads are started !!

  sigset_t sigmask;
  sigemptyset(&sigmask);                    // the sigmask here and in
  sigaddset(&sigmask, SIGINT);              // Master::Init must agree
  sigaddset(&sigmask, SIGTERM);
  sigaddset(&sigmask, SIGHUP);

  if (auto irc = sigprocmask(SIG_BLOCK, &sigmask, nullptr); irc < 0) {
    cerr << "Cbm Application::Init: sigprocmask failed: " << ::strerror(errno)
         << endl;
    return;
  }

  // process command line options --------------------------
  string cmdline = string(argv[0]);
  for (int i=1; i<argc; i++) {
    cmdline += " "s + string(argv[i]);
    fOptMapOpen[string(argv[i])] = ""s;
    // check if next argument is a value (if it does not start with "--")
    if (i+1 < argc && argv[i+1][0] != '-' && argv[i+1][1] != '-') {
      fOptMapOpen[string(argv[i])] = string(argv[i+1]);
      cmdline += " "s + string(argv[i+1]);
      i++;
    }
  }

  // look for --help or -h, if found, print help message and quit
  if (TstOpt("--help"s) || TstOpt("-h"s)) {
    cerr << "usage: cbm [OPTION]...\n"
         << "  Options:\n"
         << "    --help                print help and exit\n"
         << "    --nosyslog            no syslog: Logger sink\n"
         << "    --logfile             open Logger sink to default filename\n"
         << "    --monitor SNAME       open Monitor sink to SNAME\n"
         << "  Default for all LogLevels is Info\n"
         << "  Valid LogLevels are: Trace, Debug, Info, Note, Warning, "
         << " Error, Fatal" << endl;
    return;
  }

  // startup Logger ----------------------------------------
  fpLogger = make_unique<Logger>();
  // set main thread name, for convenience (e.g. for top 'H' display)
  // done here so that Logger can pickup the program name with PThreadName()
  SetPThreadName("Cbm:main");

  if (TstOpt("--nosyslog"s)) {
    fpLogger->OpenSink("file:cout", Logger::kLogWarning);
  } else {
    fpLogger->OpenSink("syslog:", Logger::kLogNote);
  }

  string logsinkname = "";
  if (TstOpt("--logfile"s)) {
    // open sink file:cbm_YYYY-MM-DD_HH_MM_SS_<hostname>.log
    string timestamp = TimeStamp();
    timestamp.replace(10,1, "_"s);
    timestamp.replace(13,1, "_"s);
    timestamp.replace(16,1, "_"s);
    timestamp.replace(19,7, ""s);
    logsinkname = fmt::format("file:cbm_{}_{}.log", timestamp,
                              fpLogger->HostName());
    fpLogger->OpenSink(logsinkname, Logger::kLogTrace);
  }

  // setup signal catcher ----------------------------------
  ConnectSignalCatcher(SIGSEGV);
  ConnectSignalCatcher(SIGBUS);

  // startup Monitor ---------------------------------------
  fpMonitor = make_unique<Monitor>();
  string monipath = GetOptString("--monitor"s, ""s);
  if (monipath != "") {
    try {
      fpMonitor->OpenSink(monipath);
      fpLogger->OpenSink("monitor:"s, Logger::kLogNote);
    } catch (const exception& e) {
      CBMLOGFAT1("cid=__Application", "Init-badmoni")
        << "Cbm Application::Init: --monitor failed: " << e.what();
      cerr << "Cbm Application::Init: --monitor failed: " << e.what() << endl;
      return;
    }
  }

  // abort if any unexpected options found
  if (!fOptMapOpen.empty()) {
    string badargs;
    for (auto& [opt, val]: fOptMapOpen)
      badargs += fmt::format(" {} {}"s, opt, val);

    CBMLOGFAT1("cid=__Application", "Init-badargs")
      << "Cbm Application::Init: unknown options:" << badargs;
    cerr << "Cbm Application::Init: unknown options:" << badargs << endl;
    return;
  }
}

//-----------------------------------------------------------------------------
//! \brief Tests whether command line option `opt` given

bool Application::TstOpt(const string& opt) {
  if (fOptMapOpen.count(opt) > 0) {
    fOptMapDone[opt] = fOptMapOpen[opt];
    fOptMapOpen.erase(opt);
    return true;
  }
  if (fOptMapDone.count(opt) > 0) return true;
  return false;
}

//-----------------------------------------------------------------------------
//! \brief Returns value for command line option `opt` or default `def`

const string& Application::GetOptString(const string& opt, const string& def) {
  return TstOpt(opt) ? fOptMapDone[opt] : def;
}

//-----------------------------------------------------------------------------
/* \brief Returns value for command line option `opt` or default `def`
   \throws Exception if option value doesn't convert to an `int`
 */

int Application::GetOptInt(const string& opt, int def) {
  if (!TstOpt(opt)) return def;
  istringstream ss(fOptMapDone[opt]);
  int val;
  if (!(ss >> val))                         // convert
    throw Exception(fmt::format("Application::GetOptInt: conversion error in '{}'"s,
                                fOptMapDone[opt]));
  char c;
  if (ss >> c)                              // check for trailing stuff
    throw Exception(fmt::format("Application::GetOptInt: conversion error in '{}'"s,
                                fOptMapDone[opt]));
  return val;
}

//-----------------------------------------------------------------------------
//! \brief Connects SignalCatcher to signal `signum`.

void Application::ConnectSignalCatcher(int signum) {
  struct sigaction sigact = {};
  sigact.sa_sigaction = Application::SignalCatcher;
  sigact.sa_flags = SA_SIGINFO;
  if (auto irc = sigaction(signum, &sigact, nullptr); irc < 0)
    throw SysCallException("Application::ConnectSignalHandler"s, "sigaction"s,
                           errno);
}

//-----------------------------------------------------------------------------
/*! \brief Handler for program error signals like `SIGSEGV` and `SIGBUS`

  No graceful shutdown is possible after such a signal is recieved.
  The only action possible is to collect some status information and
  make it available for later diagnosis. This handler
  - creates an error message with signal and backtrace information
  - writes this message to a file named with a name of the form
    `cbm_crash_yyyy-mm-ddThh:mm:ss.ssssss_<hostname>.log`
    into the current working directory
  - writes this message to Logger with severity _Fatal_
  - and calls abort() which typically will create a `core` dump

  \note the `cbm` should be linked with `-rdynamic` to ensure that
    the backtrace has symbol information and not only addresses.
  \note the message has the backtrace in `mangled` form. Use `c++filt`
    to `demangle` the symbols and get them more readable.
 */

[[noreturn]] void Application::SignalCatcher(int signum, siginfo_t* siginf, void*) {
  // protect agains multiple calls
  ::signal(signum, SIG_DFL);

  // collect basic signal info
  ostringstream msg;
  msg << "got signal: si_signo=" << signum
      << ", si_code=" << siginf->si_code
      << ", name=" << ::strsignal(signum);
  // get fault address
  if (signum == SIGSEGV || signum == SIGBUS) {
    msg << "\nat si_addr=" << siginf->si_addr;
  }
  // get fault traceback
  msg << "\nin thread " << PThreadName() << " at";
  const size_t btbufsize = 256;
  void*  btbuf[btbufsize];
  int nbt = ::backtrace(btbuf, btbufsize);
  char** btsym = ::backtrace_symbols(btbuf, nbt);
  if (btsym) {
    for (int i=1; i<nbt; i++) {
      msg << "\n  #" << i << " " << btsym[i];
    }
  }
  ::free(btsym);
  msg << "\nCbm CRASHED - core dump requested\n";

  // Write cbm_crash_yyyy-mm-ddThh:mm:ss.ssssss_<hostname>.log file in local dir
  char hname[80] = {};
  if (auto rc = ::gethostname(hname, sizeof(hname)); rc < 0) hname[0] = 0;
  string fname = fmt::format("cbm_crash_{}_{}.log", TimeStamp(), hname);
  auto fd = ::open(fname.c_str(), O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP);
  if (fd > 0) {
    string str = msg.str();
    if (::write(fd, str.c_str(), str.length()) < 0) SysCallErr("write");
    if (::close(fd) < 0) SysCallErr("close");
  } else {
    SysCallErr("open");
  }

  // Send output to Logger if not on Logger thread (in that case send to cerr).
  // Theoretically this might dead-lock when the logger queue protection lock
  // is held. That can only happen if the problem is within the Logger code.
  // For `SIGSEGV` in \glos{DObject} code and `SIGBUS` in PCIe access code this
  // should always work, and that are the cases this reporting is made for.
  if (PThreadName() != "Cbm:logger") {
    CBMLOGFAT1("cid=__Application", "SignalCatcher") << msg.str();
    timespec dt{0, 200000000};
    if (::nanosleep(&dt, nullptr) < 0) SysCallErr("nanosleep");
  } else {
    cerr << "Cbm Application::SignalCatcher:" << msg.str() << endl;
  }

  // finally call abort, which creates a core dump
  cerr << "Cbm CRASHED - backtrace in " << fname << "\n"
       << "Cbm CRASHED - calling std::abort (will core dump)" << endl;
  ::abort();
}

} // end namespace cbm
