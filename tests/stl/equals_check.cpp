
#include "gtest/gtest.h"
#include "strings/StaticString.h"
#include "stl/Equal.h"

TEST(equals_test, test_static_string_equals) {
    StaticString<8> str1{"darwin"};
    StaticString<6> str2{"darwin"};
    bool equal = equals<StaticString>()(str1, str2);
    ASSERT_TRUE(equal);
}
