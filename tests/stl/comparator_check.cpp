#include <gtest/gtest.h>
#include <wlib/stl/Comparator.h>
#include <wlib/strings/String.h>

using namespace wlp;

TEST(comparator_test, test_int_comparison) {
    comparator<int> cmp;
    ASSERT_TRUE(cmp.__eq__(5, 5));
    ASSERT_FALSE(cmp.__eq__(5, 6));

    ASSERT_TRUE(cmp.__ne__(5, 6));
    ASSERT_FALSE(cmp.__ne__(5, 5));

    ASSERT_TRUE(cmp.__gt__(6, 5));
    ASSERT_FALSE(cmp.__gt__(5, 5));
    ASSERT_FALSE(cmp.__gt__(5, 6));

    ASSERT_TRUE(cmp.__lt__(5, 6));
    ASSERT_FALSE(cmp.__lt__(5, 5));
    ASSERT_FALSE(cmp.__lt__(6, 5));

    ASSERT_TRUE(cmp.__le__(5, 6));
    ASSERT_TRUE(cmp.__le__(5, 5));
    ASSERT_FALSE(cmp.__le__(6, 5));

    ASSERT_TRUE(cmp.__ge__(6, 5));
    ASSERT_TRUE(cmp.__ge__(5, 5));
    ASSERT_FALSE(cmp.__ge__(5, 6));
}

TEST(comparator_test, test_c_string_comparison) {
    char str1[] = "hello";
    char str2[] = "hellsalem";
    comparator<const char *> cmp;

    ASSERT_TRUE(cmp.__eq__(str1, str1));
    ASSERT_FALSE(cmp.__eq__(str1, str2));

    ASSERT_TRUE(cmp.__ne__(str1, str2));
    ASSERT_FALSE(cmp.__ne__(str1, str1));

    ASSERT_TRUE(cmp.__gt__(str2, str1));
    ASSERT_FALSE(cmp.__gt__(str1, str2));
    ASSERT_FALSE(cmp.__gt__(str1, str1));

    ASSERT_TRUE(cmp.__lt__(str1, str2));
    ASSERT_FALSE(cmp.__lt__(str2, str1));
    ASSERT_FALSE(cmp.__lt__(str1, str1));

    ASSERT_TRUE(cmp.__le__(str1, str2));
    ASSERT_TRUE(cmp.__le__(str1, str1));
    ASSERT_FALSE(cmp.__le__(str2, str1));

    ASSERT_TRUE(cmp.__ge__(str2, str1));
    ASSERT_FALSE(cmp.__ge__(str1, str2));
    ASSERT_TRUE(cmp.__ge__(str1, str1));
}

TEST(comparator_test, test_static_string_comparator) {
    String8 str1("hello");
    String8 str2("hellsalem");
    comparator<String8> cmp;

    ASSERT_TRUE(cmp.__eq__(str1, str1));
    ASSERT_FALSE(cmp.__eq__(str1, str2));

    ASSERT_TRUE(cmp.__ne__(str1, str2));
    ASSERT_FALSE(cmp.__ne__(str1, str1));

    ASSERT_TRUE(cmp.__gt__(str2, str1));
    ASSERT_FALSE(cmp.__gt__(str1, str2));
    ASSERT_FALSE(cmp.__gt__(str1, str1));

    ASSERT_TRUE(cmp.__lt__(str1, str2));
    ASSERT_FALSE(cmp.__lt__(str2, str1));
    ASSERT_FALSE(cmp.__lt__(str1, str1));

    ASSERT_TRUE(cmp.__le__(str1, str2));
    ASSERT_TRUE(cmp.__le__(str1, str1));
    ASSERT_FALSE(cmp.__le__(str2, str1));

    ASSERT_TRUE(cmp.__ge__(str2, str1));
    ASSERT_FALSE(cmp.__ge__(str1, str2));
    ASSERT_TRUE(cmp.__ge__(str1, str1));
}

TEST(comparator_test, test_reverse_comparator) {
    ReverseComparator<int> cmp;
    ASSERT_TRUE(cmp.__eq__(5, 5));
    ASSERT_FALSE(cmp.__eq__(5, 6));

    ASSERT_TRUE(cmp.__ne__(5, 6));
    ASSERT_FALSE(cmp.__ne__(5, 5));

    ASSERT_TRUE(cmp.__lt__(6, 5));
    ASSERT_FALSE(cmp.__lt__(5, 5));
    ASSERT_FALSE(cmp.__lt__(5, 6));

    ASSERT_TRUE(cmp.__gt__(5, 6));
    ASSERT_FALSE(cmp.__gt__(5, 5));
    ASSERT_FALSE(cmp.__gt__(6, 5));

    ASSERT_TRUE(cmp.__ge__(5, 6));
    ASSERT_TRUE(cmp.__ge__(5, 5));
    ASSERT_FALSE(cmp.__ge__(6, 5));

    ASSERT_TRUE(cmp.__le__(6, 5));
    ASSERT_TRUE(cmp.__le__(5, 5));
    ASSERT_FALSE(cmp.__le__(5, 6));
}
