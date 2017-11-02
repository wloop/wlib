
#include "gtest/gtest.h"
#include "strings/StaticString.h"

TEST(static_string_tests, constructor_tests){
    StaticString<8> string1{"helloooo"};    // text given
    StaticString<8> string2;                // no text
    StaticString<8> string3{string1};       // string object given

    ASSERT_STREQ(string1.c_str(), "helloooo");
    ASSERT_STREQ(string2.c_str(), "\0");
    ASSERT_STREQ(string3.c_str(), "helloooo");
}