// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#include "LoggerSink.hpp"

namespace cbm {
using namespace std;

/*! \class LoggerSink
  \brief Logger sink - abstract base class

  This class provides an abstract interface for the Logger sink layer.
  Concrete implementations are
  - LoggerSinkFile: concrete sink for file output
  - LoggerSinkSysLog: concrete sink for syslog(3) output
*/

//-----------------------------------------------------------------------------
/*! \brief Constructor
  \param logger back reference to Logger
  \param path   path for output
  \param lvl    \glos{loglevel} for writing
 */

LoggerSink::LoggerSink(Logger& logger, const string& path, int lvl)
    : fLogger(logger), fSinkPath(path), fLogLevel(lvl) {}

} // end namespace cbm
