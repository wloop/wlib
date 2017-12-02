#ifndef EMBEDDEDCPLUSPLUS_MATH_H
#define EMBEDDEDCPLUSPLUS_MATH_H

namespace wlp {

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define ABS(x) ((x) < 0 ? -(x) : (x))

// Multiplication by Mersenne primes reduced as bit operations
// for compilers that do not already perform this optimization
#define MUL_31(x)  (((x) << 5) - (x))
#define MUL_127(x) (((x) << 7) - (x))

    template<typename int_type>
    inline constexpr int_type log2_const(int_type n) {
        return ((n < 2) ? 0 : 1 + log2_const(n / 2));
    }

    template<typename int_type>
    inline constexpr int_type pow_const(int_type a, int_type b) {
        return (b == 0) ? 1 : (a * pow_const<int_type>(a, b - 1));
    }

    template<typename int_type>
    inline int_type int_div_round(int_type n, int_type d) {
        return ((n < 0) ^ (d < 0)) ? ((n - d / 2) / d) : ((n + d / 2) / d);
    }
}

#endif //EMBEDDEDCPLUSPLUS_MATH_H
