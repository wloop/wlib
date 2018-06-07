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
    ASSERT_EQ(0, key.length());
    ASSERT_EQ(0, val.length());
    ASSERT_STREQ("", key.c_str());
    ASSERT_STREQ("", val.c_str());

    string akey("key");
    ASSERT_FALSE(map.end() == map.find(akey));
    ASSERT_STREQ("val", map[akey].c_str());
}
