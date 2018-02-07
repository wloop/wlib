#include <gtest/gtest.h>
#include <wlib/stl/Bitset.h>

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
    ASSERT_EQ(n, b.to_uint64());
    ASSERT_EQ(n & 0xffffffff, b.to_uint32());
    ASSERT_EQ(n & 0xffff, b.to_uint16());
    ASSERT_EQ(n & 0xff, b.to_uint8());
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
    ASSERT_EQ(n & 0x3fffffffffff, b.to_uint64());
    ASSERT_EQ(n & 0xffffffff, b.to_uint32());
    ASSERT_EQ(n & 0xffff, b.to_uint16());
    ASSERT_EQ(n & 0xff, b.to_uint8());
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
    ASSERT_EQ(n & 0x7ffffff, b.to_uint64());
    ASSERT_EQ(n & 0x7ffffff, b.to_uint32());
    ASSERT_EQ(n & 0xffff, b.to_uint16());
    ASSERT_EQ(n & 0xff, b.to_uint8());
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
    ASSERT_EQ(source1.to_uint64(), copy1_1.to_uint64());
    ASSERT_EQ(source1.to_uint64(), copy1_2.to_uint64());
    Bitset<42> copy2;
    copy2 = source2;
    ASSERT_EQ(copy2.to_uint64(), source2.to_uint64());
    copy2 = copy1_1;
    ASSERT_EQ(copy2.to_uint64(), source1.to_uint64());
}

TEST(bitset_test, test_to_string) {
    Bitset<64> bits(7359837697304912481u);
    char expected[] = "abc@de#f";
    ASSERT_STREQ(expected, bits.to_static_string().c_str());
    ASSERT_STREQ(expected, bits.to_dynamic_string().c_str());
}
