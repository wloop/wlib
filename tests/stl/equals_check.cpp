
#include "gtest/gtest.h"
#include "strings/StaticString.h"
#include "stl/Equal.h"

TEST(equals_test, test_static_string_equals) {
    StaticString<8> str1{"darwin"};
    StaticString<8> str2{"darwin"};
    bool equal = wlp::equals<StaticString<8>>()(str1, str2);
    ASSERT_TRUE(equal);
}
