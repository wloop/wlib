set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Waddress -Warray-bounds -Wbuiltin-macro-redefined -Wconversion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winit-self -Wnon-virtual-dtor -Woverloaded-virtual")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wswitch -Wunreachable-code")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsuggest-attribute=noreturn -Wsuggest-attribute=pure -Wsuggest-attribute=const")

set(GTEST_INCLUDE_DIR ${gtest_SOURCE_DIR}/include)
set(WLIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/wlib)

include_directories(${GTEST_INCLUDE_DIR})
include_directories(${WLIB_INCLUDE_DIR})

file(GLOB files
        "test.cpp"
        "template_defs.h"
        "stl/*.cpp"
        "fsm/*.cpp"
        "strings/*.cpp")

add_executable(tests ${files})
target_link_libraries(tests gtest)
target_link_libraries(tests wlib)
add_dependencies(tests wlib)
add_dependencies(tests gtest)
