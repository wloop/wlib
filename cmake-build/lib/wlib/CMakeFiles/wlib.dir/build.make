# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /cygdrive/c/Users/ds3dhill/.CLion2017.2/system/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/ds3dhill/.CLion2017.2/system/cygwin_cmake/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/cmake-build"

# Include any dependencies generated for this target.
include lib/wlib/CMakeFiles/wlib.dir/depend.make

# Include the progress variables for this target.
include lib/wlib/CMakeFiles/wlib.dir/progress.make

# Include the compile flags for this target's objects.
include lib/wlib/CMakeFiles/wlib.dir/flags.make

lib/wlib/CMakeFiles/wlib.dir/memory/Allocator.cpp.o: lib/wlib/CMakeFiles/wlib.dir/flags.make
lib/wlib/CMakeFiles/wlib.dir/memory/Allocator.cpp.o: ../lib/wlib/memory/Allocator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/cmake-build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/wlib/CMakeFiles/wlib.dir/memory/Allocator.cpp.o"
	cd "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/cmake-build/lib/wlib" && /usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wlib.dir/memory/Allocator.cpp.o -c "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/lib/wlib/memory/Allocator.cpp"

lib/wlib/CMakeFiles/wlib.dir/memory/Allocator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wlib.dir/memory/Allocator.cpp.i"
	cd "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/cmake-build/lib/wlib" && /usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/lib/wlib/memory/Allocator.cpp" > CMakeFiles/wlib.dir/memory/Allocator.cpp.i

lib/wlib/CMakeFiles/wlib.dir/memory/Allocator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wlib.dir/memory/Allocator.cpp.s"
	cd "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/cmake-build/lib/wlib" && /usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/lib/wlib/memory/Allocator.cpp" -o CMakeFiles/wlib.dir/memory/Allocator.cpp.s

lib/wlib/CMakeFiles/wlib.dir/memory/Allocator.cpp.o.requires:

.PHONY : lib/wlib/CMakeFiles/wlib.dir/memory/Allocator.cpp.o.requires

lib/wlib/CMakeFiles/wlib.dir/memory/Allocator.cpp.o.provides: lib/wlib/CMakeFiles/wlib.dir/memory/Allocator.cpp.o.requires
	$(MAKE) -f lib/wlib/CMakeFiles/wlib.dir/build.make lib/wlib/CMakeFiles/wlib.dir/memory/Allocator.cpp.o.provides.build
.PHONY : lib/wlib/CMakeFiles/wlib.dir/memory/Allocator.cpp.o.provides

lib/wlib/CMakeFiles/wlib.dir/memory/Allocator.cpp.o.provides.build: lib/wlib/CMakeFiles/wlib.dir/memory/Allocator.cpp.o


lib/wlib/CMakeFiles/wlib.dir/memory/Memory.cpp.o: lib/wlib/CMakeFiles/wlib.dir/flags.make
lib/wlib/CMakeFiles/wlib.dir/memory/Memory.cpp.o: ../lib/wlib/memory/Memory.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/cmake-build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object lib/wlib/CMakeFiles/wlib.dir/memory/Memory.cpp.o"
	cd "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/cmake-build/lib/wlib" && /usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wlib.dir/memory/Memory.cpp.o -c "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/lib/wlib/memory/Memory.cpp"

lib/wlib/CMakeFiles/wlib.dir/memory/Memory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wlib.dir/memory/Memory.cpp.i"
	cd "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/cmake-build/lib/wlib" && /usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/lib/wlib/memory/Memory.cpp" > CMakeFiles/wlib.dir/memory/Memory.cpp.i

lib/wlib/CMakeFiles/wlib.dir/memory/Memory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wlib.dir/memory/Memory.cpp.s"
	cd "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/cmake-build/lib/wlib" && /usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/lib/wlib/memory/Memory.cpp" -o CMakeFiles/wlib.dir/memory/Memory.cpp.s

lib/wlib/CMakeFiles/wlib.dir/memory/Memory.cpp.o.requires:

.PHONY : lib/wlib/CMakeFiles/wlib.dir/memory/Memory.cpp.o.requires

lib/wlib/CMakeFiles/wlib.dir/memory/Memory.cpp.o.provides: lib/wlib/CMakeFiles/wlib.dir/memory/Memory.cpp.o.requires
	$(MAKE) -f lib/wlib/CMakeFiles/wlib.dir/build.make lib/wlib/CMakeFiles/wlib.dir/memory/Memory.cpp.o.provides.build
.PHONY : lib/wlib/CMakeFiles/wlib.dir/memory/Memory.cpp.o.provides

lib/wlib/CMakeFiles/wlib.dir/memory/Memory.cpp.o.provides.build: lib/wlib/CMakeFiles/wlib.dir/memory/Memory.cpp.o


# Object files for target wlib
wlib_OBJECTS = \
"CMakeFiles/wlib.dir/memory/Allocator.cpp.o" \
"CMakeFiles/wlib.dir/memory/Memory.cpp.o"

# External object files for target wlib
wlib_EXTERNAL_OBJECTS =

lib/wlib/libwlib.a: lib/wlib/CMakeFiles/wlib.dir/memory/Allocator.cpp.o
lib/wlib/libwlib.a: lib/wlib/CMakeFiles/wlib.dir/memory/Memory.cpp.o
lib/wlib/libwlib.a: lib/wlib/CMakeFiles/wlib.dir/build.make
lib/wlib/libwlib.a: lib/wlib/CMakeFiles/wlib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/cmake-build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libwlib.a"
	cd "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/cmake-build/lib/wlib" && $(CMAKE_COMMAND) -P CMakeFiles/wlib.dir/cmake_clean_target.cmake
	cd "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/cmake-build/lib/wlib" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wlib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/wlib/CMakeFiles/wlib.dir/build: lib/wlib/libwlib.a

.PHONY : lib/wlib/CMakeFiles/wlib.dir/build

lib/wlib/CMakeFiles/wlib.dir/requires: lib/wlib/CMakeFiles/wlib.dir/memory/Allocator.cpp.o.requires
lib/wlib/CMakeFiles/wlib.dir/requires: lib/wlib/CMakeFiles/wlib.dir/memory/Memory.cpp.o.requires

.PHONY : lib/wlib/CMakeFiles/wlib.dir/requires

lib/wlib/CMakeFiles/wlib.dir/clean:
	cd "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/cmake-build/lib/wlib" && $(CMAKE_COMMAND) -P CMakeFiles/wlib.dir/cmake_clean.cmake
.PHONY : lib/wlib/CMakeFiles/wlib.dir/clean

lib/wlib/CMakeFiles/wlib.dir/depend:
	cd "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/cmake-build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus" "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/lib/wlib" "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/cmake-build" "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/cmake-build/lib/wlib" "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/cmake-build/lib/wlib/CMakeFiles/wlib.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : lib/wlib/CMakeFiles/wlib.dir/depend

