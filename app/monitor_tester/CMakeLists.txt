# SPDX-License-Identifier: GPL-3.0-only
# (C) Copyright 2022 Johann Wolfgang Goethe-Universität Frankfurt
# Original author: Jan de Cuveland <cuveland@compeng.uni-frankfurt.de>

file(GLOB APP_SOURCES *.cpp)
file(GLOB APP_HEADERS *.hpp)

add_executable(monitoring_tester ${APP_SOURCES} ${APP_HEADERS})

target_link_libraries(monitoring_tester
  PUBLIC monitoring
  PUBLIC Boost::boost
  PUBLIC Boost::program_options
)

target_compile_options(monitoring_tester PRIVATE -Wall -Wextra -Wpedantic)
