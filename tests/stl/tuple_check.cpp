#include <gtest/gtest.h>
#include <wlib/stl/Tuple.h>
#include <wlib/stl/OpenMap.h>

using namespace wlp;

TEST(tuple_test, test_tuple_create) {
    tuple<int, float, const char *> tuple(34, 2.1f, "hello");
    int v1 = get<0>(tuple);
    float v2 = get<1>(tuple);
    const char *v3 = get<2>(tuple);
    ASSERT_EQ(34, v1);
    ASSERT_FLOAT_EQ(2.1f, v2);
    ASSERT_STREQ("hello", v3);
    get<0>(tuple) = 25;
    ASSERT_EQ(25, get<0>(tuple));
    get<1>(tuple) = 0.5556f;
    ASSERT_FLOAT_EQ(0.5556f, get<1>(tuple));
}

TEST(tuple_test, test_tuple_default) {
    tuple<int, float, int> tuple;
    ASSERT_EQ(0, get<0>(tuple));
    ASSERT_FLOAT_EQ(0, get<1>(tuple));
    ASSERT_EQ(0, get<2>(tuple));
}

TEST(tuple_test, test_make_tuple) {
    int x = 12;
    auto tuple = make_tuple(45, x, 1.246);
    ASSERT_EQ(45, get<0>(tuple));
    ASSERT_EQ(12, get<1>(tuple));
    ASSERT_DOUBLE_EQ(1.246, get<2>(tuple));
}

TEST(tuple_test, test_tuple_size) {
    auto tuple = make_tuple(1, 2, 3, 4, 5, 6);
    ASSERT_EQ(6, get_tuple_size(tuple));
    ASSERT_EQ(6, tuple_size<decltype(tuple)>::value);
}

TEST(tuple_test, test_tuple_cat) {
    auto tupleA = make_tuple(45, 65.55, "hello");
    auto tupleB = make_tuple(77.886, "goodbye", 23);
    auto tuple = tuple_cat_pair(tupleA, tupleB);
    ASSERT_EQ(6, tuple_size<decltype(tuple)>::value);
    ASSERT_EQ(6, get_tuple_size(tuple));
    ASSERT_EQ(45, get<0>(tuple));
    ASSERT_DOUBLE_EQ(65.55, get<1>(tuple));
    ASSERT_STREQ("hello", get<2>(tuple));
    ASSERT_DOUBLE_EQ(77.886, get<3>(tuple));
    ASSERT_STREQ("goodbye", get<4>(tuple));
    ASSERT_EQ(23, get<5>(tuple));
}

TEST(tuple_test, test_tuple_multi_cat) {
    auto tupleA = make_tuple(1);
    auto tupleB = make_tuple(56.65, 43.32);
    auto tupleC = make_tuple("string", "string");
    auto tuple = tuple_cat(tupleA, tupleB, tupleC);
    ASSERT_EQ(5, tuple_size<decltype(tuple)>::value);
    ASSERT_EQ(1, get<0>(tuple));
    ASSERT_DOUBLE_EQ(56.65, get<1>(tuple));
    ASSERT_DOUBLE_EQ(43.32, get<2>(tuple));
    ASSERT_STREQ("string", get<3>(tuple));
    ASSERT_STREQ("string", get<4>(tuple));
    wlp::tuple<int> i_tuple_1(1);
    wlp::tuple<int> i_tuple_2(2);
    wlp::tuple<int> i_tuple_3(3);
    wlp::tuple<int> i_tuple_4(4);
    wlp::tuple<int, int, int, int> i_tuple = tuple_cat(i_tuple_1, i_tuple_2, i_tuple_3, i_tuple_4);
    ASSERT_EQ(1, get<0>(i_tuple));
    ASSERT_EQ(2, get<1>(i_tuple));
    ASSERT_EQ(3, get<2>(i_tuple));
    ASSERT_EQ(4, get<3>(i_tuple));
}

TEST(tuple_test, test_forward_as_tuple) {
    int a, d = 5;
    double b;
    char c;
    tuple<int &, double &, char &, int &&> tuple = forward_as_tuple(a, b, c, move(d));
    get<0>(tuple) = 10;
    ASSERT_EQ(10, a);
    get<1>(tuple) = 12.3;
    ASSERT_DOUBLE_EQ(12.3, b);
    get<2>(tuple) = 'h';
    ASSERT_EQ('h', c);
    ASSERT_EQ(5, get<3>(tuple));
    get<3>(tuple) = 5555;
    ASSERT_EQ(5555, d);
}

TEST(tuple_test, test_assign_pair) {
    tuple<int, int> tuple = make_tuple(12, 34);
    ASSERT_EQ(12, get<0>(tuple));
    ASSERT_EQ(34, get<1>(tuple));
    pair<int, int> pair(16, 19);
    tuple = pair;
    ASSERT_EQ(16, get<0>(tuple));
    ASSERT_EQ(19, get<1>(tuple));
    open_map<int, int> int_map(10, 61);
    open_map<int, int>::iterator it;
    bool b;
    tie(it, b) = int_map.insert(5, 1);
    ASSERT_EQ(1, *it);
    ++it;
    ASSERT_EQ(int_map.end(), it);
    ASSERT_TRUE(b);
    tie(it, b) = int_map.insert(5, 10);
    ASSERT_EQ(1, *it);
    ASSERT_FALSE(b);
    ++it;
    ASSERT_EQ(int_map.end(), it);
    tie(ignore, b) = int_map.insert(6, 4);
    ASSERT_TRUE(b);
    tie(ignore, b) = int_map.insert(6, 10);
    ASSERT_FALSE(b);
}

TEST(tuple_test, test_tuple_assign) {
    auto tupleA = make_tuple(5, 6, 7, 8);
    auto tupleB = make_tuple(0, 0, 0, 0);
    tupleB = tupleA;
    ASSERT_EQ(5, get<0>(tupleB));
    ASSERT_EQ(6, get<1>(tupleB));
    ASSERT_EQ(7, get<2>(tupleB));
    ASSERT_EQ(8, get<3>(tupleB));
}

TEST(tuple_test, test_tuple_tie) {
    int a, b;
    char c;
    double d;
    auto tuple = make_tuple(55, 66, 'g', 66.55);
    tie(a, b, c, d) = tuple;
    ASSERT_EQ(55, a);
    ASSERT_EQ(66, b);
    ASSERT_EQ('g', c);
    ASSERT_DOUBLE_EQ(66.55, d);
}

TEST(tuple_test, test_repeat_tuple_size) {
    RepeatTuple<int, 5> rt_1;
    size_t value = tuple_size<decltype(rt_1)>::value;
    ASSERT_EQ(value, 5u);
    ASSERT_NE(value, 6u);
    RepeatTuple<int, 6> rt_2;
    value = tuple_size<decltype(rt_2)>::value;
    ASSERT_EQ(value, 6u);
    ASSERT_NE(value, 5u);
}

TEST(tuple_test, test_repeat_tuple_values) {
    RepeatTuple<bool, 4> rt_1 = make_tuple(true, false, false, true);
    ASSERT_TRUE(get<0>(rt_1));
    ASSERT_FALSE(get<1>(rt_1));
    ASSERT_FALSE(get<2>(rt_1));
    ASSERT_TRUE(get<3>(rt_1));
}
