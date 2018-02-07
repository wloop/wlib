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

#include "wlib/Types.h"

#include "wlib/strings/String.h"

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
     * Template specialization for static string.
     *
     * @tparam tSize static string size
     */
    template<uint16_t tSize>
    struct Equal<StaticString<tSize>> {
        bool operator()(const StaticString<tSize> &key1, const StaticString<tSize> &key2) const {
            return strcmp(key1.c_str(), key2.c_str()) == 0;
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
            return strcmp(key1.c_str(), key2.c_str()) == 0;
        }
    };

    /**
     * Template specialization for character arrays.
     */
    template<>
    struct Equal<char *> {
        bool operator()(const char *key1, const char *key2) const {
            return strcmp(key1, key2) == 0;
        }
    };

    /**
     * Template specialization for C strings.
     */
    template<>
    struct Equal<const char *> {
        bool operator()(const char *key1, const char *key2) const {
            return strcmp(key1, key2) == 0;
        }
    };

}

#endif //CORE_STL_EQUAL_H
