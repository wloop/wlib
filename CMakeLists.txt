cmake_minimum_required(VERSION 3.1)
project(EmbeddedTests)

add_definitions(-std=gnu++0x)
add_definitions(-std=c++14)
add_definitions(-D_EMULATE_GLIBC=0)
SET(CMAKE_CXX_FLAGS_DISTRIBUTION "-fno-exceptions -fno-threadsafe-statics -fpermissive -std=gnu++14 -g -Os -Wall -ffunction-sections -fdata-sections -flto")
SET(CMAKE_C_FLAGS_DISTRIBUTION "-std=gnu14 -fno-fat-lto-objects -g -Os -Wall -ffunction-sections -fdata-sections -flto")
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(gtest-1.8.0)

include_directories(wlib)

add_subdirectory(wlib)
add_subdirectory(tests)



