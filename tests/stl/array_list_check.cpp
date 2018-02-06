#include <gtest/gtest.h>
#include <wlib/stl/ArrayList.h>

#include "../template_defs.h"

using namespace wlp;

typedef ArrayList<int>::const_iterator cit;

TEST(array_list_test, test_constructors) {
    int values[] = {1, 2, 3, 4, 5};
    ArrayList<int> list(values, 5, 2);
    ASSERT_EQ(5, list.capacity());
    ASSERT_EQ(5, list.size());
    ArrayList<int> list0(values, 5);
    ASSERT_EQ(5, list0.capacity());
    ASSERT_EQ(5, list.size());
    for (size_type i = 0; i < 5; i++) {
        ASSERT_EQ(values[i], list[i]);
        ASSERT_EQ(values[i], list0[i]);
    }
}

TEST(array_list_test, test_at) {
    int values[] = {2, 3, 5, 7, 11, 13};
    ArrayList<int> list(values, 6);
    ASSERT_EQ(2, list.at(0));
    ASSERT_EQ(11, list.at(4));
    list.at(0) = 111;
    ASSERT_EQ(111, list[0]);
    ASSERT_EQ(111, list.front());
    list.at(1) = 222;
    ASSERT_EQ(222, list.at(7));
    const ArrayList<int> const_list(move(list));
    ASSERT_EQ(111, const_list.at(0));
    ASSERT_EQ(222, const_list.at(7));
    ASSERT_EQ(111, const_list.front());
    ASSERT_EQ(13, const_list[5]);
    ASSERT_EQ(13, const_list.back());
    ASSERT_EQ(111, const_list.data()[0]);
}

TEST(array_list_test, test_const_list_back_front_when_empty) {
    int values[] = {1, 2, 3};
    ArrayList<int> list(values, 3);
    list.clear();
    const ArrayList<int> const_list(move(list));
    ASSERT_EQ(0, const_list.size());
    ASSERT_EQ(1, const_list.back());
    ASSERT_EQ(1, const_list.front());
}

TEST(array_list_test, test_clear) {
    int values[] = {2, 3, 5, 7};
    ArrayList<int> list(values, 4);
    list.clear();
    ASSERT_EQ(0, list.size());
    ASSERT_EQ(4, list.capacity());
    list.clear();
    ASSERT_EQ(2, list.front());
    ASSERT_EQ(2, list.back());
    ASSERT_EQ(2, list.at(100));
}

TEST(array_list_test, test_list_begin_end) {
    ArrayList<int> list(5);
    ASSERT_EQ(list.begin(), list.end());
    list.push_back(1);
    ASSERT_EQ(1, *list.begin());
    ArrayList<int>::iterator it = list.begin();
    ASSERT_EQ(1, *it);
    ++it;
    ASSERT_EQ(list.end(), it);
}

TEST(array_list_test, test_const_list_begin_end) {
    int values[] = {1, 2, 3, 4};
    const ArrayList<int> const_list(values, 4);
    const ArrayList<int> empty_const_list(5);
    ASSERT_EQ(empty_const_list.begin(), empty_const_list.end());
    ArrayList<int>::const_iterator it = const_list.begin();
    ASSERT_EQ(1, *it);
    ++it;
    ASSERT_EQ(2, *it);
    ++it;
    ASSERT_EQ(3, *it);
    ++it;
    ASSERT_EQ(4, *it);
    ++it;
    ASSERT_EQ(const_list.end(), it);
}

TEST(array_list_test, test_insert_index_lvalue) {
    int values[] = {1, 2, 3, 4};
    ArrayList<int> list(values, 4, 5);
    ASSERT_EQ(4, list.size());
    const int v = 100;
    ArrayList<int>::iterator it = list.insert(2, v);
    ASSERT_EQ(100, *it);
    int expected[] = {1, 2, 100, 3, 4};
    ASSERT_EQ(5, list.size());
    for (size_type i = 0; i < list.size(); i++) {
        ASSERT_EQ(expected[i], list[i]);
    }
}

TEST(array_list_test, test_insert_index_rvalue) {
    int values[] = {1, 10};
    ArrayList<int> list(values, 2, 3);
    ASSERT_EQ(2, list.size());
    ArrayList<int>::iterator it = list.insert(1, 100);
    ASSERT_EQ(100, *it);
    int expected[] = {1, 100, 10};
    ASSERT_EQ(3, list.size());
    for (size_type i = 0; i < list.size(); i++) {
        ASSERT_EQ(expected[i], list[i]);
    }
}

TEST(array_list_test, test_insert_iterator_lvalue) {
    int values[] = {1, 10};
    ArrayList<int> list(values, 2, 3);
    ArrayList<int>::iterator it = list.end();
    const int v = 100;
    it = list.insert(it, v);
    ASSERT_EQ(3, list.size());
    ASSERT_EQ(100, *it);
    ++it;
    ASSERT_EQ(it, list.end());
}

TEST(array_list_test, test_insert_iterator_rvalue) {
    int values[] = {1, 10};
    ArrayList<int> list(values, 2, 2);
    ArrayList<int>::iterator it = list.begin();
    it = list.insert(it, 100);
    ASSERT_EQ(3, list.size());
    ASSERT_EQ(100, *it);
    ++it;
    ASSERT_EQ(1, *it);
    ++it;
    ASSERT_EQ(10, *it);
    ++it;
    ASSERT_EQ(list.end(), it);
}

TEST(array_list_test, test_insert_when_full) {
    int values[] = {1, 10};
    ArrayList<int> list(values, 2);
    list.insert(1, 15);
    ASSERT_EQ(3, list.size());
    ASSERT_EQ(4, list.capacity());
    ASSERT_EQ(1, *list.begin());
    ASSERT_EQ(15, list.at(1));
    ASSERT_EQ(10, list.at(2));
}

TEST(array_list_test, test_insert_when_empty) {
    ArrayList<int> list(5);
    list.insert(0, 10);
    ASSERT_EQ(1, list.size());
    ASSERT_EQ(5, list.capacity());
    ASSERT_EQ(10, list.at(0));
}

TEST(array_list_test, test_insert_iterator_when_empty) {
    ArrayList<int> list1(5);
    ArrayList<int>::iterator it1 = list1.begin();
    it1 = list1.insert(it1, 10);
    ASSERT_EQ(1, list1.size());
    ASSERT_EQ(10, list1[0]);
    ASSERT_EQ(10, *it1);
    ArrayList<int> list2(5);
    ArrayList<int>::iterator it2 = list2.end();
    it2 = list2.insert(it2, 10);
    ASSERT_EQ(1, list2.size());
    ASSERT_EQ(10, list2[0]);
    ASSERT_EQ(10, *it2);
}

TEST(array_list_test, test_insert_iterator) {
    int values[] = {1, 2, 3, 4, 5};
    ArrayList<int> list(values, 5, 5);
    int traverse1[] = {1, 2, 10, 3, 4, 5};
    ArrayList<int>::iterator it = list.insert(2, 10);
    for (size_type i = 0; i < list.size(); i++) {
        ASSERT_EQ(list[i], traverse1[i]);
    }
    ++it;
    ++it;
    it = list.insert(it, 15);
    ASSERT_EQ(15, *it);
    int traverse2[] = {1, 2, 10, 3, 15, 4, 5};
    for (size_type i = 0; i < list.size(); i++) {
        ASSERT_EQ(list[i], traverse2[i]);
    }
}

TEST(array_list_test, test_insert_iterator_begin_end) {
    int values[] = {1, 2, 3, 4, 5};
    ArrayList<int> list(values, 5, 5);
    list.insert(list.begin(), 15);
    list.insert(list.end(), 20);
    int traverse[] = {15, 1, 2, 3, 4, 5, 20};
    for (size_type i = 0; i < list.size(); i++) {
        ASSERT_EQ(traverse[i], list[i]);
    }
}

TEST(array_list_test, test_push_pop_back) {
    ArrayList<int> list;
    list.push_back(10);
    list.push_back(15);
    list.push_back(20);
    int traverse[] = {10, 15, 20};
    for (size_type i = 0; i < list.size(); i++) {
        ASSERT_EQ(traverse[i], list[i]);
    }
    list.pop_back();
    list.pop_back();
    ASSERT_EQ(1, list.size());
    ASSERT_EQ(list[0], 10);
}

TEST(array_list_test, test_push_pop_front) {
    ArrayList<int> list;
    list.push_front(0);
    list.push_front(10);
    list.push_front(15);
    int traverse[] = {15, 10, 0};
    for (size_type i = 0; i < list.size(); i++) {
        ASSERT_EQ(traverse[i], list[i]);
    }
    list.pop_front();
    list.pop_front();
    ASSERT_EQ(1, list.size());
    ASSERT_EQ(list[0], 0);
}

TEST(array_list_test, test_index_of_empty_list) {
    ArrayList<int> empty_list;
    ASSERT_EQ(0, empty_list.index_of(1234));
}

TEST(array_list_test, test_index_of) {
    int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ArrayList<int> list(values, 10);
    for (size_type i = 0; i < 9; i++) {
        ASSERT_EQ(i, list.index_of(i + 1));
    }
}

TEST(array_list_test, test_index_of_repeats) {
    int values[] = {1, 2, 3, 4, 5, 6, 5, 7, 5, 3};
    ArrayList<int> list(values, 10);
    ASSERT_EQ(4, list.index_of(5));
}

TEST(array_list_test, test_index_of_not_found) {
    int values[] = {1, 2, 3, 4, 5, 6, 7, 10};
    ArrayList<int> list(values, 8);
    ASSERT_EQ(8, list.size());
    ASSERT_EQ(8, list.index_of(22));
    ASSERT_EQ(8, list.index_of(8));
}

TEST(array_list_test, test_find) {
    int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 10};
    size_type size = 9;
    ArrayList<int> list(values, size);
    ASSERT_EQ(list.begin(), list.find(1));
    ASSERT_EQ(list.end(), list.find(1000));
    ASSERT_EQ(5, *list.find(5));
}

TEST(array_list_test, test_push_back_when_full) {
    int values[] = {1, 2};
    ArrayList<int> list(values, 2);
    ASSERT_EQ(2, list.size());
    ASSERT_EQ(2, list.capacity());
    list.push_back(3);
    ASSERT_EQ(3, list.size());
    ASSERT_EQ(4, list.capacity());
    ASSERT_EQ(3, list.at(2));
}

TEST(array_list_test, test_erase_index) {
    int values[] = {1, 2, 3};
    ArrayList<int> list(values, 3);
    ASSERT_EQ(3, list.size());
    ASSERT_EQ(3, list.capacity());
    ArrayList<int>::iterator it = list.erase(1);
    ASSERT_EQ(3, *it);
    ++it;
    ASSERT_EQ(list.end(), it);
    list.erase(0);
    list.erase(0);
    ASSERT_EQ(0, list.size());
    ASSERT_EQ(list.end(), list.erase(100));
}

TEST(array_list_test, test_erase_iterator) {
    int values[] = {1, 2, 3};
    ArrayList<int> list(values, 3);
    ArrayList<int>::iterator it = list.end();
    ASSERT_EQ(list.end(), list.erase(it));
    it = list.begin();
    it = list.erase(it);
    ASSERT_EQ(2, *it);
    ASSERT_EQ(2, list.size());
    it = list.erase(it);
    ASSERT_EQ(3, *it);
    ASSERT_EQ(1, list.size());
    it = list.erase(it);
    ASSERT_EQ(list.end(), it);
    ASSERT_EQ(0, list.size());
    ASSERT_EQ(list.end(), list.erase(it));
}

TEST(array_list_test, test_reserve) {
    ArrayList<int> list(10);
    ASSERT_EQ(10, list.capacity());
    list.reserve(5);
    ASSERT_EQ(10, list.capacity());
    list.push_back(10);
    list.push_back(5);
    list.reserve(15);
    ASSERT_EQ(15, list.capacity());
    ASSERT_EQ(10, list.front());
    ASSERT_EQ(5, list.back());
}

TEST(array_list_test, test_shrink) {
    int values[] = {1, 2, 3};
    ArrayList<int> list(values, 3);
    list.shrink();
    ASSERT_EQ(3, list.size());
    ASSERT_EQ(3, list.capacity());
    list.pop_back();
    list.pop_back();
    ASSERT_EQ(1, list.size());
    ASSERT_EQ(3, list.capacity());
    list.shrink();
    ASSERT_EQ(1, list.capacity());
    ASSERT_EQ(1, list[0]);
}

TEST(list_iterator_test, test_default_ctor) {
    ArrayList<int>::iterator it;
}

TEST(list_iterator_test, test_move_ctor) {
    int values[] = {1, 2, 3};
    ArrayList<int> list(values, 3);
    ArrayList<int>::iterator it = list.begin();
    ASSERT_EQ(1, *it);
    ArrayList<int>::iterator it1(move(it));
    ASSERT_EQ(1, *it1);
}

TEST(list_iterator_test, test_overflow_ctor) {
    int values[] = {1, 2, 3};
    ArrayList<int> list(values, 3);
    ArrayList<int>::iterator it(100, &list);
    ASSERT_EQ(list.end(), it);
}

TEST(list_iterator_test, test_arrow_op) {
    ArrayList<String8> string_list(2);
    string_list.push_back(String8("hello"));
    string_list.push_back(String8("woah"));
    ArrayList<String8>::iterator it = string_list.begin();
    ASSERT_EQ(5, it->length());
    ++it;
    ASSERT_EQ(4, it->length());
    ++it;
    ASSERT_EQ(string_list.end(), it);
    ++it;
    ASSERT_EQ(string_list.end(), it);
    it++;
    ASSERT_EQ(string_list.end(), it);
    string_list.push_back(String8("boi"));
    it = string_list.begin();
    it += 2;
    ASSERT_EQ(3, it->length());
    it -= 2;
    ASSERT_EQ(5, it->length());
    size_type v = 2;
    it += v;
    ASSERT_EQ(3, it->length());
    it -= v;
    ASSERT_EQ(5, it->length());
    it += (size_type) 100;
    ASSERT_EQ(string_list.end(), it);
    it += v;
    ASSERT_EQ(string_list.end(), it);
    it -= 1;
    ASSERT_EQ(3, it->length());
    v = 1;
    it -= v;
    ASSERT_EQ(4, it->length());
    --it;
    --it;
    --it;
    ASSERT_EQ(string_list.begin(), it);
}

TEST(list_iterator_test, test_equality_operators) {
    ArrayList<int> list(5);
    ArrayList<int>::iterator it1 = list.begin();
    ArrayList<int>::iterator it2 = list.end();
    ASSERT_TRUE(it1 == it2);
    it1 = list.insert(0, 100);
    it2 = list.end();
    ASSERT_TRUE(it1 != it2);
    const ArrayList<int>::iterator it3 = list.begin();
    const ArrayList<int>::iterator it4 = list.end();
    ASSERT_TRUE(it3 != it4);
    ASSERT_FALSE(it3 == it4);
}

TEST(list_iterator_test, test_add_subtract) {
    int values[] = {1, 2, 3, 4, 5, 6, 7};
    ArrayList<int> list(values, 7);
    ArrayList<int>::iterator it1 = list.begin();
    ArrayList<int>::iterator it2 = it1 + 4;
    ASSERT_EQ(5, *it2);
    size_type v = 2;
    it1 = it2 - v;
    ASSERT_EQ(3, *it1);
    ASSERT_EQ(-7, list.begin() - list.end());
    ASSERT_EQ(7, list.end() - list.begin());
    it1 = list.end();
    ASSERT_EQ(-7, list.begin() - it1);
}

TEST(list_const_iterator_test, test_arrow_op) {
    ArrayList<String8> list(5);
    list.push_back(String8("hello"));
    const ArrayList<String8> const_list(move(list));
    ASSERT_EQ(5, const_list.begin()->length());
}

TEST(list_const_iterator_test, test_bulk_functions) {
    cit it0;
    int values[] = {1, 2, 3, 4, 5, 6, 7, 8};
    const ArrayList<int> list(values, 8);
    cit it1 = list.begin();
    cit it2 = it1;
    ASSERT_EQ(1, *it1);
    ASSERT_EQ(1, *it2);
    cit it3(100, &list);
    ASSERT_EQ(list.end(), it3);
    cit it4 = list.end();
    it4++;
    ASSERT_EQ(list.end(), it4);
    cit it5 = list.begin();
    size_type v = 2;
    it5 += 2;
    it5 += v;
    ASSERT_EQ(5, *it5);
    it5 += 100;
    ASSERT_EQ(list.end(), it5);
    cit it6 = list.begin();
    v = 100;
    it6 += v;
    ASSERT_EQ(list.end(), it6);
    cit it7 = list.begin();
    it7--;
    ASSERT_EQ(it7, list.begin());
    it7++;
    it7 += 5;
    ASSERT_EQ(7, *it7);
    it7--;
    ASSERT_EQ(6, *it7);
    it7 -= 2;
    ASSERT_EQ(4, *it7);
    v = 2;
    it7 -= v;
    ASSERT_EQ(2, *it7);
    it7 -= 100;
    ASSERT_EQ(list.begin(), it7);
    it7 += 5;
    ASSERT_EQ(6, *it7);
    v = 100;
    it7 -= v;
    ASSERT_EQ(list.begin(), it7);
    cit g1 = list.begin();
    cit g2 = list.end();
    ASSERT_FALSE(g1 == g2);
    ASSERT_TRUE(g1 != g2);
    ASSERT_NE(g1, g2);
    g1 = g2;
    ASSERT_EQ(g1, g2);
    cit g3;
    g3 = move(g1);
    ASSERT_EQ(g3, g2);
    cit g4 = list.begin();
    v = 2;
    cit g5 = g4 + v;
    cit g6 = g4 + 2;
    ASSERT_EQ(g6, g5);
    ASSERT_EQ(3, *g5);
    cit g7 = g5 - 1;
    v = 1;
    cit g8 = g5 - v;
    ASSERT_EQ(g7, g8);
    ASSERT_EQ(2, *g8);
    ASSERT_EQ(8, list.end() - list.begin());
    ASSERT_EQ(-8, list.begin() - list.end());
    g8 = list.end();
    ASSERT_EQ(-8, list.begin() - g8);
    g8 = list.begin();
    ASSERT_EQ(8, list.end() - g8);
    cit g9 = move(g8);
    ASSERT_EQ(list.begin(), g9);
}
