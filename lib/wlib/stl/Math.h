#ifndef EMBEDDEDCPLUSPLUS_MATH_H
#define EMBEDDEDCPLUSPLUS_MATH_H

namespace wlp {

    template<typename int_type>
    inline constexpr int_type log2(int_type n) {
        return static_cast<int_type>((n < 2) ? 1 : 1 + log2(static_cast<int_type>(n / 2)));
    }

    template<typename int_type>
    inline constexpr int_type pow(const int_type a, const int_type b) {
        return (b == 0) ? 1 : (a * pow(a, b - 1));
    }

}

#endif //EMBEDDEDCPLUSPLUS_MATH_H
