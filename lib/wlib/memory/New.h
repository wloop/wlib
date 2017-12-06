/**
 * @file New.h
 * @brief New operators defined for Arduino boards and for other boards use the normal new
 *
 * @author Deep Dhillon
 * @date December 4, 2017
 * @bug No known bugs.
 */

#ifndef EMBEDDEDCPLUSPLUS_NEW_H
#define EMBEDDEDCPLUSPLUS_NEW_H

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
#include <stdlib.h>

void *operator new(decltype(sizeof(0)) n) noexcept(false) {
    void *memory = malloc(n);
    return memory;
}

void* operator new(decltype(sizeof(0)) n, void* ptr) noexcept(false){
    return ptr;
}

void operator delete(void *p) throw() {
    free(p);
}

void *operator new[](decltype(sizeof(0)) n) noexcept(false) {
    void *memory = malloc(n);
    return memory;
}

void operator delete[](void *p) throw() {
    free(p);
}
#else

#include "new"

#endif

#endif //EMBEDDEDCPLUSPLUS_NEW_H
