#include <gtest/gtest.h>
#include <wlib/strings/StringIterator.h>
#include <wlib/strings/String.h>
#include <wlib/stl/ArrayHeap.h>

using namespace wlp;

TEST(string_iterator_test, test_static_string_iteration) {
    char str[] = "No Gods or Kings, only Man";
    static_string<64> ss(str);
    size_t i = 0;
    for (char &s : ss) {
        ASSERT_EQ(str[i], s);
        ++i;
    }
}

TEST(string_iterator_test, test_dynamic_string_iteration) {
    char str[] = "Is a man not entitled to the sweat of his brow?";
    dynamic_string ds(str);
    size_t i = 0;
    for (char  &s : ds) {
        ASSERT_EQ(str[i], s);
        ++i;
    }
}

TEST(string_iterator_test, test_sort_strings) {
    char str[] = "edafebc";
    char expected[] = "abcdeef";
    static_string<8> ss(str);
    dynamic_string ds(str);
    make_heap(ss.begin(), ss.end());
    make_heap(ds.begin(), ds.end());
    sort_heap(ss.begin(), ss.end());
    sort_heap(ds.begin(), ds.end());
    ASSERT_STREQ(expected, ss.c_str());
    ASSERT_STREQ(expected, ds.c_str());
}
