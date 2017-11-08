#include "gtest/gtest.h"
#include "stl/Bitset.h"

namespace wlp {

    template
    class Bitset<64>;

    template
    class Bitset<46>;

    template
    class Bitset<27>;

    template
    class Bitset<176>;

}

using namespace wlp;

typedef uint16_t ui16;

TEST(bitset_test, test_constructor_64) {
    uint64_t n = 17316249074701521315u;
    bool expected[] = {
            1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0,
            1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1,
            1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1,
            1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1
    };
    Bitset<64> b(n);
    for (ui16 i = 0; i < 64; i++) {
        ASSERT_EQ(expected[i], b.test(i));
    }
    ASSERT_EQ(n, b.to_uint64_t());
    ASSERT_EQ(n & 0xffffffff, b.to_uint32_t());
    ASSERT_EQ(n & 0xffff, b.to_uint16_t());
    ASSERT_EQ(n & 0xff, b.to_uint8_t());
}

TEST(bitset_test, test_constructor_underflow) {
    uint64_t n = 17316249074701521315u;
    bool expected[] = {
            1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0,
            1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1,
            1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    Bitset<46> b(n);
    for (ui16 i = 0; i < 64; i++) {
        ASSERT_EQ(expected[i], b.test(i));
    }
    ASSERT_EQ(n & 0x3fffffffffff, b.to_uint64_t());
    ASSERT_EQ(n & 0xffffffff, b.to_uint32_t());
    ASSERT_EQ(n & 0xffff, b.to_uint16_t());
    ASSERT_EQ(n & 0xff, b.to_uint8_t());
}

TEST(bitset_test, test_constructor_overflow) {
    uint64_t n = 17316249074701521315u;
    bool expected[] = {
            1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0,
            1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0
    };
    Bitset<27> b(n);
    for (ui16 i = 0; i < 32; i++) {
        ASSERT_EQ(expected[i], b.test(i));
    }
    ASSERT_EQ(n & 0x7ffffff, b.to_uint64_t());
    ASSERT_EQ(n & 0x7ffffff, b.to_uint32_t());
    ASSERT_EQ(n & 0xffff, b.to_uint16_t());
    ASSERT_EQ(n & 0xff, b.to_uint8_t());
}

TEST(bitset_test, test_set_reset_flip_get) {
    bool sequence[] = {
            1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0,
            1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
            0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1,
            0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0,
            0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0,
            1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1,
            0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0,
            1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0,
            1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1,
            1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1,
            1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1
    };
    Bitset<176> b1;
    Bitset<176> b2;
    for (ui16 i = 0; i < 176; i++) {
        b2.set(i);
        ASSERT_EQ(1, b2[i]);
        ASSERT_EQ(0, b1[i]);
        if (sequence[i]) {
            b1.set(i);
        } else {
            b2.reset(i);
        }
    }
    for (ui16 i = 0; i < 176; i++) {
        ASSERT_EQ(sequence[i], b1[i]);
        ASSERT_EQ(sequence[i], b2[i]);
    }
    for (ui16 i = 0; i < 176; i++) {
        b1.flip(i);
    }
    for (ui16 i = 0; i < 176; i++) {
        ASSERT_NE(b1[i], b2[i]);
    }
}

TEST(bitset_test, test_copy_constructors) {
    Bitset<42> source1(17316249074701521315u);
    Bitset<42> source2(6426756347354645451u);
    const Bitset<42> copy1_1 = source1;
    const Bitset<42> copy1_2 = copy1_1;
    ASSERT_EQ(source1.to_uint64_t(), copy1_1.to_uint64_t());
    ASSERT_EQ(source1.to_uint64_t(), copy1_2.to_uint64_t());
    Bitset<42> copy2;
    copy2 = source2;
    ASSERT_EQ(copy2.to_uint64_t(), source2.to_uint64_t());
    copy2 = copy1_1;
    ASSERT_EQ(copy2.to_uint64_t(), source1.to_uint64_t());
}
