/**
 * @file WlibMacro.h
 * @brief Basic macros that may be useful.
 *
 * @author Jeff Niu
 * @date November 1, 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDTESTS_WLIBMACRO_H
#define EMBEDDEDTESTS_WLIBMACRO_H

// Multiplication by Mersenne primes reduced as bit operations
// for compilers that do not already perform this optimization
#define MUL_31(x)  (((x) << 5) - (x))
#define MUL_127(x) (((x) << 7) - (x))

#endif //EMBEDDEDTESTS_WLIBMACRO_H
