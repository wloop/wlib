#include "gtest/gtest.h"
#include "stl/LinkedList.h"

#include "../template_defs.h"

using namespace wlp;

typedef LinkedList<int>::iterator lli_it;
typedef LinkedList<int>::const_iterator lli_cit;

TEST(list_tests, constructor_tests) {
    LinkedList<int> numlist;
    ASSERT_EQ(numlist.size(), 0);
}

TEST(list_tests, push_pop_remove_tests) {
    LinkedList<int> numlist;
    numlist.push_back(1);
    numlist.push_back(2);
    numlist.push_front(3); // 3 1 2
    ASSERT_EQ(numlist.front(), 3);
    ASSERT_EQ(numlist.back(), 2);
    ASSERT_EQ(numlist.size(), 3);
    numlist.pop_back();
    numlist.pop_front();
    ASSERT_EQ(numlist.front(), 1);
    ASSERT_EQ(numlist.back(), 1);
    numlist.push_front(4);
    numlist.push_back(5);
    numlist.erase(1);
    ASSERT_EQ(numlist.size(), 2);
    numlist.erase(1);
    ASSERT_EQ(numlist.size(), 1);
    numlist.pop_front();
    ASSERT_EQ(numlist.size(), 0);
    // does nothing, should not die
    numlist.erase(100);
    ASSERT_EQ(numlist.size(), 0);
    numlist.push_front(4);
    ASSERT_EQ(numlist.size(), 1);
    numlist.pop_back();
    ASSERT_EQ(numlist.size(), 0);
    numlist.push_back(5);
    ASSERT_EQ(numlist.size(), 1);
    numlist.erase(0);
    ASSERT_EQ(numlist.size(), 0);
    numlist.push_back(5);
    ASSERT_EQ(numlist.size(), 1);
    numlist.clear();
    ASSERT_EQ(numlist.size(), 0);
    numlist.push_back(3);
    numlist.push_back(1);
    numlist.push_back(2);
    const LinkedList<int> constlist = move(numlist);
    ASSERT_EQ(constlist.front(), 3);
    ASSERT_EQ(constlist.back(), 2);
    ASSERT_EQ(constlist.size(), 3);
}

TEST(list_tests, indexing_tests) {
    LinkedList<int> numlist;
    numlist.push_front(1);
    numlist.push_back(2);
    numlist.push_front(3); // 3 1 2
    ASSERT_EQ(numlist.indexOf(3), 0);
    ASSERT_EQ(numlist.indexOf(1), 1);
    ASSERT_EQ(numlist.indexOf(2), 2);
    ASSERT_EQ(numlist.indexOf(4), 3);
    ASSERT_EQ(numlist.at(0), 3);
    ASSERT_EQ(numlist[2], 2);
    const LinkedList<int> constlist = move(numlist);
    ASSERT_EQ(constlist[0], 3);
    ASSERT_EQ(constlist[1], 1);
    ASSERT_EQ(constlist[2], 2);
}

TEST(list_tests, iterator_tests) {
    LinkedList<int> numlist;
    numlist.push_front(1);
    numlist.push_back(2);
    numlist.push_front(3); // 3 1 2
    lli_it it = numlist.begin();
    ASSERT_EQ(*it, 3);
    ++it;
    ASSERT_EQ(*it, 1);
    it++;
    ASSERT_NE(it, numlist.end());
    ASSERT_EQ(*it, 2);
    ++it;
    ASSERT_EQ(it, numlist.end());
    const LinkedList<int> constlist = move(numlist);
    lli_cit it2 = constlist.begin();
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
    LinkedList<int> list;
    auto newlist = move(list);
    LinkedList<int> list2;
    auto movedlist = move(list2);
    ASSERT_EQ(newlist.size(), 0);
    ASSERT_EQ(movedlist.size(), 0);
}

TEST(list_tests, dynamic_list_class) {
    LinkedList<int>* pList = new LinkedList<int>;
    ASSERT_EQ(pList->size(), 0);
    delete pList;
}