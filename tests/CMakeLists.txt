project(tests)

set(GTEST_INCLUDE_DIR ${gtest_SOURCE_DIR}/include)
set(WLIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/wlib)

file(GLOB files
        "strings/*.cpp")

include_directories(${GTEST_INCLUDE_DIR} ${gtest_SOURCE_DIR})
include_directories(${WLIB_INCLUDE_DIR})

add_executable(Tests ${files} stl/equals_check.cpp)

target_link_libraries(Tests gtest gtest_main)
target_link_libraries(Tests wlib)
ADD_DEPENDENCIES(Tests wlib)
ADD_DEPENDENCIES(Tests gtest)
