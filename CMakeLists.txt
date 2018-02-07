project(wlib)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DISTRIBUTION "-fno-exceptions -fno-threadsafe-statics -fpermissive -std=gnu++11 -g -Os -Wall -ffunction-sections -fdata-sections -flto")
set(CMAKE_C_FLAGS_DISTRIBUTION "-std=gnu11 -fno-fat-lto-objects -g -Os -Wall -ffunction-sections -fdata-sections -flto")
add_definitions(-I/${CMAKE_CURRENT_SOURCE_DIR})

#[[
    These definitions are used for how the memory is allocated

    MAX_ALLOCATORS - Number of allocators (how many sizes of memory blocks)
    NUM_BLOCKS - Number of block each allocator will contain

    * Type of memory available. Available types:
    * STATIC_POOL  -> Static memory will be used and all the memory will be reserved at start
    * DYNAMIC_POOL -> Dynamic memory will be used and all the memory will be reserved at start
    * NO_POOL      -> Dynamic memory will be used and Allocators will be expanded at run time
    *
    * If you don't define any of these then no memory will created at the beginning and
    * memory will be expanded at run time
    */
]]
add_definitions(-DWLIB_DEBUG -DMAX_ALLOCATORS=10u -DNUM_BLOCKS=100u -DDYNAMIC_POOL)

file(GLOB_RECURSE header_files "wlib/*.h")

file(GLOB_RECURSE source_files "wlib/*.cpp" "wlib/*.cc")

set(HEADER_FILES ${header_files})
set(SOURCE_FILES ${source_files})

add_library(wlib STATIC ${SOURCE_FILES} ${HEADER_FILES})
