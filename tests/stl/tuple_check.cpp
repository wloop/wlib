#include "gtest/gtest.h"

#include "stl/Tuple.h"

using namespace wlp;

TEST(tuple_test, test_tuple_create) {
    Tuple<int, float, const char*> tuple(34, 2.1f, "hello");
    int v1 = get<0>(tuple);
    float v2 = get<1>(tuple);
    const char* v3 = get<2>(tuple);
    ASSERT_EQ(34, v1);
    ASSERT_FLOAT_EQ(2.1f, v2);
    ASSERT_STREQ("hello", v3);
    get<0>(tuple) = 25;
    ASSERT_EQ(25, get<0>(tuple));
}

TEST(tuple_test, test_tuple_default) {
    Tuple<int, float, int> tuple;
    ASSERT_EQ(0, get<0>(tuple));
    ASSERT_FLOAT_EQ(0, get<1>(tuple));
    ASSERT_EQ(0, get<2>(tuple));
}
