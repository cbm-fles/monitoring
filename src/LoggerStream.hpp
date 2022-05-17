// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Cbm_LoggerStream
#define included_Cbm_LoggerStream 1

#include "ChronoDefs.hpp"

#include <sstream>

namespace cbm {

class Logger;                               // forward declaration

class LoggerStream {
  public:
                    LoggerStream() = delete;
                    LoggerStream(Logger& logger, int sev, const string& keys1,
                                 const string& mid, const string& keys2);
                    ~LoggerStream();

  ostream&          Stream();

  private:
  Logger&           fLogger;                //!< back reference to Logger
  sctime_point      fTime;                  //!< timestamo
  int               fSevId;                 //!< severity
  string            fThreadName;            //!< thread name
  string            fKeys;                  //!< key string
  ostringstream     fSStream;               //!< message
};

} // end namespace cbm

#include "LoggerStream.ipp"

#endif
