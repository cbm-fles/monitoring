// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_LoggerSinkFile
#define included_Dca_LoggerSinkFile 1

#include "LoggerSink.hpp"

#include <fstream>
#include <iostream>
#include <memory>

namespace Dca {
using namespace std;
  
class LoggerSinkFile : public LoggerSink {
  public:
                    LoggerSinkFile(Logger& logger, const string& path, int lvl);

  virtual void      ProcessMessageVec(const vector<LoggerMessage>& msgvec);

  private:
  ostream*          fpCout {nullptr};       //!< pointer to cout/cerr
  unique_ptr<ofstream>  fpOStream {};       //!< uptr to general output stream
};

} // end namespace Dca

//#include "LoggerSinkFile.ipp"

#endif
