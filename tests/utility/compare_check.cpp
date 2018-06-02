#include <gtest/gtest.h>
#include <wlib/util/Compare.h>

using namespace wlp;

TEST(compare_check, check_is_any_of_ct) {
    bool true_value = is_one_of_ct<int, 8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12>::value;
    ASSERT_TRUE(true_value);
    bool false_value = is_one_of_ct<int, 8, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13>::value;
    ASSERT_FALSE(false_value);
}

TEST(compare_check, check_isnt_any_of_ct) {
    bool true_value = isnt_any_of_ct<int, 8, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13>::value;
    ASSERT_TRUE(true_value);
    bool false_value = isnt_any_of_ct<int, 8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12>::value;
    ASSERT_FALSE(false_value);
}

TEST(compare_check, check_is_any_of) {
    typedef is_one_of<int, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10> test_type;
    auto wrap = {5};
    int rt_value = *wrap.begin();
    bool true_value = test_type::apply(rt_value);
    ASSERT_TRUE(true_value);
    wrap = {11};
    rt_value = *wrap.begin();
    bool false_value = test_type::apply(rt_value);
    ASSERT_FALSE(false_value);
}

TEST(compare_check, check_isnt_any_of) {
    typedef isnt_any_of<int, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10> test_type;
    auto wrap = {11};
    int rt_value = *wrap.begin();
    bool true_value = test_type::apply(rt_value);
    ASSERT_TRUE(true_value);
    wrap = {5};
    rt_value = *wrap.begin();
    bool false_value = test_type::apply(rt_value);
    ASSERT_FALSE(false_value);
}
