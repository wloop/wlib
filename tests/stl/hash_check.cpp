
#include "gtest/gtest.h"
#include "strings/StaticString.h"
#include "stl/Hash.h"

using namespace wlp;

TEST(hash_test, test_hash_static_string) {
    hash<StaticString<8>, uint16_t> hasher = hash<StaticString<8>, uint16_t>();
    StaticString<8> str1{"darwin"};
    StaticString<8> str2{"darwin"};
    StaticString<8> str3{"hello"};
    ASSERT_EQ(hasher(str1), hasher(str2));
    ASSERT_NE(hasher(str1), hasher(str3));
    ASSERT_NE(hasher(str2), hasher(str3));
}

TEST(hash_test, test_hash_string) {
    hash<char*, uint16_t> hasher = hash<char*, uint16_t>();
    char str1[] = "drawing";
    char str2[] = "downward";
    char str3[] = "drawing";
    ASSERT_EQ(hasher(str1), hasher(str3));
    ASSERT_NE(hasher(str2), hasher(str1));
    ASSERT_NE(hasher(str2), hasher(str3));
}