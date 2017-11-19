/**
 * @file memory_check.cpp
 * @brief Unit Testing for Memory model used in Wlib
 *
 * @author Deep Dhillon
 * @date November 18, 2017
 * @bug No known bugs
 */

#include "gtest/gtest.h"

#include "memory/Memory.h"
#include "memory/MemoryConfiguration.h"

class Sample{
    int constr = 0;
public:

    Sample(){
        constr += 4;
    }

    ~Sample(){
        constr += 5;
    }

    int getConstr(){
        return constr;
    }
};

TEST(memory_check, general_usability) {
    ASSERT_EQ(getTotalMemoryAvailable(), getTotalMemoryAvailable() - getTotalMemoryUsed());
    ASSERT_TRUE(isSizeAvailable(16));
    ASSERT_TRUE(isSizeMemAvailable(16));
    ASSERT_EQ(getNumBlocks(), getNumBlocksAvailable(16));
    ASSERT_EQ(NUM_BLOCKS, getNumBlocks());
    ASSERT_EQ(MAX_ALLOCATORS, getMaxAllocations());

    // allocate a character
    auto *character = malloc<char>(); // by default creates one char
    ASSERT_NE(getTotalMemoryAvailable(), getTotalMemoryAvailable() - getTotalMemoryUsed());

    // allocate 2 characters
    auto *character2 = malloc<char>(2);

    // free all of them
    free(character);
    free(character2);

    // allocate 4 characters
    auto *character4 = malloc<char>(4);

    // reallocate
    character4 = realloc(character4, 8);

    // create an object
    auto *test = malloc<Sample>();
    ASSERT_EQ(4, test->getConstr());

    free(character4);
    free(test);

    ASSERT_EQ(9, test->getConstr());
    ASSERT_EQ(getTotalMemoryAvailable(), getTotalMemoryAvailable() - getTotalMemoryUsed());
}

TEST(memory_check, malloc_and_realloc){
    int *v = malloc<int>(75);
    *v = 75;
    int *v2 = malloc<int>();
    *v2 = 68;

    v2 = realloc(v2);
    ASSERT_EQ(68, *v2);

    v2 = realloc(v2, 15);
    ASSERT_EQ(68, *v2);

    v2 = realloc(v, 15);
    ASSERT_EQ(75, *v2);

    v2 = realloc(v2, 0);
    ASSERT_EQ(nullptr, v2);

    free(v);
    free(v2);
}

TEST(memory_check, free){
    free(nullptr);

    int *g = malloc<int>(7);
    free(g);
    int *l = malloc<int>(7);
    ASSERT_EQ(l, g);
    free(g);
}

