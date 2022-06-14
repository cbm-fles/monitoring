// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2022 Johann Wolfgang Goethe-Universität Frankfurt
// Original author: Jan de Cuveland <cuveland@compeng.uni-frankfurt.de>

#include "Application.hpp"
#include "PThreadHelper.hpp"
#include <chrono>
#include <iostream>
#include <thread>

Application::Application(Parameters const& par) : par_(par) {

  // start up Logger ---------------------------------------
  logger_ = std::make_unique<cbm::Logger>();
  // set main thread name, for convenience (e.g. for top 'H' display)
  // done here so that Logger can pick up the program name with PThreadName()
  cbm::SetPThreadName("Cbm:main");

  if (par.nosyslog) {
    logger_->OpenSink("file:cout", cbm::Logger::kLogWarning);
  } else {
    logger_->OpenSink("syslog:", cbm::Logger::kLogNote);
  }

  if (!par.logfile.empty()) {
    logger_->OpenSink(par.logfile, cbm::Logger::kLogTrace);
  }

  // start up Monitor --------------------------------------
  monitor_ = std::make_unique<cbm::Monitor>();
  if (!par.monitor_uri.empty()) {
    monitor_->OpenSink(par.monitor_uri);
    logger_->OpenSink("monitor:", cbm::Logger::kLogNote);
  } else {
    monitor_->OpenSink("file:cout");
  }
}

void Application::run() {
  // do something
  CBMLOGERR1("cid=__Application", "CBM-1") << "Example error message 1";
  cbm::Monitor::Ref().QueueMetric(
      "demo_measurement", {{"hostname", "N/A"}},
      {{"an_int", 17}, {"a_float", 1.7}, {"a_bool", true}});
}

Application::~Application() {
  // delay to allow logger and monitor to process pending messages
  constexpr auto destruct_delay = std::chrono::milliseconds(200);
  std::this_thread::sleep_for(destruct_delay);
}
