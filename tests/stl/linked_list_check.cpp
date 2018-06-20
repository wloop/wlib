#include <gtest/gtest.h>
#include <wlib/stl/LinkedList.h>

#include "../template_defs.h"

using namespace wlp;

typedef linked_list<int>::iterator lli_it;
typedef linked_list<int>::const_iterator lli_cit;

TEST(linked_list_test, constructor_tests) {
    linked_list<int> numlist;
    ASSERT_EQ(numlist.size(), 0u);
}

TEST(linked_list_test, push_pop_remove_tests) {
    linked_list<int> numlist;
    numlist.push_back(1);
    numlist.push_back(2);
    numlist.push_front(3); // 3 1 2
    ASSERT_EQ(numlist.front(), 3);
    ASSERT_EQ(numlist.back(), 2);
    ASSERT_EQ(numlist.size(), 3u);
    numlist.pop_back();
    numlist.pop_front();
    ASSERT_EQ(numlist.front(), 1);
    ASSERT_EQ(numlist.back(), 1);
    numlist.push_front(4);
    numlist.push_back(5);
    numlist.erase(1);
    ASSERT_EQ(numlist.size(), 2u);
    numlist.erase(1);
    ASSERT_EQ(numlist.size(), 1u);
    numlist.pop_front();
    ASSERT_EQ(numlist.size(), 0u);
    // does nothing, should not die
    numlist.erase(100);
    ASSERT_EQ(numlist.size(), 0u);
    numlist.push_front(4);
    ASSERT_EQ(numlist.size(), 1u);
    numlist.pop_back();
    ASSERT_EQ(numlist.size(), 0u);
    numlist.push_back(5);
    ASSERT_EQ(numlist.size(), 1u);
    numlist.erase(0);
    ASSERT_EQ(numlist.size(), 0u);
    numlist.push_back(5);
    ASSERT_EQ(numlist.size(), 1u);
    numlist.clear();
    ASSERT_EQ(numlist.size(), 0u);
    numlist.push_back(3);
    numlist.push_back(1);
    numlist.push_back(2);
    const linked_list<int> constlist = move(numlist);
    ASSERT_EQ(constlist.front(), 3);
    ASSERT_EQ(constlist.back(), 2);
    ASSERT_EQ(constlist.size(), 3u);
}

TEST(linked_list_test, indexing_tests) {
    linked_list<int> numlist;
    numlist.push_front(1);
    numlist.push_back(2);
    numlist.push_front(3); // 3 1 2
    ASSERT_EQ(numlist.index_of(3), 0u);
    ASSERT_EQ(numlist.index_of(1), 1u);
    ASSERT_EQ(numlist.index_of(2), 2u);
    ASSERT_EQ(numlist.index_of(4), 3u);
    ASSERT_EQ(numlist.at(0), 3);
    ASSERT_EQ(numlist[2], 2);
    const linked_list<int> constlist = move(numlist);
    ASSERT_EQ(constlist[0], 3);
    ASSERT_EQ(constlist[1], 1);
    ASSERT_EQ(constlist[2], 2);
}

TEST(linked_list_test, iterator_tests) {
    linked_list<int> numlist;
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
    const linked_list<int> constlist = move(numlist);
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

TEST(linked_list_test, copy_move_constructors) {
    linked_list<int> list;
    auto newlist = move(list);
    linked_list<int> list2;
    auto movedlist = move(list2);
    ASSERT_EQ(newlist.size(), 0u);
    ASSERT_EQ(movedlist.size(), 0u);
}

TEST(linked_list_test, test_insert_index) {
    linked_list<int> list;
    list.insert(0, 10);
    list.insert(0, 15);
    list.insert(1, 20);
    list.insert(1, 25);
    list.insert(2, 40);
    ASSERT_EQ(5u, list.size());
    int traverse[] = {15, 25, 40, 20, 10};
    linked_list<int>::iterator it = list.begin();
    for (size_t i = 0; i < list.size(); i++) {
        ASSERT_EQ(*it, traverse[i]);
        ++it;
    }
    ASSERT_EQ(it, list.end());
}

TEST(linked_list_test, test_insert_iterator) {
    linked_list<int> list;
    ASSERT_EQ(list.begin(), list.end());
    linked_list<int>::iterator it = list.begin();
    it = list.insert(it, 15);
    ASSERT_EQ(it, list.begin());
    ASSERT_NE(list.begin(), list.end());
    list.insert(it, 20);
    ASSERT_EQ(list.begin(), list.find(20));
    list.insert(it, 25);
    ASSERT_EQ(15, *it);
    --it;
    ASSERT_EQ(25, *it);
    it = list.insert(it, 30);
    ++it;
    list.insert(it, 45);
    int traverse[] = {20, 30, 45, 25, 15};
    it = list.begin();
    ASSERT_EQ(5u, list.size());
    for (size_t i = 0; i < list.size(); i++) {
        ASSERT_EQ(traverse[i], *it);
        ++it;
    }
    ASSERT_EQ(it, list.end());
}

TEST(linked_list_test, test_erase_index) {
    linked_list<int> list;
    int values[] = {1, 2, 3, 4, 5, 6, 7};
    for (size_t i = 0; i < 7; i++) {
        list.push_back(values[i]);
    }
    linked_list<int>::iterator it = list.erase(1);
    ASSERT_EQ(3, *it);
    --it;
    ASSERT_EQ(1, *it);
    ASSERT_EQ(list.begin(), it);
    it = list.erase(5);
    ASSERT_EQ(it, list.end());
    it = list.erase(0);
    ASSERT_EQ(3, *it);
    ASSERT_EQ(4u, list.size());
    linked_list<int> empty_list;
    ASSERT_EQ(empty_list.erase(0), empty_list.end());
}

TEST(linked_list_test, test_erase_iterator) {
    linked_list<int> list;
    int values[] = {1, 2, 3, 4, 5, 6, 7};
    for (size_t i = 0; i < 7; i++) {
        list.push_back(values[i]);
    }
    linked_list<int>::iterator it = list.find(5);
    ASSERT_EQ(5, *it);
    it = list.erase(it);
    ASSERT_EQ(6, *it);
    ++it;
    ASSERT_EQ(7, *it);
    it = list.erase(it);
    ASSERT_EQ(it, list.end());
    ASSERT_EQ(list.end(), list.erase(it));
    it = list.erase(list.begin());
    ASSERT_EQ(it, list.begin());
    ASSERT_EQ(2, *it);
}

TEST(linked_list_test, test_find) {
    linked_list<int> list;
    int values[] = {1, 2, 3, 4, 5, 6, 7};
    for (size_t i = 0; i < 7; i++) {
        list.push_back(values[i]);
    }
    ASSERT_EQ(list.find(123), list.end());
    ASSERT_EQ(7, *list.find(7));
    ASSERT_EQ(1, *list.find(1));
    ASSERT_EQ(list.begin(), list.find(1));
}
