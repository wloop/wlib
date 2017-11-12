#include "gtest/gtest.h"
#include "strings/StaticString.h"
#include "stl/Hash.h"

#include "../template_defs.h"

using namespace wlp;

TEST(hash_test, test_hash_static_string) {
	hash<String8, uint16_t> hasher = hash<String8, uint16_t>();
	String8 str1{"darwin"};
	String8 str2{"darwin"};
	String8 str3{"hello"};
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

TEST(hash_test, test_hash_integer) {
	hash<uint16_t, uint16_t> hasher = hash<uint16_t, uint16_t>();
	ASSERT_EQ(4, hasher(4));
	ASSERT_EQ(hasher(10), hasher(10));
	ASSERT_EQ(1556, hasher(1556));
}