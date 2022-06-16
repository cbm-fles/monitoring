// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#include "LoggerSinkFile.hpp"

#include "ChronoHelper.hpp"
#include "Exception.hpp"
#include "Logger.hpp"

#include "fmt/format.h"

#include <fstream>

namespace cbm {
using namespace std;

/*! \class LoggerSinkFile
  \brief Logger sink - concrete sink for file output
*/

//-----------------------------------------------------------------------------
/*! \brief Constructor
  \param logger back reference to Logger
  \param path   filename
  \param lvl    \glos{loglevel} for writing

  Write messages to a file named `path`. The special names `cout` and `cerr`
  will open these standard streams, in all other cases a file will be opened.

  Several LoggerSinkFile sinks can be opened simultaneously and operated
  with different \glos{loglevel} settings.
 */

LoggerSinkFile::LoggerSinkFile(Logger& logger, const string& path, int lvl)
    : LoggerSink(logger, path, lvl) {
  if (path == "cout"s) {
    fpCout = &cout;
  } else if (path == "cerr"s) {
    fpCout = &cerr;
  } else {
    fpOStream = make_unique<ofstream>(path);
    if (!fpOStream->is_open())
      throw Exception(
          fmt::format("LoggerSinkFile::ctor: open() failed for '{}'", path));
  }
}

//-----------------------------------------------------------------------------
/*! \brief Process a vector of messages
 */

void LoggerSinkFile::ProcessMessageVec(const vector<LoggerMessage>& msgvec) {
  ostream& os = fpCout ? *fpCout : *fpOStream;
  size_t msgcnt = 0;

  for (auto& msg : msgvec) {
    if (msg.fSevId < fLogLevel)
      continue;
    msgcnt += 1;
    string keys =
        fmt::format("host={},thread={},sev={}", fLogger.HostName(),
                    msg.fThreadName, fLogger.SeverityCode2Text(msg.fSevId));
    if (msg.fKeys.length())
      keys += "," + msg.fKeys;
    os << TimePoint2String(msg.fTime) << ": {" << keys << "}: " << msg.fMessage
       << "\n";
  }
  if (msgcnt > 0)
    os.flush();
}

} // end namespace cbm
