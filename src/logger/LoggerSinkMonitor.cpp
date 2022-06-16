// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#include "LoggerSinkMonitor.hpp"

#include "ChronoHelper.hpp"
#include "Exception.hpp"
#include "Logger.hpp"
#include "Metric.hpp"
#include "Monitor.hpp"

namespace cbm {
using namespace std;

/*! \class LoggerSinkMonitor
  \brief Logger sink - concrete sink for file output
*/

//-----------------------------------------------------------------------------
/*! \brief Constructor
  \param logger back reference to Logger
  \param path   filename (ignored)
  \param lvl    \glos{loglevel} for writing

  Write messages to Monitor. The Metric tag set it contains
  - thread (from message)
  - severity (numeric, from message)
  - all other keys from the message key list

  The Metric field set contains a single field named 'msg' with the message
  body.
 */

LoggerSinkMonitor::LoggerSinkMonitor(Logger& logger,
                                     const string& path,
                                     int lvl)
    : LoggerSink(logger, path, lvl) {}

//-----------------------------------------------------------------------------
/*! \brief Process a vector of messages
 */

void LoggerSinkMonitor::ProcessMessageVec(const vector<LoggerMessage>& msgvec) {
  for (auto& msg : msgvec) {
    if (msg.fSevId < fLogLevel)
      continue;
    MetricTagSet tagset = {{"thread", msg.fThreadName},
                           {"sev", to_string(msg.fSevId)}};
    size_t pbeg = 0;
    size_t pend = 0;
    bool cidmoni = false;
    while ((pbeg = msg.fKeys.find_first_not_of(',', pend)) != string::npos) {
      pend = msg.fKeys.find(',', pbeg);
      string keyval = msg.fKeys.substr(pbeg, pend - pbeg);
      size_t pdel = keyval.find('=');
      if (keyval == "cid=__Monitor")
        cidmoni = true;
      if (pdel != string::npos && pdel > 0 && pdel + 1 < keyval.size())
        tagset.emplace_back(keyval.substr(0, pdel), keyval.substr(pdel + 1));
    }

    // drop messages related to Monitor with a severity of Warning or above.
    // They will be related to MonitorSink processing, will lileky not be
    // delivered anyway, and might create an eternal Logger/Monitor loop
    if (cidmoni && msg.fSevId >= Logger::kLogWarning)
      continue;

    // ensure that Monitor is running, it is started after Logger and stopped
    // before Logger. A very early or very late Logger messages are therefore
    // not transfered to Monitor.
    if (Monitor::Ptr())
      Monitor::Ptr()->QueueMetric("Logger", move(tagset),
                                  {{"msg", msg.fMessage}}, msg.fTime);
  }
}

} // end namespace cbm
