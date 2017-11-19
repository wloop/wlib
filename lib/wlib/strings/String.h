/**
 * @file Types.h
 * @brief type definition of all the string types we will be using
 *
 * @author Deep Dhillon
 * @date November 19, 2017
 * @bug No known bug
 */

#ifndef EMBEDDEDCPLUSPLUS_STRING_H
#define EMBEDDEDCPLUSPLUS_STRING_H

#include "strings/StaticString.h"

namespace wlp{
    // Static Strings
    typedef StaticString<8u> String8;
    typedef StaticString<16u> String16;
    typedef StaticString<32u> String32;
    typedef StaticString<64u> String64;
    typedef StaticString<128u> String128;
    typedef StaticString<256u> String256;

    /*
    // Dynamic String
    typedef wlp::DynamicString String;
    */
}

#endif //EMBEDDEDCPLUSPLUS_STRING_H
