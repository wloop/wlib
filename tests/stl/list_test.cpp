#include "gtest/gtest.h"
#include "stl/List.h"
#include <stdint.h>
#include "stl/Utility.h"

namespace wlp {
    template class List<uint16_t>;
}

using namespace wlp;

TEST(list_tests, constructor_tests) {
    List<uint16_t> numlist;
    ASSERT_EQ(numlist.size(), 0);
}

TEST(list_tests, push_pop_remove_tests) {
    List<uint16_t> numlist;
    numlist.push_back(1);
    numlist.push_back(2);
    numlist.push_front(3); // 3 1 2
    const List<uint16_t> constlist = numlist;
    ASSERT_EQ(constlist.front(), 3);
    ASSERT_EQ(constlist.back(), 2);
    ASSERT_EQ(constlist.size(), 3);
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
    numlist.pop_front();
    ASSERT_EQ(numlist.size(), 0);
    // does nothing, should not die
    numlist.remove_at(100);
    ASSERT_EQ(numlist.size(), 0);
    numlist.push_front(4);
    ASSERT_EQ(numlist.size(), 1);
    numlist.pop_back();
    ASSERT_EQ(numlist.size(), 0);
    numlist.push_back(5);
    ASSERT_EQ(numlist.size(), 1);
    numlist.remove_at(0);
    ASSERT_EQ(numlist.size(), 0);
    numlist.push_back(5);
    ASSERT_EQ(numlist.size(), 1);
    numlist.clear();
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
    const List<uint16_t> constlist = numlist;
    ASSERT_EQ(constlist[0], 3);
    ASSERT_EQ(constlist[1], 1);
    ASSERT_EQ(constlist[2], 2);
}

TEST(list_tests, iterator_tests) {
    List<uint16_t> numlist;
    numlist.push_front(1);
    numlist.push_back(2);
    numlist.push_front(3); // 3 1 2
    ListIterator<uint16_t> it = numlist.begin();
    ASSERT_EQ(*it, 3);
    ++it;
    ASSERT_EQ(*it, 1);
    it++;
    ASSERT_NE(it, numlist.end());
    ASSERT_EQ(*it, 2);
    ++it;
    ASSERT_EQ(it, numlist.end());
    const List<uint16_t> constlist = numlist;
    ListConstIterator<uint16_t> it2 = constlist.begin();
    ASSERT_EQ(*it2, 3);
    ++it2;
    ASSERT_EQ(*it2, 1);
    it2++;
    ASSERT_NE(it2, constlist.end());
    ASSERT_EQ(*it2, 2);
    ++it2;
    ASSERT_EQ(it2, constlist.end());
}

TEST(list_tests, copy_move_constructors) {
    List<uint16_t> list;
    auto newlist = list;
    List<uint16_t> list2;
    auto movedlist = move(list2);
    ASSERT_EQ(newlist.size(), 0);
    ASSERT_EQ(movedlist.size(), 0);
}

TEST(list_tests, dynamic_list_class) {
    List<uint16_t>* pList = new List<uint16_t>;
    ASSERT_EQ(pList->size(), 0);
    delete pList;
}