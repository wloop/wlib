#if (defined(__AVR_ATmega1280__)  || \
     defined(__AVR_ATmega2560__)  || \
     defined(__AVR_ATmega1284__)  || \
     defined(__AVR_ATmega1284P__) || \
     defined(__AVR_ATmega644__)   || \
     defined(__AVR_ATmega644A__)  || \
     defined(__AVR_ATmega644P__)  || \
     defined(__AVR_ATmega644PA__)) && !defined(__AVR_Cosa__)

#include "New.h"

void *operator new(decltype(sizeof(0)), void *ptr) noexcept {
    return ptr;
}

#elif defined(WLIB_DEBUG) && !defined(__AVR_Cosa__)
#else

#include <stdlib.h>
#include "New.h"

void *operator new(decltype(sizeof(0)) n) noexcept(false) {
    return malloc(n);
}

void *operator new(decltype(sizeof(0)), void *ptr) noexcept {
    return ptr;
}

void operator delete(void *ptr) throw() {
    free(ptr);
}

void *operator new[](decltype(sizeof(0)) n) noexcept(false) {
    return malloc(n);
}

void operator delete[](void *ptr) throw() {
    free(ptr);
}

#endif
