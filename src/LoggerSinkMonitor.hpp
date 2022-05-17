// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_LoggerSinkMonitor
#define included_Dca_LoggerSinkMonitor 1

#include "LoggerSink.hpp"

namespace Dca {
using namespace std;

class LoggerSinkMonitor : public LoggerSink {
  public:
                    LoggerSinkMonitor(Logger& logger, const string& path,
                                      int lvl);

  virtual void      ProcessMessageVec(const vector<LoggerMessage>& msgvec);

  private:
};

} // end namespace Dca

//#include "LoggerSinkMonitor.ipp"

#endif
