#include <gtest/gtest.h>
#include <wlib/stl/HashMap.h>
#include <wlib/strings/String.h>

#include "../template_defs.h"

using namespace wlp;

typedef uint16_t ui16;
typedef hash_map<String16, String16> string_map;
typedef hash_map<int, int> int_map;
typedef int_map::iterator imi;
typedef int_map::const_iterator cimi;
typedef pair<imi, bool> P_imi_b;
typedef string_map::iterator smi;
typedef pair<smi, bool> P_smi_b;

TEST(chain_map_test, test_chain_map_node) {
    int_map::table_type::node_type node;
    node.m_element = make_tuple(6, 1);
    imi it(&node, nullptr);
    ASSERT_EQ(1, *it);
    ASSERT_EQ(&node, it.m_node);
    ASSERT_EQ(nullptr, it.m_table);
    string_map::table_type::node_type snode;
    snode.m_element = make_tuple(String16{"hello"}, String16{"hello"});
    smi sit(&snode, nullptr);
    ASSERT_EQ(5, sit->length());
    ASSERT_EQ(16, sit->capacity());
}

TEST(chain_map_test, test_const_iterator) {
    int_map map(10, 255);
    map[5] = 5;
    map[6] = 6;
    map[7] = 7;
    int_map::iterator mit = map.begin();
    ASSERT_EQ(5, *mit);
    const int_map const_map(move(map));
    int_map::const_iterator it = const_map.begin();
    ASSERT_EQ(5, *it);
    ++it;
    ASSERT_EQ(6, *it);
    ++it;
    ASSERT_EQ(7, *it);
    ++it;
    ASSERT_EQ(const_map.end(), it);
}

TEST(chain_map_test, test_const_node_equals) {
    int_map map(10, 150);
    map[15] = 10;
    map[10] = 9;
    map[9] = 19;
    imi it = map.begin();
    int_map::table_type::node_type *node = it.m_node;
    ASSERT_TRUE(map.begin().m_node == node);
}

TEST(chain_map_test, test_iterator_constructors) {
    int_map map(10, 150);
    map[15] = 10;
    map[10] = 9;
    const imi it = map.begin();
    imi it2(it);
    ASSERT_EQ(map.begin(), it2);
}

TEST(chain_map_test, test_iterator_equals) {
    int_map map(10, 15);
    map[15] = 10;
    map[10] = 9;
    imi it = map.begin();
    ++it;
    imi it2 = map.begin();
    ASSERT_FALSE(it == it2);
    ASSERT_TRUE(it != it2);
    it = it2;
    ASSERT_EQ(it, map.begin());
}

TEST(chain_map_test, test_ensure_capacity_holes) {
    int_map map(5, 50);
    map[1] = 1;
    map[6] = 6;
    map[11] = 11;
    ASSERT_EQ(5, map.capacity());
    map[16] = 16;
    ASSERT_EQ(10, map.capacity());
    map[21] = 21;
    ASSERT_EQ(10, map.capacity());
    map[26] = 26;
    ASSERT_EQ(20, map.capacity());
    ASSERT_EQ(20, map.capacity());
    ui16 expected_values_traverse[] = {1, 21, 26, 6, 11, 16};
    imi it = map.begin();
    for (ui16 i = 0; i < 6; i++) {
        ASSERT_EQ(expected_values_traverse[i], *it);
        ++it;
    }
    ASSERT_EQ(map.end(), it);
    map.clear();
    ASSERT_EQ(map.end(), map.begin());
    ASSERT_EQ(0, map.size());
    ASSERT_EQ(20, map.capacity());
}

TEST(chain_map_test, test_erase_cases) {
    int_map map(10, 255);
    imi it = map.insert(1, 1).first();
    map[11] = 11;
    map[21] = 21;
    map[31] = 31;
    map[2] = 2;
    it = map.erase(it);
    ASSERT_EQ(2, *it);
    it = map.end();
    map.erase(it);
    ASSERT_EQ(map.end(), it);
}

TEST(chain_map_test, test_constructor_params) {
    int_map map(10, 150);
    ASSERT_EQ(10, map.capacity());
    ASSERT_EQ(150, map.max_load());
    ASSERT_EQ(0, map.size());
    ASSERT_TRUE(map.empty());
}

TEST(chain_map_test, test_begin_returns_end_when_empty) {
    string_map map(10, 100);
    ASSERT_EQ(map.begin(), map.end());
}

TEST(chain_map_test, test_begin_end_const) {
    const string_map map(10, 100);
    ASSERT_EQ(map.begin(), map.end());
}

TEST(chain_map_test, test_insert_at_iterator_no_collision) {
    int_map map(5, 255);
    ui16 keys[] = {0, 1, 2, 3, 4};
    ui16 values[] = {0, 10, 20, 30, 40};
    P_imi_b r[] = {
            map.insert(0, 0),
            map.insert(1, 10),
            map.insert(2, 20),
            map.insert(3, 30),
            map.insert(4, 40)
    };
    ASSERT_EQ(5, map.size());
    for (ui16 i = 0; i < 5; i++) {
        ASSERT_TRUE(r[i].second());
        ASSERT_EQ(values[i], *r[i].first());
        ASSERT_EQ(values[i], map.at(keys[i]));
    }
    P_imi_b failed = map.insert(0, 10);
    ASSERT_FALSE(failed.second());
    ASSERT_EQ(0, *failed.first());
    imi it = r[0].first();
    ASSERT_EQ(it, map.begin());
    ++it;
    ASSERT_EQ(*r[1].first(), *it);
    ASSERT_EQ(r[1].first(), it);
    ++it;
    ASSERT_EQ(*r[2].first(), *it);
    ASSERT_EQ(r[2].first(), it);
    ++it;
    ASSERT_EQ(*r[3].first(), *it);
    ASSERT_EQ(r[3].first(), it);
    ++it;
    ASSERT_EQ(*r[4].first(), *it);
    ASSERT_EQ(r[4].first(), it);
    ++it;
    ASSERT_EQ(map.end(), it);
}

TEST(chain_map_test, test_insert_at_iterator_collision_resolution) {
    int_map map(5, 255);
    int keys[] = {
            0, 1, 2, 3, 4,
            5, 6, 7, 8, 9,
            10, 12, 15, 17, 20
    };
    int values[] = {
            0, 10, 20, 30, 40, 50, 60, 70, 80, 90,
            100, 120, 150, 170, 200
    };
    // ceil(2.55 * 5) = 13
    // Map will rehash before inserting 17
    P_imi_b r[15];
    for (ui16 i = 0; i < 15; i++) {
        r[i] = map.insert(keys[i], values[i]);
        ASSERT_TRUE(r[i].second());
        ASSERT_EQ(values[i], *r[i].first());
    }
    for (ui16 i = 0; i < 15; i++) {
        ASSERT_EQ(values[i], *r[i].first());
    }
    ASSERT_EQ(15, map.size());
    imi it = r[14].first();
    ASSERT_EQ(it, map.begin());
    int expected_values_traverse[] = {
            200, 0, 100, 10, 20, 120,
            30, 40, 50, 150, 60, 170, 70,
            80, 90
    };
    int expected_r_traverse[] = {
            14, 0, 10, 1, 2, 11,
            3, 4, 5, 12, 6, 13,
            7, 8, 9
    };
    for (ui16 i = 0; i < 15; i++) {
        ASSERT_EQ(expected_values_traverse[i], *it);
        ASSERT_EQ(expected_values_traverse[i], *r[expected_r_traverse[i]].first());
        ASSERT_EQ(r[expected_r_traverse[i]].first(), it);
        ++it;
    }
    ASSERT_EQ(map.end(), it);
}

TEST(chain_map_test, test_insert_or_assign) {
    string_map map(15, 255);
    String16 a1{"key1"};
    String16 a2{"key2"};
    String16 v1{"value1"};
    String16 v2{"value2"};
    String16 v3{"value3"};
    P_smi_b r1 = map.insert_or_assign(a1, v1);
    P_smi_b r2 = map.insert_or_assign(a2, v2);
    ASSERT_EQ(2, map.size());
    ASSERT_TRUE(r1.second());
    ASSERT_TRUE(r2.second());
    ASSERT_EQ(v1, map.at(a1));
    ASSERT_EQ(v2, map.at(a2));
    P_smi_b r3 = map.insert_or_assign(a1, v3);
    ASSERT_EQ(2, map.size());
    ASSERT_FALSE(r3.second());
    ASSERT_EQ(v3, *r3.first());
    ASSERT_EQ(v3, map.at(a1));
}

TEST(chain_map_test, test_erase_key_nothing) {
    string_map map(15, 255);
    String16 a{"key"};
    ASSERT_FALSE(map.erase(a));
    ASSERT_EQ(0, map.size());
}

TEST(chain_map_test, test_erase_key) {
    string_map map(15, 255);
    String16 a{"key"};
    String16 b{"val"};
    map.insert(a, b);
    ASSERT_EQ(1, map.size());
    ASSERT_TRUE(map.erase(a));
    ASSERT_EQ(0, map.size());
}

TEST(chain_map_test, test_erase_iterator) {
    int_map map(5, 255);
    // 0  1  _  3  _  _
    // 20       33
    // 40
    P_imi_b r0 = map.insert(0, 0);
    P_imi_b r1 = map.insert(1, 1);
    P_imi_b r3 = map.insert(3, 3);
    ASSERT_EQ(3, map.size());
    P_imi_b r20 = map.insert(20, 20);
    P_imi_b r33 = map.insert(33, 33);
    map.insert(40, 40);
    ASSERT_EQ(6, map.size());
    imi it = r1.first();
    it = map.erase(it);
    ASSERT_EQ(5, map.size());
    ASSERT_EQ(33, *it);
    ASSERT_EQ(it, r33.first());
    int_map::iterator prev_it = it;
    it = map.erase(it);
    ASSERT_EQ(4, map.size());
    ASSERT_EQ(3, *it);
    ASSERT_NE(prev_it, r3.first()); // iterator invalidated by erase
    ASSERT_EQ(*it, *r3.first());
    it = map.erase(it);
    ASSERT_EQ(3, map.size());
    ASSERT_EQ(map.end(), it);
    ASSERT_EQ(40, map.at(40));
    ASSERT_EQ(20, map.at(20));
    ASSERT_EQ(0, map.at(0));
    it = r20.first();
    prev_it = it;
    it = map.erase(it);
    ASSERT_EQ(2, map.size());
    ASSERT_EQ(0, *it);
    ASSERT_NE(prev_it, r0.first());
    ASSERT_EQ(0, *r0.first());
    it = map.erase(it);
    ASSERT_EQ(map.end(), it);
    ASSERT_EQ(1, map.size());
    ASSERT_EQ(40, *map.begin());
}

TEST(chain_map_test, test_contains_access_operator) {
    int_map map(5, 255);
    map[5] = 50;
    map[15] = 150;
    map[0] = 0;
    map[20] = 200;
    map[25] = 250;
    ASSERT_EQ(5, map.size());
    map.insert(3, 30);
    ASSERT_EQ(6, map.size());
    ASSERT_EQ(30, map.at(3));
    map[3] = 33;
    ASSERT_EQ(6, map.size());
    ASSERT_EQ(33, map.at(3));
    ASSERT_EQ(50, map[5]);
    ASSERT_EQ(150, map[15]);
    ASSERT_EQ(200, map[20]);
    ASSERT_EQ(250, map[25]);
    ASSERT_TRUE(map.contains(25));
    ASSERT_TRUE(map.contains(15));
    ASSERT_TRUE(map.contains(3));
    ASSERT_FALSE(map.contains(4));
    map[14] = 14;
    ASSERT_FALSE(map.contains(4));
    map[24] = 24;
    ASSERT_FALSE(map.contains(4));
    ASSERT_EQ(8, map.size());
    map[4] = 4;
    ASSERT_TRUE(map.contains(4));
    ASSERT_EQ(9, map.size());
}

TEST(chain_map_test, test_find) {
    int_map map(10, 255);
    map[16] = 1116;
    map[21] = 1211;
    map[71] = 1711;
    ASSERT_EQ(map.end(), map.find(15));
    ASSERT_EQ(map.end(), map.find(0));
    ASSERT_EQ(map.end(), map.find(6));
    ASSERT_EQ(map.end(), map.find(1));
    ASSERT_EQ(1116, *map.find(16));
    ASSERT_EQ(1711, *map.find(71));
    ASSERT_EQ(1211, *map.find(21));
    imi it = map.find(71);
    ++it;
    ASSERT_EQ(1211, *it);
    ++it;
    ASSERT_EQ(1116, *it);
    ++it;
    ASSERT_EQ(map.end(), it);
}

TEST(chain_map_test, test_erase_key_cases) {
    int_map map(10, 255);
    map[6] = 6;
    map[16] = 16;
    map[26] = 26;
    map[46] = 46;
    map[56] = 56;
    map[36] = 36;
    map[4] = 4;
    ui16 k = 14;
    ASSERT_FALSE(map.erase(k));
    k = 36;
    ASSERT_TRUE(map.erase(k));
    k = 26;
    ASSERT_TRUE(map.erase(k));
    k = 66;
    ASSERT_FALSE(map.erase(k));
}

TEST(chain_map_test, test_iterator_ctor) {
    int_map map(10, 255);
    map.insert(1, 5);
    map.insert(2, 10);
    map.insert(3, 15);
    imi i1 = map.begin();
    ASSERT_EQ(5, *i1);
    cimi ci1;
    const int_map const_map(move(map));
    cimi ci2 = move(const_map.begin());
    cimi ci3(ci2);
    ASSERT_EQ(ci2, ci3);
    string_map s_map(10, 255);
    s_map.insert(String16("key"), String16("val"));
    const string_map const_s_map(move(s_map));
    string_map::const_iterator s_ci1 = const_s_map.begin();
    ASSERT_EQ(3, s_ci1->length());
    ci2 = move(const_map.begin());
    ASSERT_EQ(5, *ci2);
    ci3 = move(const_map.begin());
    ci2 = ci3;
    ASSERT_EQ(5, *ci2);
}

TEST(chain_map_test, test_at_const) {
    int_map map(10, 255);
    map.insert(1, 5);
    map.insert(2, 10);
    map.insert(3, 15);
    const int_map const_map(move(map));
    const int v = 3;
    ASSERT_EQ(15, const_map.at(v));
    ASSERT_EQ(15, *const_map.find(v));
}

TEST(chain_map_test, test_move_assignment_op) {
    int_map map(10, 255);
    map.insert(1, 5);
    map.insert(2, 10);
    map.insert(3, 15);
    int_map map1(10, 255);
    map1 = move(map);
    ASSERT_EQ(3, map1.size());
    ASSERT_EQ(5, map1[1]);
    ASSERT_EQ(10, map1[2]);
    ASSERT_EQ(15, map1[3]);
}
