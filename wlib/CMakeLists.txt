project(wlib)

file (GLOB header_files
        "strings/*.h"
        "stl/*.h"
        "memory/*.h")

file (GLOB source_files
        "memory/*.cpp")


set(HEADER_FILES ${header_files} Wlib.h)

set(SOURCE_FILES ${source_files})

add_library(wlib STATIC ${SOURCE_FILES} ${HEADER_FILES})
