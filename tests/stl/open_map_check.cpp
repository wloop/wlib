
#include "gtest/gtest.h"
#include "stl/OpenMap.h"

using namespace wlp;

typedef StaticString<16> string16;
typedef OpenHashMap<string16, string16> string_map;
typedef OpenHashMap<uint16_t, uint16_t> int_map;
typedef int_map::iterator imi;
typedef Pair<imi, bool> P_imi_b;

TEST(open_map_test, test_constructor_parameters) {
    int_map map(15, 61);
    ASSERT_EQ(15, map.max_size());
    ASSERT_EQ(0, map.size());
    ASSERT_EQ(61, map.max_load());
}

TEST(open_map_test, test_constructor_allocator) {
    string_map map(12, 75);
    const Allocator* alloc = map.get_node_allocator();
    size_t expected = sizeof(string_map::map_node);
    ASSERT_EQ(expected, alloc->GetBlockSize());
    ASSERT_EQ(expected * 12, alloc->GetPoolSize());
}

TEST(open_map_test, test_is_empty_on_construct) {
    string_map map(12, 75);
    ASSERT_TRUE(map.empty());
}

TEST(open_map_test, test_begin_returns_end_when_empty) {
    string_map map(12, 75);
    ASSERT_EQ(map.begin(), map.end());
}

TEST(open_map_test, test_begin_end_const_and_non_const) {
    const string_map map(12, 75);
    string_map::const_iterator cit = map.begin();
    string_map::const_iterator cit_end = map.end();
    ASSERT_EQ(cit, cit_end);
}

TEST(open_map_test, test_insert_find_iterate_integer) {
    int_map map(10, 61);
    P_imi_b res1 = map.insert(0, 15);
    P_imi_b res2 = map.insert(1, 20);
    P_imi_b res3 = map.insert(0, 35);
    P_imi_b res4 = map.insert(9, 90);
    P_imi_b res5 = map.insert(20, 100);
    P_imi_b res6 = map.insert(19, 120);
    ASSERT_TRUE(res1.second());
    ASSERT_TRUE(res2.second());
    ASSERT_FALSE(res3.second());
    ASSERT_TRUE(res4.second());
    ASSERT_TRUE(res5.second());
    ASSERT_TRUE(res6.second());
    ASSERT_EQ(15, *res1.first());
    ASSERT_EQ(20, *res2.first());
    ASSERT_EQ(15, *res3.first());
    ASSERT_EQ(90, *res4.first());
    ASSERT_EQ(100, *res5.first());
    ASSERT_EQ(120, *res6.first());
    int_map::iterator it1 = res1.first();
    ++it1;
    ASSERT_EQ(it1, res2.first());
    ASSERT_EQ(20, *it1);
    ++it1;
    ASSERT_EQ(it1, res5.first());
    ASSERT_EQ(100, *it1);
    ++it1;
    ASSERT_EQ(it1, res6.first());
    ASSERT_EQ(120, *it1);
    ++it1;
    ASSERT_EQ(it1, res4.first());
    ASSERT_EQ(90, *it1);
    ++it1;
    ASSERT_EQ(it1, map.end());
    ASSERT_EQ(5, map.size());
    ASSERT_EQ(10, map.max_size());
}

TEST(open_map_test, test_map_iterator_postfix) {
    int_map map(15, 75);
    P_imi_b res1 = map.insert(2, 10);
    map.insert(10, 12);
    imi it = res1.first();
    imi it_post = it++;
    ASSERT_NE(it_post, it);
    ASSERT_EQ(it_post, res1.first());
    ASSERT_EQ(*it_post, 10);
    ASSERT_EQ(*it, 12);
    ++it_post;
    ASSERT_EQ(it_post, it);
    ASSERT_EQ(2, map.size());
}

TEST(open_map_test, test_map_insert_or_assign) {
    int_map map(10, 75);
    P_imi_b res1 = map.insert_or_assign(2, 10);
    P_imi_b res2 = map.insert_or_assign(3, 12);
    P_imi_b res3 = map.insert_or_assign(2, 14);
    ASSERT_TRUE(res1.second());
    ASSERT_TRUE(res2.second());
    ASSERT_FALSE(res3.second());
    ASSERT_EQ(14, *res1.first());
    ASSERT_EQ(12, *res2.first());
    ASSERT_EQ(14, *res3.first());
    ASSERT_EQ(2, map.size());
}

TEST(open_map_test, test_at_returns_value) {
    int_map map(10, 75);
    map.insert(10, 12);
    map.insert(16, 15);
    map.insert(20, 19);
    map.insert(4, 16);
    ASSERT_EQ(4, map.size());
    ASSERT_EQ(12, *map.at(10));
    ASSERT_EQ(15, *map.at(16));
    ASSERT_EQ(19, *map.at(20));
    ASSERT_EQ(16, *map.at(4));
}

TEST(open_map_test, test_at_is_assignable) {
    int_map map(10, 75);
    map.insert(10, 12);
    map.insert(16, 15);
    ASSERT_EQ(15, *map.at(16));
    ASSERT_EQ(12, *map.at(10));
    ASSERT_EQ(2, map.size());
    *map.at(16) = 100;
    *map.at(10) = 101;
    ASSERT_EQ(100, *map.at(16));
    ASSERT_EQ(101, *map.at(10));
    ASSERT_EQ(2, map.size());
}

TEST(open_map_test, test_at_returns_pass_the_end) {
    int_map map(10, 75);
    ASSERT_EQ(map.end(), map.at(4));
}

TEST(open_map_test, test_contains_key) {
    string_map map(10, 75);
    string16 key1{"moshi"};
    string16 key2{"welcome"};
    string16 key3{"never"};
    string16 val1{"someval"};
    string16 val2{"anotherval"};
    string16 val3{"yetanother"};
    ASSERT_TRUE(map.insert(key1, val1).second());
    ASSERT_TRUE(map.insert(key2, val2).second());
    ASSERT_TRUE(map.insert(key3, val3).second());
    ASSERT_TRUE(map.contains(key1));
    ASSERT_TRUE(map.contains(key2));
    ASSERT_TRUE(map.contains(key3));
    ASSERT_EQ(val1, *map.at(key1));
    ASSERT_EQ(val2, *map.at(key2));
    ASSERT_EQ(val3, *map.at(key3));
}

TEST(open_map_test, test_find) {
    int_map map(10, 75);
    P_imi_b r1 = map.insert(1, 10);
    P_imi_b r2 = map.insert(3, 30);
    P_imi_b r3 = map.insert(5, 50);
    P_imi_b r4 = map.insert(7, 70);
    ASSERT_EQ(r1.first(), map.find(1));
    ASSERT_EQ(r2.first(), map.find(3));
    ASSERT_EQ(r3.first(), map.find(5));
    ASSERT_EQ(r4.first(), map.find(7));
}

TEST(open_map_test, test_access_operator) {
    int_map map(10, 75);
    map.insert(5, 100);
    map[5] = 19;
    map[10] = 14;
    map[556] = 9901;
    ASSERT_EQ(19, map[5]);
    ASSERT_EQ(14, map[10]);
    ASSERT_EQ(9901, map[556]);
    ASSERT_FALSE(map.insert(5, 20).second());
    ASSERT_FALSE(map.insert(556, 10).second());
    ASSERT_TRUE(map.contains(10));
}

TEST(open_map_test, test_rehash) {
    int_map map(2, 50);
    map[0] = 0;
    map[1] = 10;
    map[2] = 20;
    map[3] = 30;
    map[4] = 40;
    map[115] = 2115;
    map[226] = 2216;
    map[337] = 2317;
    map[448] = 2418;
    int keys[] = {0, 1, 2, 3, 4, 115, 226, 337, 448};
    int values[] = {0, 10, 20, 30, 40, 2115, 2216, 2317, 2418};
    for (uint16_t i = 0; i < 9; i++) {
        ASSERT_TRUE(map.contains(keys[i]));
        ASSERT_EQ(*map.find(keys[i]), values[i]);
    }
}
