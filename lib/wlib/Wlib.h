#ifndef EMBEDDEDTESTS_WLIB_H
#define EMBEDDEDTESTS_WLIB_H

// Constants for standard sizes
#define BYTE_SIZE 8
#define INT32_SIZE (BYTE_SIZE * sizeof(uint32_t))

// Variadic macro argument helpers
#define __NARG__(...) __NARG_I_(__VA_ARGS__,__RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define __RSEQ_N() 8, 7, 6, 5, 4, 3, 2, 1, 0
#define _VFUNC_(name, n) name##n
#define _VFUNC(name, n) _VFUNC_(name, n)
#define VFUNC(func, ...) _VFUNC(func, __NARG__(__VA_ARGS__)) (__VA_ARGS__)

#endif //EMBEDDEDTESTS_WLIB_H
