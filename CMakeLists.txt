cmake_minimum_required(VERSION 3.10)
project(shiki)

set(CMAKE_CXX_STANDARD 11)

add_executable(shiki main.cpp)
install (TARGETS shiki RUNTIME DESTINATION bin)
