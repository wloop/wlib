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
#include <wlib/memory/Memory.h>

int main(int argc, char *argv[]) {
    wlp::size32_type startingFreeMemory = getTotalMemoryFree();

    ::testing::InitGoogleTest(&argc, argv);
    int testStatusCode = RUN_ALL_TESTS();

    wlp::size32_type endingMemory = getTotalMemoryFree();

    int memoryStatusCode = 0;
    if (endingMemory - startingFreeMemory != 0 || startingFreeMemory != getTotalMemoryAvailable())
        memoryStatusCode = 1;

    int finalCode = 0;

    if (testStatusCode != memoryStatusCode)
        finalCode = 1;

    return finalCode;
}
