#include <gtest/gtest.h>
#include <wlib/utility/Compare.h>

using namespace wlp;

TEST(compare_check, check_is_any_of) {
    bool true_value = is_one_of<int, 8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12>::value;
    ASSERT_TRUE(true_value);
    bool false_value = is_one_of<int, 8, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13>::value;
    ASSERT_FALSE(false_value);
}

TEST(compare_check, check_isnt_any_of) {
    bool true_value = isnt_any_of<int, 8, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13>::value;
    ASSERT_TRUE(true_value);
    bool false_value = isnt_any_of<int, 8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12>::value;
    ASSERT_FALSE(false_value);
}
