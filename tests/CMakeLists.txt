set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Waddress -Warray-bounds -Wbuiltin-macro-redefined -Wconversion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winit-self -Wnon-virtual-dtor -Woverloaded-virtual")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wswitch -Wunreachable-code")

set(GTEST_INCLUDE_DIR ${gtest_SOURCE_DIR}/include)
set(WLIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/wlib)

include_directories(${GTEST_INCLUDE_DIR})
include_directories(${WLIB_INCLUDE_DIR})

file(GLOB files
		"*.h"
		"test.cpp"
		"strings/*.cpp"
		"stl/*.cpp"
        "fsm/*.cpp"
		"memory/*.cpp")

add_executable(tests ${files})
target_link_libraries(tests gtest)
target_link_libraries(tests wlib)
add_dependencies(tests wlib)
add_dependencies(tests gtest)
