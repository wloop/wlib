/**
 * @file Map.h
 * @brief Tests for equality for basic data types.
 *
 * @author Jeff Niu
 * @date November 1, 2017
 * @bug No known bugs
 */

#ifndef CORE_STL_EQUAL_H
#define CORE_STL_EQUAL_H

#include <stdint.h>

#include "WlibConfig.h"
#include "strings/StaticString.h"

namespace wlp {

    /**
     * The equality function type returns whether
     * two key-type values are to be considered equal.
     * @tparam Key key type
     */
    template<class Key>
    struct equals {
        bool operator()(const Key &key1, const Key &key2) {
            return key1 == key2;
        }
    };

    template<uint16_t tSize>
    inline bool static_string_equals(StaticString<tSize> &str1, StaticString<tSize> &str2) {
        if (str1.length() != str2.length()) {
            return false;
        }
        for (uint16_t i = 0; i < str1.length(); ++i) {
            if (str1[i] != str2[i]) {
                return false;
            }
        }
        return true;
    }

    inline bool string_equals(const char *&str1, const char *&str2) {
        for (; *str1 && *str2; ++str1, ++str2) {
            if (*str1 != *str2) {
                return false;
            }
        }
        return *str1 == *str2;
    }

    template<uint16_t tSize>
    struct equals<StaticString<tSize> > {
        bool operator()(StaticString<tSize> &key1, StaticString<tSize> &key2) {
            return static_string_equals(key1, key2);
        }
    };

    TEMPLATE_NULL struct equals<char *> {
        bool operator()(const char *key1, const char *key2) {
            return string_equals(key1, key2);
        }
    };

    TEMPLATE_NULL struct equals<const char *> {
        bool operator()(const char *key1, const char *key2) {
            return string_equals(key1, key2);
        }
    };

}

#endif //CORE_STL_EQUAL_H
