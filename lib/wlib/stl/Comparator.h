#ifndef EMBEDDEDCPLUSPLUS_COMPARATOR_H
#define EMBEDDEDCPLUSPLUS_COMPARATOR_H

#include <stdint.h>

#include "../WlibConfig.h"
#include "Equal.h"

namespace wlp {

    inline int8_t str_cmp(const char *s1, const char *s2) {
        for (; *s1 == *s2; ++s1, ++s2) {
            if (*s1 == 0) {
                return 0;
            }
        }
        return *s2 - *s1;
    }

    template<typename T>
    struct Comparator {
        bool __lt__(const T &t1, const T &t2) const {
            return t1 < t2;
        }
        bool __le__(const T &t1, const T &t2) const {
            return t1 <= t2;
        }
        bool __eq__(const T &t1, const T &t2) const {
            return t1 == t2;
        }
        bool __ne__(const T &t1, const T &t2) const {
            return t1 != t2;
        }
        bool __gt__(const T &t1, const T &t2) const {
            return t1 > t2;
        }
        bool __ge__(const T &t1, const T &t2) const {
            return t1 >= t2;
        }
    };

    template<typename T>
    struct ReverseComparator {
        bool __lt__(const T &t1, const T &t2) const {
            return t1 > t2;
        }
        bool __le__(const T &t1, const T &t2) const {
            return t1 >= t2;
        }
        bool __eq__(const T &t1, const T &t2) const {
            return t1 == t2;
        }
        bool __ne__(const T &t1, const T &t2) const {
            return t1 != t2;
        }
        bool __gt__(const T &t1, const T &t2) const {
            return t1 < t2;
        }
        bool __ge__(const T &t1, const T &t2) const {
            return t1 >= t2;
        }
    };

    template<>
    struct Comparator<char *> {
        bool __lt__(const char *s1, const char *s2) {
            return str_cmp(s1, s2) < 0;
        }
        bool __le__(const char *s1, const char *s2) {
            return str_cmp(s1, s2) <= 0;
        }
        bool __eq__(const char *s1, const char *s2) {
            return str_cmp(s1, s2) == 0;
        }
        bool __ne__(const char *s1, const char *s2) {
            return str_cmp(s1, s2) != 0;
        }
        bool __gt__(const char *s1, const char *s2) {
            return str_cmp(s1, s2) > 0;
        }
        bool __ge__(const char *s1, const char *s2) {
            return str_cmp(s1, s2) >= 0;
        }
    };

    template<>
    struct Comparator<const char *> {
        bool __lt__(const char *s1, const char *s2) {
            return str_cmp(s1, s2) < 0;
        }
        bool __le__(const char *s1, const char *s2) {
            return str_cmp(s1, s2) <= 0;
        }
        bool __eq__(const char *s1, const char *s2) {
            return str_cmp(s1, s2) == 0;
        }
        bool __ne__(const char *s1, const char *s2) {
            return str_cmp(s1, s2) != 0;
        }
        bool __gt__(const char *s1, const char *s2) {
            return str_cmp(s1, s2) > 0;
        }
        bool __ge__(const char *s1, const char *s2) {
            return str_cmp(s1, s2) >= 0;
        }
    };

    template<uint16_t tSize>
    struct Comparator<StaticString<tSize>> {
        bool __lt__(const StaticString<tSize> &s1, const StaticString<tSize> &s2) {
            return str_cmp(s1.c_str(), s2.c_str()) < 0;
        }
        bool __le__(const StaticString<tSize> &s1, const StaticString<tSize> &s2) {
            return str_cmp(s1.c_str(), s2.c_str()) <= 0;
        }
        bool __eq__(const StaticString<tSize> &s1, const StaticString<tSize> &s2) {
            return str_cmp(s1.c_str(), s2.c_str()) == 0;
        }
        bool __ne__(const StaticString<tSize> &s1, const StaticString<tSize> &s2) {
            return str_cmp(s1.c_str(), s2.c_str()) != 0;
        }
        bool __gt__(const StaticString<tSize> &s1, const StaticString<tSize> &s2) {
            return str_cmp(s1.c_str(), s2.c_str()) > 0;
        }
        bool __ge__(const StaticString<tSize> &s1, const StaticString<tSize> &s2) {
            return str_cmp(s1.c_str(), s2.c_str()) >= 0;
        }
    };

    template<uint16_t tSize>
    struct Comparator<const StaticString<tSize>> {
        bool __lt__(const StaticString<tSize> &s1, const StaticString<tSize> &s2) {
            return str_cmp(s1.c_str(), s2.c_str()) < 0;
        }
        bool __le__(const StaticString<tSize> &s1, const StaticString<tSize> &s2) {
            return str_cmp(s1.c_str(), s2.c_str()) <= 0;
        }
        bool __eq__(const StaticString<tSize> &s1, const StaticString<tSize> &s2) {
            return str_cmp(s1.c_str(), s2.c_str()) == 0;
        }
        bool __ne__(const StaticString<tSize> &s1, const StaticString<tSize> &s2) {
            return str_cmp(s1.c_str(), s2.c_str()) != 0;
        }
        bool __gt__(const StaticString<tSize> &s1, const StaticString<tSize> &s2) {
            return str_cmp(s1.c_str(), s2.c_str()) > 0;
        }
        bool __ge__(const StaticString<tSize> &s1, const StaticString<tSize> &s2) {
            return str_cmp(s1.c_str(), s2.c_str()) >= 0;
        }
    };

}

#endif //EMBEDDEDCPLUSPLUS_COMPARATOR_H
