#include <gtest/gtest.h>
#include <wlib/stl/OpenSet.h>
#include <wlib/stl/HashSet.h>
#include <wlib/stl/ArrayList.h>
#include <wlib/stl/ArrayHeap.h>
#include <wlib/stl/OpenMap.h>
#include <wlib/stl/RedBlackTree.h>
#include <wlib/stl/Tuple.h>
#include <wlib/stl/TreeMap.h>

#include "../test_helper.h"

using namespace wlp;

typedef tuple<char, int> _rb_element;
typedef MapGetKey<char, int> _rb_key;
typedef MapGetVal<char, int> _rb_val;

typedef typename tree<_rb_element, char, int, _rb_key, _rb_val>::iterator rbi;
typedef tree<_rb_element, char, int, _rb_key, _rb_val> rb_tree;

TEST(rb_tree_test, test_insert_iterator_order) {
    char keys[] = {'g', 'h', 'j', 'k', 'y', 'c', 'd', 'q', 'w'};
    int vals[] = {5, 1, 0, 9, -1, -4, 12, 10, -66};
    open_map<char, int> val_map(20);
    rb_tree tree;
    for (size_t i = 0; i < 9; i++) {
        val_map.insert(keys[i], vals[i]);
        pair<rbi, bool> res = tree.insert_unique(make_tuple(keys[i], vals[i]));
        ASSERT_TRUE(res.second());
        ASSERT_EQ(vals[i], *res.first());
    }
    ASSERT_EQ(9u, tree.size());
    array_list<char> key_list(keys, 9);
    heap_sort(key_list);
    rbi it = tree.begin();
    for (size_t i = 0; i < key_list.size(); i++) {
        char expected_key = key_list[i];
        int expected_val = val_map[expected_key];
        char key = get<0>(it.m_node->m_element);
        int val = *it;
        ASSERT_EQ(expected_key, key);
        ASSERT_EQ(expected_val, val);
        ++it;
    }
    ASSERT_TRUE(tree.end() == it);
}

TEST(rb_tree_test, test_insert_unique_find) {
    rb_tree tree;
    char keys[] = {
            'o', 'y', '8', 'D', 'B', '9', 'e', 'B', '2', '8',
            'P', 'p', 'k', 'j', 'd', 'M', 'm', 'c', 'Z', 'h',
            'd', 'T', 'p', 'o', 'H', 's', 'Y', 'p', 'd', 'L',
            'I', '3', '6', '6', 'L', 'o', '3', 'd', 's', 'G'
    };
    int vals[40];
    open_map<char, int> val_map(80);
    open_set<char> key_set(80);
    for (int i = 0; i < 40; i++) {
        vals[i] = random_int();
        pair<rbi, bool> res = tree.insert_unique(make_tuple(keys[i], vals[i]));
        if (key_set.contains(keys[i])) {
            ASSERT_FALSE(res.second());
        } else {
            ASSERT_TRUE(res.second());
        }
        key_set.insert(keys[i]);
        val_map.insert(keys[i], vals[i]);
        ASSERT_EQ(val_map[keys[i]], *res.first());
    }
    array_list<char> key_list(key_set.size());
    for (open_set<char>::iterator it = key_set.begin(); it != key_set.end(); ++it) {
        key_list.push_back(*it);
    }
    reverse_comparator<char> cmp;
    heap_sort(key_list, cmp);
    for (int i = 0; i < 40; i++) {
        pair<rbi, bool> res = tree.insert_unique(make_tuple(keys[i], vals[i]));
        ASSERT_FALSE(res.second());
        ASSERT_EQ(val_map[keys[i]], *res.first());
        ASSERT_EQ(keys[i], get<0>(res.first().m_node->m_element));
    }
    ASSERT_EQ(key_set.size(), tree.size());
    int count = 0;
    array_list<char>::iterator kit = key_list.begin();
    for (rbi it = --tree.end();; --it) {
        ASSERT_EQ(*kit, get<0>(it.m_node->m_element));
        ASSERT_EQ(val_map[*kit], *it);
        ++kit;
        if (it == tree.begin()) {
            break;
        }
        count++;
        if (count >= 40) {
            FAIL();
        }
    }
    for (int i = 0; i < 40; i++) {
        ASSERT_EQ(val_map[keys[i]], *tree.find(keys[i]));
    }
    for (open_set<char>::iterator it = key_set.begin(); it != key_set.end(); ++it) {
        ASSERT_EQ(1u, tree.erase(*it));
    }
    ASSERT_EQ(0u, tree.size());
}

TEST(rb_tree_test, test_insert_equal_and_range) {
    char keys[] = {'a', 'a', 'a', 'b', 'b', 'c', 'c', 'c', 'c', 'd'};
    int values[] = {5, 6, 7, 8, 9, 10, 10, 11, 12, 13};
    hash_set<int> val_set(20);
    size_t cnt = 10;
    rb_tree tree;
    for (size_t i = 0; i < cnt; i++) {
        rbi it = tree.insert_equal(make_tuple(keys[i], values[i]));
        ASSERT_EQ(values[i], *it);
        ASSERT_EQ(keys[i], get<0>(it.m_node->m_element));
        val_set.insert(values[i]);
    }
    char ukeys[] = {'a', 'b', 'c', 'd'};
    unsigned int counts[] = {3, 2, 4, 1};
    for (int i = 0; i < 4; i++) {
        ASSERT_EQ(counts[i], tree.count(ukeys[i]));
    }
    bool at_repeat = false;
    for (int i = 0; i < 4; i++) {
        char ukey = ukeys[i];
        pair<rbi, rbi> eq_range = tree.equal_range(ukey);
        for (rbi it = eq_range.first(); it != eq_range.second(); ++it) {
            int val = *it;
            if (val == 10) {
                if (at_repeat) {
                    continue;
                }
                at_repeat = true;
            }
            ASSERT_TRUE(val_set.contains(val));
            ASSERT_TRUE(val_set.erase(val));
        }
    }
    ASSERT_TRUE(val_set.empty());
    for (int i = 0; i < 4; i++) {
        char ukey = ukeys[i];
        size_t erased = tree.erase(ukey);
        ASSERT_EQ(counts[i], erased);
    }
    ASSERT_EQ(0u, tree.size());
    ASSERT_TRUE(tree.empty());
    ASSERT_TRUE(tree.begin() == tree.end());
}

