set(GTEST_INCLUDE_DIR ${gtest_SOURCE_DIR}/include)
set(WLIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/wlib)

include_directories(${GTEST_INCLUDE_DIR})
include_directories(${WLIB_INCLUDE_DIR})

file(GLOB files
        "test.cpp"
        "strings/*.cpp")

add_executable(tests ${files})
target_link_libraries(tests gtest)
target_link_libraries(tests wlib)
ADD_DEPENDENCIES(tests wlib)
ADD_DEPENDENCIES(tests gtest)
