set(WLIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/wlib)
include_directories(${WLIB_INCLUDE_DIR})

file(GLOB files
        "./*.cpp")

add_executable(examples ${files})
target_link_libraries(examples wlib)
add_dependencies(examples wlib)
