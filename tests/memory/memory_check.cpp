/**
 * @file memory_check.cpp
 * @brief Unit Testing for Memory model used in Wlib
 *
 * @author Deep Dhillon
 * @date November 18, 2017
 * @bug No known bugs
 */

#include <gtest/gtest.h>
#include <wlib/memory/Memory.h>

class Sample {
public:
    static int constr;

    Sample() {
        constr += 4;
    }

    Sample(const char *) {
        constr += 2;
    }

    ~Sample() {
        constr += 5;
    }
};

class SamplePtrContainer {
public:
    Sample *simp;

    SamplePtrContainer()
            : simp(malloc<Sample>()) {
    }

    ~SamplePtrContainer() {
        if (get() != nullptr) {
            free<Sample>(get()); // calls rvalue overload
        }
    }

    void destroy() {
        free<Sample>(simp);
    }

    Sample *get() {
        return simp;
    }
};

int Sample::constr = 0;

TEST(memory_check, general_usability) {
    ASSERT_EQ(getTotalMemoryAvailable(), getTotalMemoryAvailable() - getTotalMemoryUsed());
    ASSERT_TRUE(isSizeAvailable(16));
    ASSERT_TRUE(isSizeMemAvailable(16));
    ASSERT_EQ(getNumBlocks(), getNumBlocksAvailable(16));

    // allocate a character
    auto *character = malloc<char>(); // by default creates one char
    ASSERT_NE(getTotalMemoryAvailable(), getTotalMemoryAvailable() - getTotalMemoryUsed());

    // allocate 2 characters
    auto *character2 = malloc<char[]>(2);

    // free all of them
    free(character);
    free(character2);

    // allocate 4 characters
    auto *character4 = malloc<char[]>(4);

    // reallocate
    character4 = realloc(character4, 8);

    // create an object
    Sample::constr = 0;
    auto *test = malloc<Sample>();
    ASSERT_EQ(4, Sample::constr);

    free(character4);
    free(test);

    ASSERT_EQ(9, Sample::constr);
    ASSERT_EQ(getTotalMemoryAvailable(), getTotalMemoryAvailable() - getTotalMemoryUsed());
}

TEST(memory_check, malloc_realloc_and_calloc) {
    int *v = malloc<int[]>(75);
    v[0] = 75;
    v[1] = 175;
    v[2] = 375;

    int *v2 = malloc<int>();
    v2[0] = 68;

    v = realloc(v, 2);
    ASSERT_EQ(75, v[0]);
    ASSERT_EQ(175, v[1]);

    v2 = realloc(v2, 4);
    ASSERT_EQ(68, v2[0]);

    v2 = realloc(v2, 0);
    ASSERT_EQ(nullptr, v2);

    char *v3 = calloc<char[]>(2);
    ASSERT_EQ('\0', v3[0]);
    ASSERT_EQ('\0', v3[1]);

    uint16_t *v4 = calloc<uint16_t[]>(2);
    ASSERT_EQ(0u, v4[0]);
    ASSERT_EQ(0u, v4[1]);

    uint32_t *v5 = calloc<uint32_t>();
    ASSERT_EQ(0u, v5[0]);

    char *v6 = calloc<char>();
    ASSERT_EQ('\0', v6[0]);

    free(v);
    free(v2);
    free(v3);
    free(v4);
    free(v5);
    free(v6);
}

TEST(memory_check, array_allocation) {
    Sample::constr = 0;

    Sample *s = malloc<Sample[]>(2);
    ASSERT_EQ(8, Sample::constr);

    Sample *d = arg_array_malloc<Sample[]>(3, "hello");
    ASSERT_EQ(14, Sample::constr);

    free(d);
    ASSERT_EQ(29, Sample::constr);
    free(s);
    ASSERT_EQ(39, Sample::constr);
}

TEST(memory_check, free) {
    int *g = malloc<int>(7);
    free<int>(g);
    ASSERT_EQ(nullptr, g);

    free<int>(g);
    ASSERT_EQ(nullptr, g);
}

TEST(memory_check, free_rvalue) {
    Sample::constr = 0;
    SamplePtrContainer *pSpc = malloc<SamplePtrContainer>();
    ASSERT_EQ(4, Sample::constr);
    ASSERT_NE(nullptr, pSpc->simp);
    ASSERT_NE(nullptr, pSpc->get());
    free<SamplePtrContainer>(pSpc);
    ASSERT_EQ(9, Sample::constr);
    ASSERT_EQ(nullptr, pSpc);
    Sample::constr = 0;
    SamplePtrContainer spc;
    ASSERT_EQ(4, Sample::constr);
    spc.destroy();
    ASSERT_EQ(9, Sample::constr);
    ASSERT_EQ(nullptr, spc.simp);
    Sample::constr = 0;
    {
        SamplePtrContainer local_spc;
        ASSERT_EQ(4, Sample::constr);
    } // ~SamplePtrContainer called on local_spc
    ASSERT_EQ(9, Sample::constr);
}

TEST(memory_check, placement_fundamental) {
    int *i = malloc<int>(5);
    ASSERT_EQ(5, *i);
    free<int>(i);
    char *l = malloc<char>('c');
    ASSERT_EQ('c', *l);
    free<char>(l);
}
