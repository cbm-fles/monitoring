// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Cbm_LoggerSinkFile
#define included_Cbm_LoggerSinkFile 1

#include "LoggerSink.hpp"

#include <fstream>
#include <iostream>
#include <memory>

namespace cbm {
using namespace std;

class LoggerSinkFile : public LoggerSink {
public:
  LoggerSinkFile(Logger& logger, const string& path, int lvl);

  virtual void ProcessMessageVec(const vector<LoggerMessage>& msgvec);

private:
  ostream* fpCout{nullptr};         //!< pointer to cout/cerr
  unique_ptr<ofstream> fpOStream{}; //!< uptr to general output stream
};

} // end namespace cbm

//#include "LoggerSinkFile.ipp"

#endif
