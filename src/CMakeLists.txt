# 2022-05-17, Jan de Cuveland

file(GLOB LIB_SOURCES *.cpp)
file(GLOB LIB_HEADERS *.hpp)
file(GLOB LIB_IPP *.ipp)

add_library(monitoring ${LIB_SOURCES} ${LIB_IPP} ${LIB_HEADERS})

target_include_directories(monitoring PUBLIC .)

target_link_libraries(monitoring PRIVATE Boost::boost)

target_compile_features(monitoring PUBLIC cxx_std_17)
