#ifndef EMBEDDEDCPLUSPLUS_MATH_H
#define EMBEDDEDCPLUSPLUS_MATH_H

namespace wlp {

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

// Multiplication by Mersenne primes reduced as bit operations
// for compilers that do not already perform this optimization
#define MUL_31(x)  (((x) << 5) - (x))
#define MUL_127(x) (((x) << 7) - (x))

    template<typename int_type>
    inline constexpr int_type log2_const(int_type n) {
        return ((n < 2) ? 0 : 1 + log2_const(n / 2));
    }

    template<typename int_type>
    inline constexpr int_type pow_const(const int_type a, const int_type b) {
        return (b == 0) ? 1 : (a * pow_const<int_type>(a, b - 1));
    }
}

#endif //EMBEDDEDCPLUSPLUS_MATH_H
