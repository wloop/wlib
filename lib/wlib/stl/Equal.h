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

#include "../Types.h"

#include "../strings/StaticString.h"

namespace wlp {

    /**
     * The equality function type returns whether
     * two key-type values are to be considered equal.
     * @tparam Key key type
     */
    template<class Key>
    struct Equal {
        bool operator()(const Key &key1, const Key &key2) const {
            return key1 == key2;
        }
    };

    /**
     * Checks whether two static strings are equal.
     *
     * @tparam tSize static string size
     * @param str1 first string
     * @param str2 second string
     * @return true if the strings are equal
     */
    template<uint16_t tSize>
    inline bool static_string_equals(const StaticString<tSize> &str1, const StaticString<tSize> &str2) {
        if (str1.length() != str2.length()) {
            return false;
        }
        for (size_type i = 0; i < str1.length(); ++i) {
            if (str1[i] != str2[i]) {
                return false;
            }
        }
        return true;
    }

    /**
     * Checks whether two C strings are equal.
     *
     * @param str1 first string
     * @param str2 second string
     * @return true if the strings are equal
     */
    inline bool string_equals(const char *&str1, const char *&str2) {
        for (; *str1 && *str2; ++str1, ++str2) {
            if (*str1 != *str2) {
                return false;
            }
        }
        return *str1 == *str2;
    }

    /**
     * Template specialization for static string.
     *
     * @tparam tSize static string size
     */
    template<uint16_t tSize>
    struct Equal<StaticString<tSize>> {
        bool operator()(const StaticString<tSize> &key1, const StaticString<tSize> &key2) const {
            return static_string_equals(key1, key2);
        }
    };

    /**
     * Template specialization for const static string.
     *
     * @tparam tSize static string size
     */
    template<uint16_t tSize>
    struct Equal<const StaticString<tSize>> {
        bool operator()(const StaticString<tSize> &key1, const StaticString<tSize> &key2) const {
            return static_string_equals(key1, key2);
        }
    };

    /**
     * Template specialization for character arrays.
     */
    template<>
    struct Equal<char *> {
        bool operator()(const char *key1, const char *key2) const {
            return string_equals(key1, key2);
        }
    };

    /**
     * Template specialization for C strings.
     */
    template<>
    struct Equal<const char *> {
        bool operator()(const char *key1, const char *key2) const {
            return string_equals(key1, key2);
        }
    };

}

#endif //CORE_STL_EQUAL_H
