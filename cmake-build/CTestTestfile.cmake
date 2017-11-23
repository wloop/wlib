# CMake generated Testfile for 
# Source directory: /cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus
# Build directory: /cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/cmake-build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(EmbeddedCplusplusTests "/cygdrive/d/Code/Waterloop/Goose 3/embedded-cplusplus/cmake-build/tests/tests.exe")
subdirs("include/gtest-1.8.0")
subdirs("lib/wlib")
subdirs("examples")
subdirs("tests")
