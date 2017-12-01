[![Build Status](https://travis-ci.org/teamwaterloop/embedded-cplusplus.svg?branch=master)](https://travis-ci.org/teamwaterloop/embedded-cplusplus)
[![Coverage Status](https://coveralls.io/repos/github/teamwaterloop/embedded-cplusplus/badge.svg?branch=master)](https://coveralls.io/github/teamwaterloop/embedded-cplusplus?branch=master)

# Waterloop Embedded C++

C++ version of libraries used by Waterloop for embedded systems. The only library created is **Wlib**. Wlib is a library built in-house for our pod embedded systems. The main goal of this project is to create a lighter and a safer alternative to the STL library provided in C++. There were many reasons why we did not want to go with STL and other embedded STL alternatives out there. One of the biggest reason was that all of these libraries use heap extensively but we wanted to use controlled heap where there is a pool of memory allocated upfront. This library achieves that and there is no dependecy with any of standard C++ library but standard C libraries are being used in the project. This library is optimized to be used on embedded systems that have less memory and low processing power.

For detail documentation check our [documentation site](https://teamwaterloop.github.io/embedded-cplusplus/)
