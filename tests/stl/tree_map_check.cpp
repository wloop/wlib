#include <gtest/gtest.h>

#include <wlib/stl/TreeMap.h>
#include <wlib/strings/String.h>

using namespace wlp;

TEST(tree_map, insert_or_assign_rvalue) {
    typedef dynamic_string string;
    typedef tree_map<string, string> dstringmap;

    dstringmap map;

    string key1("key1");
    string val1("val1");
    string val2("val2");

    map.insert(key1, val1);
    auto ret = map.insert_or_assign(move(key1), move(val2));

    ASSERT_FALSE(ret.second());
    ASSERT_STREQ("val2", ret.first()->c_str());
}

TEST(tree_map, insert_rvalue) {
    typedef dynamic_string string;
    typedef tree_map<string, string> dstringmap;

    dstringmap map;

    string key("key");
    string val("val");

    map.insert(move(key), move(val));
    ASSERT_EQ(0u, key.length());
    ASSERT_EQ(0u, val.length());
    ASSERT_STREQ("", key.c_str());
    ASSERT_STREQ("", val.c_str());

    string akey("key");
    ASSERT_FALSE(map.end() == map.find(akey));
    ASSERT_STREQ("val", map[akey].c_str());
}

TEST(tree_map, test_iterator_get_key) {
    tree_map<char *, int> map;
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
