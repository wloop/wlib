#include "gtest/gtest.h"
#include "strings/StaticString.h"
#include "stl/Equal.h"

#include "../template_defs.h"

using namespace wlp;

TEST(equals_test, test_string_equals) {
	equals<char*> comparator = equals<char*>();
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
	equals<StaticString<8>> comparator = equals<StaticString<8>>();
	StaticString<8> str1{"darwin"};
	StaticString<8> str2{"darwin"};
	StaticString<8> str3{"money__\""};
	StaticString<8> str4{"money__\""};
	ASSERT_TRUE(comparator(str1, str2));
	ASSERT_TRUE(comparator(str3, str4));
}

TEST(equals_test, test_static_string_unequal) {
	equals<StaticString<8>> comparator = equals<StaticString<8>>();
	StaticString<8> str1{"darwin"};
	StaticString<8> str2{"money__\""};
	StaticString<8> str3{"houses"};
	ASSERT_FALSE(comparator(str1, str2));
	ASSERT_FALSE(comparator(str2, str3));
}

TEST(equals_test, test_integer_equal) {
	equals<uint16_t> comparator = equals<uint16_t>();
	ASSERT_TRUE(comparator(1, 1));
	ASSERT_TRUE(comparator(15, 15));
	ASSERT_FALSE(comparator(1, 14));
	ASSERT_FALSE(comparator(14, 1));
}