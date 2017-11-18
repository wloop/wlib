/**
 * @file Types.h
 * @brief type definition of all the types we will be using
 *
 * @author Deep Dhillon
 * @date November 12, 2017
 * @bug No known bug
 */


#ifndef EMBEDDEDCPLUSPLUS_TYPES_H
#define EMBEDDEDCPLUSPLUS_TYPES_H

#include <stdint.h>
#include "strings/StaticString.h"

namespace wlp {

    // size
    typedef uint16_t size_type;

    // Static Strings
    typedef StaticString<8> String8;
    typedef StaticString<16> String16;
    typedef StaticString<32> String32;
    typedef StaticString<64> String64;
    typedef StaticString<128> String128;
    typedef StaticString<256> String256;

    /*
    // Dynamic String
    typedef wlp::DynamicString String;
    */

}

#endif //EMBEDDEDCPLUSPLUS_TYPES_H
