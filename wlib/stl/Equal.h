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

#include <string.h> // strcmp

#include <wlib/strings/String.h>

namespace wlp {

    /**
     * The equality function type returns whether
     * two key-type values are to be considered equal.
     * @tparam Key key type
     */
    template<class Key>
    struct equals {
        bool operator()(const Key &key1, const Key &key2) const {
            return key1 == key2;
        }
    };

    /**
     * Template specialization for static string.
     *
     * @tparam tSize static string size
     */
    template<uint16_t tSize>
    struct equals<static_string<tSize>> {
        bool operator()(const static_string<tSize> &key1, const static_string<tSize> &key2) const {
            return strcmp(key1.c_str(), key2.c_str()) == 0;
        }
    };

    /**
     * Template specialization for const static string.
     *
     * @tparam tSize static string size
     */
    template<uint16_t tSize>
    struct equals<const static_string<tSize>> {
        bool operator()(const static_string<tSize> &key1, const static_string<tSize> &key2) const {
            return strcmp(key1.c_str(), key2.c_str()) == 0;
        }
    };

    template<>
    struct equals<dynamic_string> {
        bool operator()(const dynamic_string &str1, const dynamic_string &str2) const {
            return strcmp(str1.c_str(), str2.c_str()) == 0;
        }
    };

    template<>
    struct equals<const dynamic_string> {
        bool operator()(const dynamic_string &str1, const dynamic_string &str2) const {
            return strcmp(str1.c_str(), str2.c_str()) == 0;
        }
    };

    /**
     * Template specialization for character arrays.
     */
    template<>
    struct equals<char *> {
        bool operator()(const char *key1, const char *key2) const {
            return strcmp(key1, key2) == 0;
        }
    };

    /**
     * Template specialization for C strings.
     */
    template<>
    struct equals<const char *> {
        bool operator()(const char *key1, const char *key2) const {
            return strcmp(key1, key2) == 0;
        }
    };

}

#endif //CORE_STL_EQUAL_H
