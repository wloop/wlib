#ifndef EMBEDDEDCPLUSPLUS_TEST_HELPER_H
#define EMBEDDEDCPLUSPLUS_TEST_HELPER_H

#include <stdlib.h>

namespace wlp {

    char random_char() {
        return static_cast<char>(rand() % 0x100);
    }

    int random_int() {
        return rand() % 0x10000;
    }

}

#endif //EMBEDDEDCPLUSPLUS_TEST_HELPER_H
