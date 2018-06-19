project(wlib)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DISTRIBUTION "-fno-exceptions -fno-threadsafe-statics -fpermissive -std=gnu++11 -g -Os -Wall -ffunction-sections -fdata-sections -flto")
set(CMAKE_C_FLAGS_DISTRIBUTION "-std=gnu11 -fno-fat-lto-objects -g -Os -Wall -ffunction-sections -fdata-sections -flto")
add_definitions(-I/${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE header_files "wlib/*.h")
file(GLOB_RECURSE source_files "wlib/*.cpp" "wlib/*.cc")

set(HEADER_FILES ${header_files})
set(SOURCE_FILES ${source_files})

add_library(wlib STATIC ${SOURCE_FILES} ${HEADER_FILES})

set(WLIB_STL_DIR ${CMAKE_CURRENT_LIST_DIR}/../../wlib-stl)
set(WIO_MODULES_DIR ${WLIB_STL_DIR}/.wio/node_modules)

set(wlib-tmp_dir ${WIO_MODULES_DIR}/wlib-tmp)
set(wlib-memory_dir ${WIO_MODULES_DIR}/wlib-memory)
set(wlib-malloc_dir ${WIO_MODULES_DIR}/wlib-malloc)
set(wlib-tlsf_dir ${WIO_MODULES_DIR}/wlib-tlsf)

target_include_directories(wlib PUBLIC ${wlib-tmp_dir}/include)
target_include_directories(wlib PUBLIC ${wlib-memory_dir}/include)
target_include_directories(wlib PUBLIC ${wlib-malloc_dir}/include)

foreach(dep wlib-tlsf wlib-malloc)
    file(GLOB_RECURSE ${dep}_files ${${dep}_dir}/*.cpp)
    add_library(${dep} ${${dep}_files})
    target_include_directories(${dep} PRIVATE ${${dep}_dir}/include)
endforeach()

target_link_libraries(wlib-malloc wlib-tlsf)
target_include_directories(wlib-malloc PRIVATE ${wlib-tlsf_dir}/include)
target_compile_definitions(wlib-tlsf PRIVATE
    WLIB_TLSF_32BIT
    WLIB_TLSF_LOG2_ALIGN=2
    WLIB_TLSF_LOG2_MAX=15
    WLIB_TLSF_LOG2_DIV=5)
