[![Build Status](https://travis-ci.org/teamwaterloop/embedded-cplusplus.svg?branch=master)](https://travis-ci.org/teamwaterloop/embedded-cplusplus)
[![Coverage Status](https://coveralls.io/repos/github/teamwaterloop/embedded-cplusplus/badge.svg?branch=master)](https://coveralls.io/github/teamwaterloop/embedded-cplusplus?branch=master)
# Waterloop Embedded C++ Wlib
Wlib is a C++ library used by team Waterloop to program embedded systems. This library is created to add missing STL features that we need to program our embedded systems. This is not a complete clone of STL but it contains a lot of data structures and template helpers to make porgramming safer and easier.

## Why Wlib?
STL is solid and there are many embedded STL clones available to be used so why make Wlib? There are many reasons but the all fall under safety and customization. Wlib is created for these reasons:

- Custom fixed size memory allocations
- Custom memory used by every feature in the library
- String class that uses Stack
- Light weight because it only contains what we need
- Highly documented
- Compile time checks using templates and concepts
- Compatible with Arduino hardware