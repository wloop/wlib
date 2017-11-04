include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

file(GLOB files
        "strings/*.cpp"
        "strings2/*.cpp"
        "strings/*.cc"
        "stl/*_check.cpp")

add_executable(Tests ${files})

target_link_libraries(Tests gtest gtest_main)
target_link_libraries(wlib)
