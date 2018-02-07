/**
 * @file hash.h
 * @brief Provides hash functions for basic data types.
 *
 * @author Jeff Niu
 * @date November 1, 2017
 * @bug No known bugs
 */

/*
 * TODO: implementation of uniform hash functions
 */

#ifndef CORE_STL_HASH_H
#define CORE_STL_HASH_H

#include "wlib/Types.h"
#include "wlib/WlibConfig.h"
#include "wlib/utility/Math.h"
#include "wlib/strings/String.h"

namespace wlp {

    /**
     * A basic hash function is defined to be a function that, as best
     * as reasonably possible, maps a key-type value to a unique positive integer
     * value.
     *
     * @pre The default hash function attempts to cast to the integer type,
     *      which works for integer, character, and floating point types.
     *
     * @tparam Key     key type
     * @tparam IntType the unsigned integer type to return
     */
    template<class Key, class IntType>
    struct Hash {
        IntType operator()(const Key &key) const {
            return (IntType) key;
        }
    };

    /**
     * Hash a static string by multiplying the character value
     * by 127 and adding consecutively.
     *
     * @tparam IntType the integer return type
     * @tparam tSize the static string size
     * @param static_string the string to hash
     * @return a hash code of the string
     */
    template<class IntType, uint16_t tSize>
    inline IntType hash_static_string(const StaticString<tSize> &static_string) {
        IntType h = 0;
        for (size_type pos = 0; pos < static_string.length(); ++pos) {
            h = (IntType) (MUL_127(h) + static_string[pos]);
        }
        return h;
    };

    /**
     * Hash a C string by multiplying the character value
     * by 127 and adding consecutively.
     *
     * @tparam IntType the integer return type value
     * @param s the string to hash
     * @return a hash code of the string
     */
    template<class IntType>
    inline IntType hash_string(const char *s) {
        IntType h = 0;
        for (; *s; ++s) {
            h = (IntType) (MUL_127(h) + *s);
        }
        return h;
    }

    /**
     * Template specialization for static stirng.
     *
     * @tparam IntType hash code integer type
     * @tparam tSize static string size
     */
    template<class IntType, uint16_t tSize>
    struct Hash<StaticString<tSize>, IntType> {
        IntType operator()(const StaticString<tSize> &s) const {
            return hash_static_string<IntType, tSize>(s);
        }
    };

    /**
     * Template specialization for C strings.
     *
     * @tparam IntType hash code integer type
     */
    template<class IntType>
    struct Hash<char *, IntType> {
        IntType operator()(const char *s) const {
            return hash_string<IntType>(s);
        }
    };

    /**
     * Template specialization for C strings.
     *
     * @tparam IntType hash code integer type
     */
    template<class IntType>
    struct Hash<const char *, IntType> {
        IntType operator()(const char *s) const {
            return hash_string<IntType>(s);
        }
    };

}

#endif //CORE_STL_HASH_H
