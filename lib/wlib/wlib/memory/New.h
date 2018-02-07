/**
 * @file New.h
 * @brief New operators defined for Arduino boards and for other boards use the normal new
 *
 * Some versions of Arduino provide a @code new.h @endcode but these do not include
 * a definition for placement new. In debug mode, for Wlib develop, the standard C++
 * @code <new> @endcode is included, and for all other cases, these operators fall
 * back on the C allocation functions.
 *
 * COSA_H is defined when the project is built with Cosa instead of Arduino SDK. Since Cosa
 * does not provide a @code new.h @endcode custom definitions are given in that case as well.
 *
 * @author Deep Dhillon
 * @author Jeff Niu
 * @date December 4, 2017
 * @bug No known bugs.
 */

#ifndef EMBEDDEDCPLUSPLUS_NEW_H
#define EMBEDDEDCPLUSPLUS_NEW_H
#if (defined(__AVR_ATmega1280__)  || \
     defined(__AVR_ATmega2560__)  || \
     defined(__AVR_ATmega1284__)  || \
     defined(__AVR_ATmega1284P__) || \
     defined(__AVR_ATmega644__)   || \
     defined(__AVR_ATmega644A__)  || \
     defined(__AVR_ATmega644P__)  || \
     defined(__AVR_ATmega644PA__)) && !defined(__AVR_Cosa__)

#include <new.h>

void* operator new(decltype(sizeof(0)), void* ptr) noexcept;

#elif defined(WLIB_DEBUG) && !defined(__AVR_Cosa__)

#include <new>

#else

void *operator new(decltype(sizeof(0)) n) noexcept(false);

void *operator new(decltype(sizeof(0)), void *ptr) noexcept;

void operator delete(void *ptr) throw();

void *operator new[](decltype(sizeof(0)) n) noexcept(false);

void operator delete[](void *ptr) throw();

#endif
#endif //EMBEDDEDCPLUSPLUS_NEW_H
