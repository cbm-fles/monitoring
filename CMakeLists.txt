# 2022-05-17, Jan de Cuveland

cmake_minimum_required(VERSION 3.14)

project(Monitoring VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS program_options)
find_package(Threads REQUIRED)

include(FetchContent)

# Formatting library
FetchContent_Declare(
  fmtlib
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 5.3.0)
FetchContent_MakeAvailable(fmtlib)
# Adds fmt::fmt

add_subdirectory(src)
add_subdirectory(app/tester)
