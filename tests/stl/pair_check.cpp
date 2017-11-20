#include "gtest/gtest.h"

#include "stl/Pair.h"

#include "../template_defs.h"

using namespace wlp;

TEST(pair_test, test_pair_constructor) {
    Pair<uint16_t, const char *> pair1{5, "string"};
    Pair<uint16_t, const char *> pair2{8, "nothing"};
    Pair<uint16_t, const char *> pair3{5, "string"};
    Pair<uint16_t, const char *> pair4{9, "another"};
    ASSERT_TRUE(pair1 == pair3);
    ASSERT_EQ(5, pair1.first());
    ASSERT_EQ(8, pair2.first());
    ASSERT_STREQ("string", pair3.second());
    ASSERT_STREQ("another", pair4.second());
}

TEST(pair_test, test_pair_assign_operator) {
    Pair<uint16_t, const char *> pair1{5, "string"};
    Pair<uint16_t, const char *> pair2{8, "nothing"};
    pair2 = pair1;
    ASSERT_TRUE(pair1 == pair2);
    ASSERT_EQ(pair1.first(), pair2.first());
    ASSERT_EQ(pair1.second(), pair2.second());
}

TEST(pair_test, test_pair_const_equality) {
    Pair<uint16_t, const char *> pair1{5, "string"};
    const Pair<uint16_t, const char *> pair2{8, "nothing"};
    ASSERT_FALSE(pair1 == pair2);
}
