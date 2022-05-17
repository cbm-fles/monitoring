// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020-2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Cbm_Application
#define included_Cbm_Application 1

#include "Logger.hpp"
#include "Monitor.hpp"

#include <memory>
#include <unordered_map>

#include <signal.h>

namespace cbm {
using namespace std;

class Application {
  public:

                    Application();
                    ~Application();

  int               Init(int argc, char* argv[]);

  bool              TstOpt(const string& opt);
  const string&     GetOptString(const string& opt, const string& def);
  int               GetOptInt(const string& opt, int def);

  const string&     ProgName() const;

  static Application&   Ref();
  static Application*   Ptr();

  private:
  void              ConnectSignalCatcher(int signum);
  [[noreturn]] static void  SignalCatcher(int signum, siginfo_t* siginf, void*);

  private:
  unique_ptr<Logger>         fpLogger;      //!< unique_ptr of Logger
  unique_ptr<Monitor>        fpMonitor;     //!< unique_ptr of Monitor
  unordered_map<string,string> fOptMapOpen; //!< options still open
  unordered_map<string,string> fOptMapDone; //!< options alread processed
  string            fProgName;              //!< program name
  static Application*   fpSingleton;            //!< \glos{singleton} this
};

} // end namespace cbm

#include "Application.ipp"

#endif
