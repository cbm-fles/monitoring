// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2022 Johann Wolfgang Goethe-Universität Frankfurt
// Original author: Jan de Cuveland <cuveland@compeng.uni-frankfurt.de>

#include "Application.hpp"
#include "PThreadHelper.hpp"
#include <chrono>
#include <iostream>
#include <thread>

Application::Application(Parameters const& par) : par_(par) {

  // start up Monitor --------------------------------------
  monitor_ = std::make_unique<cbm::Monitor>();
  if (!par.monitor_uri.empty()) {
    monitor_->OpenSink(par.monitor_uri);
  } else {
    monitor_->OpenSink("file:cout");
  }
}

void Application::run() {
  // do something
  cbm::Monitor::Ref().QueueMetric(
      "demo_measurement", {{"hostname", "N/A"}},
      {{"an_int", 17}, {"a_float", 1.7}, {"a_bool", true}});
}

Application::~Application() {
  // delay to allow monitor to process pending messages
  constexpr auto destruct_delay = std::chrono::milliseconds(200);
  std::this_thread::sleep_for(destruct_delay);
}
