#include "gtest/gtest.h"
#include "stl/List.h"
#include <stdint.h>

using namespace wlp;

TEST(list_tests, constructor_tests) {
    List<uint8_t> numlist;
    ASSERT_EQ(numlist.size(), 0);
}

TEST(list_tests, push_pop_remove_tests) {
    List<uint16_t> numlist;
    numlist.push_back(1);
    numlist.push_back(2);
    numlist.push_front(3);
    ASSERT_EQ(numlist.front(), 3);
    ASSERT_EQ(numlist.back(), 2);
    ASSERT_EQ(numlist.size(), 3);
    numlist.pop_back();
    numlist.pop_front();
    ASSERT_EQ(numlist.front(), 1);
    ASSERT_EQ(numlist.back(), 1);
    numlist.push_front(4);
    numlist.push_back(5);
    numlist.remove_at(1);
    ASSERT_EQ(numlist.size(), 2);
    numlist.remove_at(1);
    ASSERT_EQ(numlist.size(), 1);
    numlist.remove_at(0);
    ASSERT_EQ(numlist.size(), 0);
    numlist.push_front(4);
    ASSERT_EQ(numlist.size(), 1);
    numlist.pop_back();
    ASSERT_EQ(numlist.size(), 0);
    numlist.push_back(5);
    ASSERT_EQ(numlist.size(), 1);
    numlist.pop_front();
    ASSERT_EQ(numlist.size(), 0);
}

TEST(list_tests, indexing_tests) {
    List<uint16_t> numlist;
    numlist.push_front(1);
    numlist.push_back(2);
    numlist.push_front(3); // 3 1 2
    ASSERT_EQ(numlist.indexOf(3), 0);
    ASSERT_EQ(numlist.indexOf(1), 1);
    ASSERT_EQ(numlist.indexOf(2), 2);
    ASSERT_EQ(numlist.indexOf(4), 3);
    ASSERT_EQ(numlist.at(0), 3);
    ASSERT_EQ(numlist.get(1), 1);
    ASSERT_EQ(numlist[2], 2);
}
