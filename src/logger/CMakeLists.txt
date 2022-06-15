# SPDX-License-Identifier: GPL-3.0-only
# (C) Copyright 2022 Johann Wolfgang Goethe-Universität Frankfurt
# Original author: Jan de Cuveland <cuveland@compeng.uni-frankfurt.de>

file(GLOB LIB_SOURCES *.cpp)
file(GLOB LIB_HEADERS *.hpp)
file(GLOB LIB_IPP *.ipp)

add_library(logging ${LIB_SOURCES} ${LIB_IPP} ${LIB_HEADERS})

target_include_directories(logging PUBLIC .)

target_link_libraries(logging
  PRIVATE Boost::boost
  PUBLIC monitoring
  PUBLIC Threads::Threads
  PUBLIC fmt::fmt
)

target_compile_features(logging PUBLIC cxx_std_17)

target_compile_options(logging PRIVATE -Wall -Wextra -Wpedantic)
