#include "gtest/gtest.h"
#include "stl/List.h"
#include <stdint.h>

using namespace wlp;

TEST(list_tests, constructor_tests) {
    List<uint8_t> numlist;
    ASSERT_EQ(numlist.size(), 0);
}

TEST(list_tests, push_pop_remove_tests) {
    List<uint8_t> numlist;
    numlist.push_back(1);
    numlist.push_back(2);
    numlist.push_front(3);
    ASSERT_EQ(numlist.front(), 3);
    ASSERT_EQ(numlist.back(), 2);
    ASSERT_EQ(numlist.length(), 3);
    numlist.pop_back();
    numlist.pop_front();
    ASSERT_EQ(numlist.front(), 2);
    ASSERT_EQ(numlist.back(), 2);
    numlist.push_front(4);
    numlist.push_back(5);
    numlist.remove_at(1);
    ASSERT_EQ(numlist.length(), 2);
    numlist.remove_at(1);
    ASSERT_EQ(numlist.length(), 1);
    numlist.remove_at(0);
    ASSERT_EQ(numlist.length(), 0);
}

TEST(list_tests, indexing_tests) {
    List<uint8_t> numlist;
    numlist.push_back(1);
    numlist.push_back(2);
    numlist.push_front(3);
    ASSERT_EQ(numlist.indexOf(2), 1);
    ASSERT_EQ(numlist.at(1), 2);
    ASSERT_EQ(numlist.get(1), 2);
    ASSERT_EQ(numlist[1], 2);
}
