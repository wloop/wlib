# CMake generated Testfile for 
# Source directory: /mnt/c/Users/bobqy/Desktop/Waterloop/embedded-cplusplus
# Build directory: /mnt/c/Users/bobqy/Desktop/Waterloop/embedded-cplusplus
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(EmbeddedCplusplusTests "/mnt/c/Users/bobqy/Desktop/Waterloop/embedded-cplusplus/tests/tests")
subdirs(include/gtest-1.8.0)
subdirs(lib/wlib)
subdirs(examples)
subdirs(tests)
