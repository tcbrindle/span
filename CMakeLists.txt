
cmake_minimum_required(VERSION 3.2)
project(span LANGUAGES CXX)

add_library(span INTERFACE)
target_sources(span INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/tcb/span.hpp)
target_include_directories(span INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(span INTERFACE cxx_std_11)

add_subdirectory(test)
