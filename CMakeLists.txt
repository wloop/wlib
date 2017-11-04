cmake_minimum_required(VERSION 3.1)
project(EmbeddedTests)

SET(CMAKE_CXX_STANDARD 14)

add_subdirectory(gtest-1.8.0)
add_subdirectory(wlib)
add_subdirectory(tests)
