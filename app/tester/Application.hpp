// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2022 Johann Wolfgang Goethe-Universität Frankfurt
// Original author: Jan de Cuveland <cuveland@compeng.uni-frankfurt.de>

#ifndef INCLUDE_APPLICATION
#define INCLUDE_APPLICATION

#include "Logger.hpp"
#include "Monitor.hpp"
#include "Parameters.hpp"
#include <memory>

class Application {
public:
  explicit Application(Parameters const& par);
  ~Application();
  void run();

  Application(const Application&) = delete;
  void operator=(const Application&) = delete;

private:
  /// The run parameters object.
  Parameters const& par_;

  std::unique_ptr<cbm::Logger> logger_;
  std::unique_ptr<cbm::Monitor> monitor_;
};

#endif
