#include "gtest/gtest.h"

#include "strings/StringIterator.h"
#include "strings/StaticString.h"
#include "strings/DynamicString.h"
#include "stl/ArrayHeap.h"

using namespace wlp;

TEST(string_iterator_test, test_static_string_iteration) {
    char str[] = "No Gods or Kings, only Man";
    StaticString<64> ss(str);
    size_type i = 0;
    for (char &s : ss) {
        ASSERT_EQ(str[i], s);
        ++i;
    }
}

TEST(string_iterator_test, test_dynamic_string_iteration) {
    char str[] = "Is a man not entitled to the sweat of his brow?";
    DynamicString ds(str);
    size_type i = 0;
    for (char  &s : ds) {
        ASSERT_EQ(str[i], s);
        ++i;
    }
}

TEST(string_iterator_test, test_sort_strings) {
    char str[] = "edafebc";
    char expected[] = "abcdeef";
    StaticString<8> ss(str);
    DynamicString ds(str);
    make_heap(ss.begin(), ss.end());
    make_heap(ds.begin(), ds.end());
    sort_heap(ss.begin(), ss.end());
    sort_heap(ds.begin(), ds.end());
    ASSERT_STREQ(expected, ss.c_str());
    ASSERT_STREQ(expected, ds.c_str());
}
