cmake_minimum_required(VERSION 3.1)
project(EmbeddedTests)

add_definitions(-std=gnu++0x)
add_definitions(-D_EMULATE_GLIBC=0)

add_subdirectory(gtest-1.8.0)

include_directories(wlib)

add_subdirectory(wlib)
add_subdirectory(tests)



