/**
 * @file Comparator.h
 * @brief Comparator implementation for basic types.
 *
 * @author Jeff Niu
 * @date November 9, 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDCPLUSPLUS_COMPARATOR_H
#define EMBEDDEDCPLUSPLUS_COMPARATOR_H

#include <string.h> // strcmp

#include "wlib/Types.h"

#include "Equal.h"
#include "wlib/strings/String.h"

namespace wlp {

    /**
     * Base comparator type uses the basic comparison
     * operators.
     *
     * @tparam T compared type
     */
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

    /**
     * Reverse comparator flips the semantics for
     * greater than and less than comparisons.
     *
     * @tparam T compared type
     */
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
            return t1 <= t2;
        }
    };

    /**
     * Template specialization for C strings.
     */
    template<>
    struct Comparator<const char *> {
        bool __lt__(const char *s1, const char *s2) const {
            return strcmp(s1, s2) < 0;
        }

        bool __le__(const char *s1, const char *s2) const {
            return strcmp(s1, s2) <= 0;
        }

        bool __eq__(const char *s1, const char *s2) const {
            return strcmp(s1, s2) == 0;
        }

        bool __ne__(const char *s1, const char *s2) const {
            return strcmp(s1, s2) != 0;
        }

        bool __gt__(const char *s1, const char *s2) const {
            return strcmp(s1, s2) > 0;
        }

        bool __ge__(const char *s1, const char *s2) const {
            return strcmp(s1, s2) >= 0;
        }
    };

    /**
     * Template specialization for static strings.
     *
     * @tparam tSize static string size
     */
    template<uint16_t tSize>
    struct Comparator<StaticString <tSize>> {
        bool __lt__(const StaticString <tSize> &s1, const StaticString <tSize> &s2) const {
            return strcmp(s1.c_str(), s2.c_str()) < 0;
        }

        bool __le__(const StaticString <tSize> &s1, const StaticString <tSize> &s2) const {
            return strcmp(s1.c_str(), s2.c_str()) <= 0;
        }

        bool __eq__(const StaticString <tSize> &s1, const StaticString <tSize> &s2) const {
            return strcmp(s1.c_str(), s2.c_str()) == 0;
        }

        bool __ne__(const StaticString <tSize> &s1, const StaticString <tSize> &s2) const {
            return strcmp(s1.c_str(), s2.c_str()) != 0;
        }

        bool __gt__(const StaticString <tSize> &s1, const StaticString <tSize> &s2) const {
            return strcmp(s1.c_str(), s2.c_str()) > 0;
        }

        bool __ge__(const StaticString <tSize> &s1, const StaticString <tSize> &s2) const {
            return strcmp(s1.c_str(), s2.c_str()) >= 0;
        }
    };

}

#endif //EMBEDDEDCPLUSPLUS_COMPARATOR_H
