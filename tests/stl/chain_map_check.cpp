#include "gtest/gtest.h"
#include "stl/ChainMap.h"

#include "../template_defs.h"

using namespace wlp;

typedef StaticString<16> string16;
typedef uint16_t ui16;
typedef ChainHashMap<string16, string16> string_map;
typedef ChainHashMap<ui16, ui16> int_map;
typedef int_map::iterator imi;
typedef Pair<imi, bool> P_imi_b;
typedef string_map::iterator smi;
typedef Pair<smi, bool> P_smi_b;

TEST(chain_map_test, test_chain_map_node) {
    int_map::node_type node;
    node.m_key = 6;
    node.m_val = 1;
    imi it(&node, nullptr);
    ASSERT_EQ(1, *it);
    ASSERT_EQ(node, *it.m_current);
    ASSERT_EQ(nullptr, it.m_hash_map);
    string_map::node_type snode;
    snode.m_key = string16{"hello"};
    snode.m_val = string16{"hello"};
    smi sit(&snode, nullptr);
    ASSERT_EQ(5, sit->length());
    ASSERT_EQ(16, sit->capacity());
}

TEST(chain_map_test, test_const_iterator) {
    int_map map(10, 255);
    map[5] = 5;
    map[6] = 6;
    map[7] = 7;
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
    int_map::node_type node = *it.m_current;
    ASSERT_TRUE(*map.begin().m_current == node);
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
    map[16] = 16;
    map[21] = 21;
    map[26] = 26;
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
    map.erase(it);
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

TEST(chain_map_test, test_constructor_allocator) {
    string_map map(10, 100);
    const size_t expected = sizeof(string_map::node_type);
    ASSERT_EQ(expected, map.get_node_allocator()->GetBlockSize());
    ASSERT_EQ(expected * 10, map.get_node_allocator()->GetPoolSize());
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
        ASSERT_EQ(values[i], *map.at(keys[i]));
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
    ui16 keys[] = {
            0, 1, 2, 3, 4,
            5, 6, 7, 8, 9,
            10, 12, 15, 17, 20
    };
    ui16 values[] = {
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
    ui16 expected_values_traverse[] = {
            200, 0, 100, 10, 20, 120,
            30, 40, 50, 150, 60, 170, 70,
            80, 90
    };
    ui16 expected_r_traverse[] = {
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
    string16 a1{"key1"};
    string16 a2{"key2"};
    string16 v1{"value1"};
    string16 v2{"value2"};
    string16 v3{"value3"};
    P_smi_b r1 = map.insert_or_assign(a1, v1);
    P_smi_b r2 = map.insert_or_assign(a2, v2);
    ASSERT_EQ(2, map.size());
    ASSERT_TRUE(r1.second());
    ASSERT_TRUE(r2.second());
    ASSERT_EQ(v1, *map.at(a1));
    ASSERT_EQ(v2, *map.at(a2));
    P_smi_b r3 = map.insert_or_assign(a1, v3);
    ASSERT_EQ(2, map.size());
    ASSERT_FALSE(r3.second());
    ASSERT_EQ(v3, *r3.first());
    ASSERT_EQ(v3, *map.at(a1));
}

TEST(chain_map_test, test_erase_key_nothing) {
    string_map map(15, 255);
    string16 a{"key"};
    ASSERT_FALSE(map.erase(a));
    ASSERT_EQ(0, map.size());
}

TEST(chain_map_test, test_erase_key) {
    string_map map(15, 255);
    string16 a{"key"};
    string16 b{"val"};
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
    map.erase(it);
    ASSERT_EQ(5, map.size());
    ASSERT_EQ(33, *it);
    ASSERT_EQ(it, r33.first());
    map.erase(it);
    ASSERT_EQ(4, map.size());
    ASSERT_EQ(3, *it);
    ASSERT_NE(it, r3.first()); // iterator invalidated by erase
    ASSERT_EQ(*it, *r3.first());
    map.erase(it);
    ASSERT_EQ(3, map.size());
    ASSERT_EQ(map.end(), it);
    ASSERT_EQ(40, *map.at(40));
    ASSERT_EQ(20, *map.at(20));
    ASSERT_EQ(0, *map.at(0));
    ASSERT_EQ(map.end(), map.at(1));
    ASSERT_EQ(map.end(), map.at(3));
    ASSERT_EQ(map.end(), map.at(33));
    it = r20.first();
    map.erase(it);
    ASSERT_EQ(2, map.size());
    ASSERT_EQ(0, *it);
    ASSERT_NE(it, r0.first());
    ASSERT_EQ(0, *r0.first());
    map.erase(it);
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
    ASSERT_EQ(30, *map.at(3));
    map[3] = 33;
    ASSERT_EQ(6, map.size());
    ASSERT_EQ(33, *map.at(3));
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
