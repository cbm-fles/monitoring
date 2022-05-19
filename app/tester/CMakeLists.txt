# 2022-05-17, Jan de Cuveland

file(GLOB APP_SOURCES *.cpp)
file(GLOB APP_HEADERS *.hpp)

add_executable(tester ${APP_SOURCES} ${APP_HEADERS})

target_link_libraries(tester
  PUBLIC monitoring
  PUBLIC Boost::boost
  PUBLIC Boost::program_options
)
