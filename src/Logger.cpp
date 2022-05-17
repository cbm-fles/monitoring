// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020-2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#include "Logger.hpp"

#include "Exception.hpp"
#include "LoggerSinkFile.hpp"
#include "LoggerSinkMonitor.hpp"
#include "LoggerSinkSyslog.hpp"
#include "PThreadHelper.hpp"
#include "SysCallException.hpp"

#include "fmt/format.h"

#include <errno.h>
#include <poll.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>
#include <unistd.h>

namespace cbm {

/*! \class Logger
  \brief Thread-safe logging system for DCA

  This class provides a thread-safe logging for the DCA framework.
  The Logger is instantiated as a \glos{singleton} in Context as very first
  step and destroyed as very last step, and therefore available in a
  \glos{DCAmain} throughout its whole lifetime.

  The Logger system has three layers
  - macros which provide a stream like interface to generate message
  - a core which collects and buffers messages
  - back-ends called `sinks` which write messages to files or other destinations

  The Logger generates log messages with three parts
  - a timestamp
  - a key list
  - a message body

  The key list is a list of `key=value` pairs and concatenated from three
  sources:
  - general keys (usage depends on sink)
    - `host`: the host name from `gethostname(2)`
    - `prog`: program name, from initial PThreadName()
    - `thread`: thread name as retrieved with PThreadHelper::PThreadName()
    - `sev`: severity
  - environment keys, usually from local `LogKeys()`. For \glos{DObject}s:
    - cid: the \glos{classid}
    - oid: the \glos{objectid}
  - and message specific keys, at least
    - mid: the \glos{messageid}

  This provides well structured log information which can be processed and
  indexed by other tools. This structure allows to determine
  - _what happened_: from \glos{classid} and \glos{messageid}
  - _where it happened_: from \glos{classid} and \glos{objectid}
  - _why it happened_: from message body

  The Logger defines a set of severity levels for message filtering
  - `kLogTrace` : used for very detailed tracing
  - `kLogDebug` : used to emit debug information
  - `kLogInfo` : used to emit execution information
  - `kLogNote` : used to emit important notifications
  - `kLogWarning` : used to signal expected exceptions
  - `kLogError` : used to signal unexpected fault conditions
  - `kLogFatal` : used to signal unrecoverable fault conditions

  The Logger processes messages with severity 'Note' and higher immediately.
  Message with lower severity are queued to minimize thread-switching and
  processed every 100 ms.

  The Logger provides a stream-like interface via macros with build-in
  severity filtering and environment key generation. Typical usage in
  a \glos{DObject}s is
  \code{.cpp}
  DCALOGWAR("<mid>","") << "text: a1=" << a1 << ",a2=" << a2;
  DCALOGWAR("<mid>","") << fmt::format("text: a1={},a2={}", a1, a2);
  \endcode

  This example creates and writes a `Warning` level message only if the
  `LogLevel()` of the \glos{DObject} is at `kLogWarning` or less.

  In most cases the Logger is used in \glos{DClass} code via these macros:
  - `DCALOGTRA(mid,keys)`: for `Warning`
  - `DCALOGDEB(mid,keys)`: for `Debug`
  - `DCALOGINF(mid,keys)`: for `Info`
  - `DCALOGNOT(mid,keys)`: for `Note`
  - `DCALOGWAR(mid,keys)`: for `Warning`
  - `DCALOGERR(mid,keys)`: for `Error`
  - `DCALOGFAT(mid,keys)`: for `Fatal`
  - `DCALOGNOTI(mid,keys)`: for `Note` without \glos{loglevel} check
  - `DCALOGGEN(sev,mid,keys)`: with severity as argument
  - `DCALOGGEN1(sev,mid,keys)`: with severity as argument without
                                \glos{loglevel} check

  A set of interface macros for usage outside of \glos{DClass}es is also provided
  - `DCALOGNOT1(keys1,mid)`: for `Note`
  - `DCALOGERR1(keys1,mid)`: for `Error`
  - `DCALOGFAT1(keys1,mid)`: for `Fatal`
  - `DCALOG(sel,sev,keys1,mid,keys2)`: allows full control of message

  For cases where one sequence of `operator<<()`s is not sufficient the
  low level interface provided by MakeStream() can be used directly.

  The Logger back-end is provided by LoggerSink objects and controlled via
  - OpenSink(): creates a new sink
  - CloseSink(): removes a sink
  - SinkLogLevel(): changes the \glos{loglevel} of a sink

  Currently three sink types are implemented
  - LoggerSinkFile: writes to files
  - LoggerSinkSyslog: writes to `syslog(3)`
  - LoggerSinkMonitor: write to Monitor

  The Logger is a \glos{singleton} and accessed via the Logger::Ref() static
  method.

  \note On \glos{loglevel}
    - a message is created and passed to the Logger core when the severity is
      equal or higher than the \glos{loglevel} of the \glos{DObject} or class
      which generates the message
    - a message is written to a LoggerSink when when the severity is equal or
      higher than the \glos{loglevel} of the LoggerSink.
    - only when both conditions are met a message will show up in a sink.

  \note On \ref objectownership
    - is owned by Context

  \note **Implementation notes**
  - the macros first evaluate the selection, and create message context and
    body only when the message is actually written. The `operator<<()` are
    only executed for messages passed on to the Logger core.
  - the Logger uses a worker thread named "Dca:logger" and a `mutex` to
    protect the message queue. The code sequences executed under `mutex` lock
    use `std::move` and are absolutely minimal, contention when locking the
    `mutex` is thus very unlikely:
    - at message creation: just a `vector::push_back(move(...))`
    - at message processing: just a `vector::swap(...)`
*/

//-----------------------------------------------------------------------------
/*! \brief Constructor
  \throws Exception in case Logger is already instantiated
  \throws SysCallException in case a system calls fails

  Initializes the Logger \glos{singleton} and creates a work thread with
  the name "Dca:logger" for processing the messages.
 */

Logger::Logger() :
  fSevCode2Text{"Trace", "Debug", "Info", "Note", "Warning", "Error", "Fatal"}
{
  // singleton check
  if (fpSingleton)
    throw Exception("Logger::ctor: already instantiated");

  // setup eventfd
  int fd = ::eventfd(0U, 0);
  if (fd < 0)
    throw SysCallException("Logger::ctor"s, "eventfd"s, errno);
  fEvtFd.Set(fd);

  // get hostname
  char hostname[80];
  if (int rc = ::gethostname(hostname, sizeof(hostname)); rc < 0)
    throw SysCallException("Logger::ctor"s, "gethostname"s, errno);
  fHostName = hostname;

  // get progname
  fProgName = PThreadName();

  // start EventLoop
  fThread = thread([this](){ EventLoop(); });

  fpSingleton = this;
}

//-----------------------------------------------------------------------------
/*! \brief Destructor

  Calls Stop(), which will trigger the processing of all still pending
  messages and termination of the work thread.
 */

Logger::~Logger() {
  fpSingleton = nullptr;
  Stop();
}

//-----------------------------------------------------------------------------
/*! \brief Create and return a LoggerStream object
  \param sev   message severity
  \param keys1 primary environment keys
  \param mid   \glos{messageid}
  \param keys2 seconday environment keys
  \returns LoggerMessage object
 */

LoggerStream Logger::MakeStream(int sev, const string& keys1,
                                const string& mid, const string& keys2) {
  return LoggerStream(*this, sev, keys1, mid, keys2);
}

//-----------------------------------------------------------------------------
/*! \brief Convert numerical severity code into a text
  \param sevcode   severity code
  \param nothrow   controls response for invalid `sevcode`, default `false`
  \returns  severity text or "" if `nothrow=true` and `sevcode` invalid
  \throws Exception if `nothrow=false` and `sevcode` invalid
 */

string Logger::SeverityCode2Text(int sevcode, bool nothrow) {
  if (sevcode >= 0 && sevcode <= kLogFatal) {
    return fSevCode2Text[size_t(sevcode)];
  }
  if (nothrow) return ""s;
  throw Exception(fmt::format("Logger::SeverityCode2Text:"
                              " invalid severity code {}", sevcode));
}

//-----------------------------------------------------------------------------
/*! \brief Convert severity text into severity code
  \param sevtext   severity text
  \param nothrow   controls response for invalid `sevtext`, default `false`
  \returns  severity text or "" if `nothrow=true` and `sevtext` invalid
  \throws Exception if `nothrow=false` and `sevtext` invalid
 */

int Logger::SeverityText2Code(const string& sevtext, bool nothrow) {
  for (size_t i=0; i<fSevCode2Text.size(); i++)
    if (fSevCode2Text[i] == sevtext) return int(i);
  if (nothrow) return -1;
  throw Exception(fmt::format("Logger::SeverityText2Code:"
                              " invalid severity text '{}'", sevtext));
}

//-----------------------------------------------------------------------------
/*! \brief Open a new sink
  \param sname    sink name, given as proto:path
  \param lvl      \glos{loglevel} for writing

  `sname` must have the form `proto:path`. Currently supported `proto` values
  - `file`: will create a LoggerSinkFile sink
  - `syslog`: will create a LoggerSinkSyslog sink
  - `monitor`: will create a LoggerSinkMonitor sink
 */

void Logger::OpenSink(const string& sname, int lvl) {

  {
    lock_guard<mutex> lock(fSinkMapMutex);
    auto it = fSinkMap.find(sname);
    if (it != fSinkMap.end())
      throw Exception(fmt::format("Logger::OpenSink: sink '{}' already open",
                                  sname));
  }

  auto pos = sname.find(':');
  if (pos == string::npos)
    throw Exception(fmt::format("Logger::OpenSink:"
                                " no sink type specified in '{}'", sname));

  string stype = sname.substr(0,pos);
  string spath = sname.substr(pos+1);

  if (stype == "file") {
    unique_ptr<LoggerSink> uptr = make_unique<LoggerSinkFile>(*this, spath, lvl);
    lock_guard<mutex> lock(fSinkMapMutex);
    fSinkMap.try_emplace(sname, move(uptr));
  } else if (stype == "syslog") {
    unique_ptr<LoggerSink> uptr = make_unique<LoggerSinkSyslog>(*this, spath,
                                                                lvl);
    lock_guard<mutex> lock(fSinkMapMutex);
    fSinkMap.try_emplace(sname, move(uptr));
  } else if (stype == "monitor") {
    unique_ptr<LoggerSink> uptr = make_unique<LoggerSinkMonitor>(*this, spath,
                                                                 lvl);
    lock_guard<mutex> lock(fSinkMapMutex);
    fSinkMap.try_emplace(sname, move(uptr));
  } else {
    throw Exception(fmt::format("Logger::OpenSink: invalid sink type '{}'",
                                stype));
  }
}

//-----------------------------------------------------------------------------
/*! \brief Close a sink
  \param sname    sink name, given as proto:path
  \throws Exception if no sink named `sname` exists
 */

void Logger::CloseSink(const string& sname) {
  lock_guard<mutex> lock(fSinkMapMutex);
  if (fSinkMap.erase(sname) == 0)
    throw Exception(fmt::format("Logger::CloseSink: sink '{}' not found",
                                sname));
}

//-----------------------------------------------------------------------------
//! \brief Return list of all sinks

vector<string> Logger::SinkList() {
  vector<string> res;
  lock_guard<mutex> lock(fSinkMapMutex);
  for (auto& kv : fSinkMap) res.push_back(kv.first);
  return res;
}

//-----------------------------------------------------------------------------
/*! \brief Return \glos{loglevel} of a sink
  \param sname    sink name, given as proto:path
  \throws Exception if no sink named `sname` exists
 */

int Logger::SinkLogLevel(const string& sname) {
  lock_guard<mutex> lock(fSinkMapMutex);
  return SinkRef(sname).LogLevel();
}

//-----------------------------------------------------------------------------
/*! \brief Set \glos{loglevel} of a sink
  \param sname    sink name, given as proto:path
  \param lvl      \glos{loglevel} for writing
  \throws Exception if no sink named `sname` exists
 */

void Logger::SetSinkLogLevel(const string& sname, int lvl) {
  lock_guard<mutex> lock(fSinkMapMutex);
  SinkRef(sname).SetLogLevel(lvl);
}

//-----------------------------------------------------------------------------
/*! \brief Queues a message
  \param msg   LoggerMessage object, which will be `move`ed to the message queue
 */

void Logger::QueueMessage(LoggerMessage&& msg) {
  bool wakeup = msg.fSevId >= kLogNote;
  {
    lock_guard<mutex> lock(fMsgVecMutex);
    fMsgVec.push_back(move(msg));
  }
  if (wakeup) Wakeup();
}

//-----------------------------------------------------------------------------
/*! \brief Stop Logger work thread

  Calls Wakeup() to wakeup the work thread. This triggers the processing
  of all still pending messages, after that the thread will terminate.
  Stop() joins the work thread, after that the Logger object can be
  safely destructed.
 */

void Logger::Stop() {
  fStopped = true;
  Wakeup();
  if (fThread.joinable()) fThread.join();
}

//-----------------------------------------------------------------------------
/*! \brief Wakeup work thread

  Uses an `eventfd` channel to wakeup the work thread. Is used to trigger
  the processing of message, e.g. after `Error` and `Fatal`
  messages, and in thread shutdown with Stop().
 */

void Logger::Wakeup() {
  uint64_t one(1);
  if (::write(fEvtFd, &one, sizeof(one)) != sizeof(one))
    throw SysCallException("Logger::Wakeup"s, "write"s, "fEvtFd"s, errno);
}

//-----------------------------------------------------------------------------
/*! \brief The event loop of Logger work thread
 */

void Logger::EventLoop() {
  // set worker thread name, for convenience (e.g. for top 'H' display)
  SetPThreadName("Dca:logger");

  pollfd polllist[1];
  polllist[0] = pollfd{fEvtFd, POLLIN, 0};

  while (true) {
    ::poll(polllist, 1, kELoopTimeout);     // timeout results in auto flush

    // handle fEvtFd -------------------------------------------------
    if (polllist[0].revents == POLLIN) {
      uint64_t cnt=0;
      if (::read(fEvtFd, &cnt, sizeof(cnt)) != sizeof(cnt))
        throw SysCallException("Logger::EventLoop"s, "read"s, "fEvtFd"s, errno);
    }

    msgvec_t msgvec;
    {
      lock_guard<mutex> lock(fMsgVecMutex);
      if (!fMsgVec.empty()) {
        // move whole vector from protected queue to local environment
        msgvec.swap(fMsgVec);
        // determine sensible capacity to minimize re-allocs
        size_t ncap = msgvec.capacity();
        if (msgvec.size() > msgvec.capacity()/2) ncap += ncap/2;
        else ncap = ncap/2;
        if (ncap < 4) ncap = 4;
        fMsgVec.reserve(ncap);
      }
    }

    if (msgvec.size() > 0) {
      lock_guard<mutex> lock(fSinkMapMutex);
      for (auto& kv : fSinkMap) (*kv.second).ProcessMessageVec(msgvec);
    }

    if (fStopped) break;
  } // while (true)
}

//-----------------------------------------------------------------------------
/*! \brief Returns reference to a sink
  \param sname    sink name, given as proto:path
  \throws Exception if no sink named `sname` exists
  \returns LoggerSink reference
 */

LoggerSink& Logger::SinkRef(const string& sname) {
  auto it = fSinkMap.find(sname);
  if (it == fSinkMap.end())
    throw Exception(fmt::format("Logger::SinkRef: sink '{}' not found", sname));
  return *(it->second.get());
}

//-----------------------------------------------------------------------------
// define static member variables

Logger* Logger::fpSingleton = nullptr;

//+++ Document the macros +++++++++++++++++++++++++++++++++++++++++++++++++++++
/*!
  \def DCALOG(sel,sev,keys1,mid,keys2)
  \brief Writes a message under selector `sel` with severity `sev` and keys
  \param sel   selection expression, must return or convert to a `bool`
  \param sev   severity
  \param keys1 primary environment keys
  \param mid   \glos{messageid}
  \param keys2 seconday environment keys

  This macro provides a _stream like_ interface to Logger. It
  - executes the selector `sel` in an `if(sel)` statement
  - if `true`, creates a message context with Logger::MakeMessage()
  - and expects that the message body is streamed in with an `operator<<()`
    without a trailing "\n" or `endl`

  This macro is used by DCALOGGEN(sev,mid,keys), DCALOGGEN1(sev,mid,keys),
  DCALOGNOT1(keys1,mid), DCALOGERR1(keys1,mid), and DCALOGFAT1(keys1,mid)
  and usually not used directly.
*/
/*!
  \def DCALOGGEN(sev,mid,keys)
  \brief Writes a message if `sev` is `>=` the local `LogLevel()`
  \param sev   severity
  \param mid   \glos{messageid}
  \param keys  secondary environment keys

  This macro provides a _stream like_ interface to Logger. It expects that the
  methods `LogLevel()` and  `LogKeys()` are defined in the execution context.
  If `sev` is larger or equal `LogLevel()` it creates a message context and
  passes `LogKeys()` as primary key set, `mid` as value of the `mid=` key,
  and `keys` as secondary key set.
  The message body must be streamed in with an `operator<<()` without a
  trailing "\n" or `endl`.
  Typical usage is
  \code{.cpp}
  DCALOGGEN(Logger::kLogWarning,"<mid>","") << "text: a1=" << a1 << ",a2=" << a2;
  \endcode

  This macro is used by DCALOGERR(mid,keys), DCALOGWAR(mid,keys),
  DCALOGNOT(mid,keys), DCALOGINF(mid,keys), DCALOGDEB(mid,keys),
  DCALOGTRA(mid,keys), and DCALOGTRAOBJ(obj,mid,keys) and rarely used directly.
*/
/*!
  \def DCALOGGEN1(sev,mid,keys)
  \brief Writes a message unconditionally
  \param sev   severity
  \param mid   \glos{messageid}
  \param keys  secondary environment keys

  This macro provides a _stream like_ interface to Logger. It expects that the
  method `LogKeys()` is defined in the execution context.
  It unconditionally creates a message context and passes `LogKeys()` as primary
  key set, `mid` as vakue of the `mid=` key, and `keys` as secondary key set.
  The message body must be streamed in with an `operator<<()` without a
  trailing "\n" or `endl`.

  This macro is used by DCALOGNOTI(mid,keys) and DCALOGFAT(mid,keys)
  and rarely used directly.
*/
/*!
  \def DCALOGERR(mid,keys)
  \brief Writes a `Error` level message if allowed by local `LogLevel()`
  \param mid   \glos{messageid}
  \param keys  secondary environment keys

  This macro provides a _stream like_ interface to Logger.
  It expects that the methods `LogLevel()` and `LogKeys()` are defined in the
  execution context.
  If severity `Error` is compatile with `LogLevel()` it creates a message
  context and passes `LogKeys()` as primary key set, `mid` as vakue of the 
  `mid=` key, and `keys` as secondary key set.

  The message body must be streamed in with an `operator<<()` without a
  trailing "\n" or `endl`.
  Typical usage is
  \code{.cpp}
  DCALOGERR("<mid>","") << "text: a1=" << a1 << ",a2=" << a2;
  \endcode
*/
/*!
  \def DCALOGWAR(mid,keys)
  \brief Writes a `Warning` level message if allowed by local `LogLevel()`

  Works like DCALOGERR(mid,keys), but creates a severity `Warning` message.
*/
/*!
  \def DCALOGNOT(mid,keys)
  \brief Writes a `Note` level message if allowed by local `LogLevel()`

  Works like DCALOGERR(mid,keys), but creates a severity `Note` message.
*/
/*!
  \def DCALOGINF(mid,keys)
  \brief Writes a `Info` level message if allowed by local `LogLevel()`

  Works like DCALOGERR(mid,keys), but creates a severity `Info` message.
*/
/*!
  \def DCALOGDEB(mid,keys)
  \brief Writes a `Debug` level message if allowed by local `LogLevel()`

  Works like DCALOGERR(mid,keys), but creates a severity `Debug` message.
*/
/*!
  \def DCALOGTRA(mid,keys)
  \brief Writes a `Trace` level message if allowed by local `LogLevel()`

  Works like DCALOGERR(mid,keys), but creates a severity `Trace` message.
*/
/*!
  \def DCALOGFAT(mid,keys)
  \brief Writes a `Fatal` level message unconditionally

  Works like DCALOGERR(mid,keys), but unconditionally creates a severity 
  `Fatal` message
*/
/*!
  \def DCALOGNOTI(mid,keys)
  \brief Writes a `Note` level message unconditionally

  Works like DCALOGNOT(mid,keys), but bypasses the `LogLevel()` check.
  This macro is used for very essential status messages which should always
  be logged. It should be used sparingly.
*/
/*!
  \def DCALOGTRAOBJ(obj,mid,keys)
  \brief Writes a `Trace` level message using the context of another object.
  \param obj   reference to an object
  \param mid   \glos{messageid}
  \param keys  secondary environment keys

  Works similar to DCALOGTRA(keys), but allows to use the context of another
  object `obj` for `LogLevel()` and `LogKeys()`.
  It expects that the methods `obj.LogLevel()` and `obj.LogKeys()` are defined.
  If severity `Trace` is compatile with `obj.LogLevel()` it creates a message
  context and passes `obj.LogKeys()` as primary key set, uses `mid` as vakue 
  of the `mid=` key, and `keys` as secondary key set.

  The message body must be streamed in with an `operator<<()` without a
  trailing "\n" or `endl`.
*/

/*!
  \def DCALOGFAT1(keys1,mid)
  \brief Writes unconditionally a severity `Fatal` message
  \param keys1 primary environment keys
  \param mid   \glos{messageid}

  This macro provides a _stream like_ interface to Logger. It creates a
  message context with Logger::MakeMessage() and expects that the message
  body is streamed in with an `operator<<()` without a trailing "\n" or `endl.`
  Typical usage is
  \code{.cpp}
  DCALOGFAT1("cid_=<cid>","<mid>") << "text: a1=" << a1 << ",a2=" << a2;
  \endcode

  Is used in situations were fault recovery is impossible.
*/
/*!
  \def DCALOGERR1(keys1,mid)
  \brief Writes unconditionally a severity `Error` message
  Works like DCALOGFAT1(keys1,mid), but creates a severity `Error` message.
  This macro is used for very essential status messages which should always
  be logged. It should be used sparingly.
*/
/*!
  \def DCALOGNOT1(keys1,mid)
  \brief Writes unconditionally a severity `Note` message
  Works like DCALOGFAT1(keys1,mid), but creates a severity `Info` message.
  This macro is used for very essential status messages which should always
  be logged. It should be used sparingly.
*/
  
} // end namespace cbm
