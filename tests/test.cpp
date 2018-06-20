/**
 * @file test.cpp
 * @brief main file for all the tests and to run them together
 *
 * This file should never be modified
 *
 * @author Deep Dhillon
 * @date November 05, 2017
 * @bug No known bugs
 */

#include <gtest/gtest.h>

namespace wlp {
    namespace mem {
        void *alloc(size_t bytes)
        { return ::malloc(bytes); }
        void free(void *ptr)
        { return ::free(ptr); }
        void *realloc(void *ptr, size_t bytes)
        { return ::realloc(ptr, bytes); }
    }
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
