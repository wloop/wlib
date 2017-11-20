#ifndef EMBEDDEDCPLUSPLUS_MATH_H
#define EMBEDDEDCPLUSPLUS_MATH_H

namespace wlp {

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
