#include <gtest/gtest.h>
#include <wlib/strings/String.h>
#include <wlib/stl/Equal.h>

#include "../template_defs.h"

using namespace wlp;

TEST(equals_test, test_string_equals) {
    Equal<char *> comparator = Equal<char *>();
    char str1[] = "somestring";
    char str2[] = "somestring";
    char str3[] = "shoestring";
    char str4[] = "money";
    ASSERT_TRUE(comparator(str1, str2));
    ASSERT_FALSE(comparator(str2, str3));
    ASSERT_FALSE(comparator(str4, str3));
    ASSERT_FALSE(comparator(str2, str4));
    ASSERT_FALSE(comparator(str1, str4));
}

TEST(equals_test, test_static_string_equals) {
    Equal<String8> comparator = Equal<String8>();
    String8 str1{"darwin"};
    String8 str2{"darwin"};
    String8 str3{"money__\""};
    String8 str4{"money__\""};
    ASSERT_TRUE(comparator(str1, str2));
    ASSERT_TRUE(comparator(str3, str4));
}

TEST(equals_test, test_static_string_unequal) {
    Equal<String8> comparator = Equal<String8>();
    String8 str1{"darwin"};
    String8 str2{"money__\""};
    String8 str3{"houses"};
    ASSERT_FALSE(comparator(str1, str2));
    ASSERT_FALSE(comparator(str2, str3));
}

TEST(equals_test, test_integer_equal) {
    Equal<uint16_t> comparator = Equal<uint16_t>();
    ASSERT_TRUE(comparator(1, 1));
    ASSERT_TRUE(comparator(15, 15));
    ASSERT_FALSE(comparator(1, 14));
    ASSERT_FALSE(comparator(14, 1));
}