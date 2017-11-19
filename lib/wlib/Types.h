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

    /**
     * Standard size type for Wlib. Used as a 16-bit unsigned integer.
     * Represents a discrete measure of amount.
     */
    typedef uint16_t size_type;
    /**
     * Standard different type for Wlib. Used as a 16-bit signed integer.
     * Represents a discrete relative difference which has a concept
     * of direction.
     */
    typedef int16_t diff_type;
    /**
     * Type used to represent a state as a distinct numerical value.
     * Uses the unsigned 8-bit integer.
     */
    typedef uint8_t state_type;

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
