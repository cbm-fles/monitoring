// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020-2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_Context
#define included_Dca_Context 1

#include "Logger.hpp"
#include "Monitor.hpp"

#include <memory>
#include <unordered_map>

#include <signal.h>

namespace cbm {
using namespace std;

class Context {
  public:

                    Context();
                    ~Context();

  int               Init(int argc, char* argv[]);

  bool              TstOpt(const string& opt);
  const string&     GetOptString(const string& opt, const string& def);
  int               GetOptInt(const string& opt, int def);

  const string&     ProgName() const;

  static Context&   Ref();
  static Context*   Ptr();

  private:
  void              ConnectSignalCatcher(int signum);
  [[noreturn]] static void  SignalCatcher(int signum, siginfo_t* siginf, void*);

  private:
  unique_ptr<Logger>         fpLogger;      //!< unique_ptr of Logger
  unique_ptr<Monitor>        fpMonitor;     //!< unique_ptr of Monitor
  unordered_map<string,string> fOptMapOpen; //!< options still open
  unordered_map<string,string> fOptMapDone; //!< options alread processed
  string            fProgName;              //!< program name
  static Context*   fpSingleton;            //!< \glos{singleton} this
};

} // end namespace cbm

#include "Context.ipp"

#endif
