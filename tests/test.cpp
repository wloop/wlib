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

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
