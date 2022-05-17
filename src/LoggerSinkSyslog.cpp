// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020-2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#include "LoggerSinkSyslog.hpp"

#include "ChronoHelper.hpp"
#include "Exception.hpp"
#include "Logger.hpp"

#include "fmt/format.h"

#include <syslog.h>

namespace cbm {
using namespace std;

/*! \class LoggerSinkSyslog
  \brief Logger sink - concrete sink for `syslog(3)` output

  Write messages to the `syslog(3)` interface, in most current systems
  therefore to `rsyslog`. The Logger severities are mapped to the `syslog`
  severity levels, mostly one-to-one, exceptions are
  - kLogTrace --> LOG_DEBUG
  - kLogFatal --> LOG_ALERT

  Only a single instance of this sink can be opened because there is only
  a single `syslog(3)` interface available per process.
*/

//-----------------------------------------------------------------------------
/*! \brief Constructor
  \param logger back reference to Logger
  \param path   must be ""
  \param lvl    \glos{loglevel} for writing
 */

LoggerSinkSyslog::LoggerSinkSyslog(Logger& logger, const string& path,
                                   int lvl) :
  LoggerSink(logger, path, lvl)
{
  // enforce path == "", ensures that this sink type can only be created once
  if (path != "")
    throw Exception(fmt::format("LoggerSinkSyslog::ctor: path non-enpty '{}'",
                                path));

  // setup Logger -> Syslog severity mapping
  fSevMap.resize(Logger::kLogFatal+1);
  fSevMap[Logger::kLogTrace]   = LOG_DEBUG;
  fSevMap[Logger::kLogDebug]   = LOG_DEBUG;
  fSevMap[Logger::kLogInfo]    = LOG_INFO;
  fSevMap[Logger::kLogNote]    = LOG_NOTICE;
  fSevMap[Logger::kLogWarning] = LOG_WARNING;
  fSevMap[Logger::kLogError]   = LOG_ERR;
  fSevMap[Logger::kLogFatal]   = LOG_ERR;       // EMERG is too noisy !

  // open syslog connection: use facility 'local1'
  ::openlog("cbm", LOG_PID, LOG_LOCAL1);
}

//-----------------------------------------------------------------------------
/*! \brief Process a vector of messages
 */

void LoggerSinkSyslog::ProcessMessageVec(const vector<LoggerMessage>& msgvec) {
  for (auto& msg : msgvec) {
    if (msg.fSevId < fLogLevel) continue;
    string keys = fmt::format("time={},thread={},sev={}",
                              TimePoint2String(msg.fTime), msg.fThreadName,
                              fLogger.SeverityCode2Text(msg.fSevId));
    if (msg.fKeys.length()) keys += "," + msg.fKeys;

    int syslvl = LOG_ERR;
    if (msg.fSevId >= 0 && msg.fSevId <= Logger::kLogFatal)
      syslvl = fSevMap[size_t(msg.fSevId)];

    ::syslog(syslvl, "{%s}: %s", keys.c_str(), msg.fMessage.c_str());
  }
}

} // end namespace cbm
