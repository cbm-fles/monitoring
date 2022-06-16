// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Cbm_LoggerSinkSyslog
#define included_Cbm_LoggerSinkSyslog 1

#include "LoggerSink.hpp"

#include <vector>

namespace cbm {
using namespace std;

class LoggerSinkSyslog : public LoggerSink {
public:
  LoggerSinkSyslog(Logger& logger, const string& path, int lvl);

  virtual void ProcessMessageVec(const vector<LoggerMessage>& msgvec);

private:
  vector<int> fSevMap{}; //!< severity mapping
};

} // end namespace cbm

//#include "LoggerSinkSyslog.ipp"

#endif
