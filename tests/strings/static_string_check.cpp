#include "gtest/gtest.h"
#include "strings/StaticString.h"
#include "memory/Memory.h"

#include "../template_defs.h"

TEST(static_string_tests, constructor_tests){
    StaticString<8> string1{"helloooo"};    // text given
    StaticString<8> string2;                // no text
    StaticString<8> string3{string1};       // string object given

    auto *ptr = memory_alloc(75);
    ptr = memory_realloc(ptr, 35);
    memory_free(ptr);

    string3.empty();

    string2.clear();

    ASSERT_EQ("helloooo", "helloooo");
}