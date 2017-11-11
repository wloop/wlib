#ifndef EMBEDDEDTESTS_WLIB_H
#define EMBEDDEDTESTS_WLIB_H

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

// Multiplication by Mersenne primes reduced as bit operations
// for compilers that do not already perform this optimization
#define MUL_31(x)  (((x) << 5) - (x))
#define MUL_127(x) (((x) << 7) - (x))

// Constants for standard sizes
#define BYTE_SIZE 8
#define INT_SIZE (BYTE_SIZE * sizeof(uint32_t))

// Variadic macro argument helpers
#define __NARG__(...) __NARG_I_(__VA_ARGS__,__RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define __RSEQ_N() 8, 7, 6, 5, 4, 3, 2, 1, 0
#define _VFUNC_(name, n) name##n
#define _VFUNC(name, n) _VFUNC_(name, n)
#define VFUNC(func, ...) _VFUNC(func, __NARG__(__VA_ARGS__)) (__VA_ARGS__)

namespace wlp {

    /**
     * Standard size_type for Wlib. Unsigned 16-bit integer
     * is preferred over @code std::size_t @endcode which is
     * of the unsigned long variant.
     */
    typedef uint16_t size_type;

}

#endif //EMBEDDEDTESTS_WLIB_H
