/**
 * @file hash.h
 * @brief Provides hash functions for basic data types.
 *
 * @author Jeff Niu
 * @date November 1, 2017
 * @bug No known bugs
 */

#ifndef CORE_STL_HASH_H
#define CORE_STL_HASH_H

#include <stdint.h>

#include "WlibConfig.h"
#include "WlibMacro.h"
#include "strings/StaticString.h"

template <class Key, class IntType> struct hash {};

template<class IntType, uint16_t tSize>
inline IntType hash_static_string(StaticString<tSize>& static_string) {
    IntType h = 0;
    for (uint16_t pos = 0; pos < static_string.length(); ++pos) {
        h = MUL_127(h) + static_string[pos];
    }
    return h;
};

template<class IntType>
inline IntType hash_string(const char* s) {
    IntType h = 0;
    for (; *s; ++s) {
        h = MUL_127(h) + *s;
    }
    return h;
}

template<class IntType, uint16_t tSize>
struct hash<StaticString<tSize>, IntType> {
    IntType operator()(StaticString<tSize>& s) const {
        return hash_static_string<IntType, tSize>(s);
    }
};

template<class IntType> struct hash<char*, IntType> {
    IntType operator()(const char* s) const {
        return hash_string<IntType>(s);
    }
};

template<class IntType> struct hash<const char*, IntType> {
    IntType operator()(const char* s) const  {
        return hash_string<IntType>(s);
    }
};

template<class IntType> struct hash<char, IntType> {
    IntType operator()(char x) const {
        return x;
    }
};

template<class IntType> struct hash<unsigned char, IntType> {
    IntType operator()(unsigned char x) const {
        return x;
    }
};

template<class IntType> struct hash<signed char, IntType> {
    IntType operator()(signed char x) const {
        return x;
    }
};

template<class IntType> struct hash<uint8_t, IntType> {
    IntType operator()(uint8_t x) const {
        return x;
    }
};

template<class IntType> struct hash<uint16_t, IntType> {
    IntType operator()(uint16_t x) const {
        return x;
    }
};

template<class IntType> struct hash<uint32_t, IntType> {
    IntType operator()(uint16_t x) const {
        return x;
    }
};

template<class IntType> struct hash<int8_t, IntType> {
    IntType operator()(int8_t x) const {
        return x;
    }
};

template<class IntType> struct hash<int16_t, IntType> {
    IntType operator()(int16_t x) const {
        return x;
    }
};

template<class IntType> struct hash<int32_t, IntType> {
    IntType operator()(int32_t x) const {
        return x;
    }
};

#endif //CORE_STL_HASH_H
