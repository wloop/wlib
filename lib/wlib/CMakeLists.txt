project(wlib)

SET(CMAKE_CXX_FLAGS_DISTRIBUTION "-fno-exceptions -fno-threadsafe-statics -fpermissive -std=gnu++11 -g -Os -Wall -ffunction-sections -fdata-sections -flto -mmcu=atmega2560 -DF_CPU=16000000L -DPLATFORMIO=30401 -DARDUINO_ARCH_AVR -DARDUINO_AVR_MEGA2560 -DARDUINO=10619")
SET(CMAKE_C_FLAGS_DISTRIBUTION "-std=gnu11 -fno-fat-lto-objects -g -Os -Wall -ffunction-sections -fdata-sections -flto -mmcu=atmega2560 -DF_CPU=16000000L -DPLATFORMIO=30401 -DARDUINO_ARCH_AVR -DARDUINO_AVR_MEGA2560 -DARDUINO=10619")
set(CMAKE_CXX_STANDARD 11)

file (GLOB header_files
        "Wlib.h"
        "strings/*.h"
        "stl/*.h"
        "memory/*.h")

file (GLOB source_files
        "memory/*.cpp")

set(HEADER_FILES ${header_files} )
set(SOURCE_FILES ${source_files})

add_library(wlib STATIC ${SOURCE_FILES} ${HEADER_FILES})
