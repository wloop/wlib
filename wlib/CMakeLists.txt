project(wlib)

file (GLOB header_files
        "strings/*.h"
        "stl/*.h"
        "memory/*.h")

file (GLOB source_files
        "memory/*.cpp")


set(HEADER_FILES ${header_files} Wlib.h)

set(SOURCE_FILES ${source_files} stl/Hash.h stl/Map.h stl/Equal.h WlibConfig.h WlibMacro.h stl/Pair.h exception/Exception.h)

add_library(wlib STATIC ${SOURCE_FILES} ${HEADER_FILES})
