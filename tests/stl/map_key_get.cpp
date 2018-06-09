#include <gtest/gtest.h>

#include <wlib/stl/HashMap.h>
#include <wlib/stl/OpenMap.h>
#include <wlib/stl/TreeMap.h>

using namespace wlp;

TEST(chain_map_check, test_iterator_get_key) {
    hash_map<char *, int> map;
    static char key1[] = "first";
    static char key2[]=  "second";
    static char key3[] = "third";
    map.insert(key1, 1);
    map.insert(key2, 2);
    map.insert(key3, 3);
    int sum = 0;
    for (int i : map) {
        sum += i;
    }
    ASSERT_EQ(6, sum);
    for (auto it = map.begin(); it != map.end(); ++it) {
        sum -= map[it.key()];
    }
    ASSERT_EQ(0, sum);
}

TEST(open_map_check, test_iterator_get_key) {
    open_map<char *, int> map;
    static char key1[] = "first";
    static char key2[]=  "second";
    static char key3[] = "third";
    map.insert(key1, 1);
    map.insert(key2, 2);
    map.insert(key3, 3);
    int sum = 0;
    for (int i : map) {
        sum += i;
    }
    ASSERT_EQ(6, sum);
    for (auto it = map.begin(); it != map.end(); ++it) {
        sum -= map[it.key()];
    }
    ASSERT_EQ(0, sum);
}
