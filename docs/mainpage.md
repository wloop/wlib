## Overview
Waterloop is a team competiting at SpaceX Hyperloop competition. We are based on in Waterloo and our goal is to innovate the field of transportation and make people go from one place to another in a fraction of time it takes at the moment. **Wlib** is a library built in-house for our pod embedded systems. The main goal of this project is to create a lighter and a safer alternative to the STL library provided in C++. There were many reasons why we did not want to go with STL and other embedded STL alternatives out there. One of the biggest reason was that all of these libraries use heap extensively but we wanted to use controlled heap where there is a pool of memory allocated upfront. This library achieves that and there is no dependecy with any of standard C++ library but standard C libraries are being used in the project. This library is optimized to be used on embedded systems that have less memory and low processing power.

## Why Wlib?

- Wlib is safe from memory fragmentation because it uses fixed sized memory pool
- Wlib has a very little overhead because a lot of functions are optimized to run at compile time
- Wlib is very lightweight compared to STL and this is a huge factor for embedded systems with little memory
- Wlib provides all the necessary data structures and features like **smart_ptrs** from STL that one would need for embedded systems

Check our documentation for any questions you may have and if there is any issue or a bug you find, please create an issue on our [github](https://github.com/teamwaterloop/embedded-cplusplus) and we will look into resolving that. This library is complete for our needs and hence not alot of work will be done on it.