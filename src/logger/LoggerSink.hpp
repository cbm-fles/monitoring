// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Cbm_LoggerSink
#define included_Cbm_LoggerSink 1

#include "LoggerMessage.hpp"

#include <string>
#include <vector>

namespace cbm {
using namespace std;

class Logger; // forward declaration

class LoggerSink {
public:
  LoggerSink(Logger& logger, const string& path, int lvl);
  virtual ~LoggerSink() = default;

  virtual void ProcessMessageVec(const vector<LoggerMessage>& msgvec) = 0;

  void SetLogLevel(int lvl);
  int LogLevel() const;

protected:
  Logger& fLogger;  //!< back reference to Logger
  string fSinkPath; //!< path for output
  int fLogLevel;    //!< \glos{loglevel} for write
};

} // end namespace cbm

#include "LoggerSink.ipp"

#endif
