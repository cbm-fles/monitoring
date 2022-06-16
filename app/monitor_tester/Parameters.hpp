// Copyright 2012-2013 Jan de Cuveland <cmail@cuveland.de>
// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2022 Johann Wolfgang Goethe-Universität Frankfurt
// Original author: Jan de Cuveland <cuveland@compeng.uni-frankfurt.de>

#ifndef INCLUDE_PARAMETERS
#define INCLUDE_PARAMETERS

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

/// Run parameter exception class.
/** A ParametersException object signals an error in a given parameter
    on the command line or in a configuration file. */

class ParametersException : public std::runtime_error {
public:
  /// The ParametersException constructor.
  explicit ParametersException(const std::string& what_arg = "")
      : std::runtime_error(what_arg) {}
};

/// Global run parameter class.
/** A Parameters object stores the information given on the command
    line or in a configuration file. */

class Parameters {
public:
  /// The Parameters command-line parsing constructor.
  Parameters(int argc, char* argv[]);

  Parameters(const Parameters&) = delete;
  void operator=(const Parameters&) = delete;

  std::string monitor_uri;
};

#endif
