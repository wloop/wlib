#include <gtest/gtest.h>
#include <wlib/strings/String.h>

using namespace wlp;


TEST(dynamic_string_tests, constructor_tests) {
    dynamic_string string1;
    dynamic_string string2("HELLo WORld!");
    dynamic_string string3(string1);
    ASSERT_EQ(string2, "HELLo WORld!");
    ASSERT_TRUE(string1.empty());
    ASSERT_EQ(12, string2.length());
    ASSERT_EQ(0, string3.length()); //This assert fails

    string2.clear();

    ASSERT_TRUE(string2.empty());
}

TEST(dynamic_string_tests, assignment_tests) {
    dynamic_string string1;
    dynamic_string string2("Waterloop");
    dynamic_string string3("is awesome");

    string3 = string2;
    string2 = string1;

    ASSERT_EQ(string3, "Waterloop");
    ASSERT_EQ(string2, "");
    ASSERT_EQ(string1, "");

    ASSERT_EQ(string3.length(), 9);
    ASSERT_EQ(string2.length(), 0);
}

TEST(dynamic_string_tests, character_access_tests) {
    dynamic_string string1("Hello my name is BOB");

    EXPECT_TRUE(string1[4] == 'o');
    EXPECT_TRUE(string1.at(3) == 'l');
    EXPECT_TRUE(string1.front() == 'H');
    EXPECT_TRUE(string1.back() == 'B');
}

TEST(dynamic_string_tests, append_operator_tests) {
    dynamic_string string1("Hey");
    dynamic_string string2("Water");
    dynamic_string string3("Loo");

    char array1[] = {'y', 'o', 'o', 'o', '\0'};

    ASSERT_STREQ((string1 += "Water").c_str(), "HeyWater");
    ASSERT_STREQ((string1 += "!").c_str(), "HeyWater!");
    ASSERT_STREQ((string1 += array1).c_str(), "HeyWater!yooo");
    ASSERT_STREQ((string2 += string3).c_str(), "WaterLoo");
}

TEST(dynamic_string_tests, append_tests) {
    dynamic_string string1("Hey");
    dynamic_string string2("Water");
    dynamic_string string3("Loo");

    char array1[] = {'y', 'o', 'o', 'o', '\0'};

    ASSERT_STREQ((string1.append("Water")).c_str(), "HeyWater");
    ASSERT_STREQ((string1.append("!")).c_str(), "HeyWater!");
    ASSERT_STREQ((string1.append(array1)).c_str(), "HeyWater!yooo");
    ASSERT_STREQ((string2.append(string3)).c_str(), "WaterLoo");
    string3.push_back('k');
    ASSERT_STREQ(string3.c_str(), "Look");
    string2.push_back('x');
    ASSERT_STREQ(string2.c_str(), "WaterLoox");
}

TEST(dynamic_string_tests, substring_tests) {
    dynamic_string string1("Heeelllloooo");
    dynamic_string string2;

    ASSERT_STREQ("Heee", (string1.substr(0, 4)).c_str());
    ASSERT_STREQ("el", (string1.substr(3, 2)).c_str());
    ASSERT_STREQ("", (string2.substr(0, 3)).c_str());
}

TEST(dynamic_string_tests, addition_operator_tests) {
    dynamic_string string1("boiii");
    dynamic_string string2("mannns");

    ASSERT_STREQ("boiiimannns", (string1 + string2).c_str());
    ASSERT_STREQ("boiii!", (string1 + '!').c_str());
    ASSERT_STREQ("!mannns", ('!' + string2).c_str());
    ASSERT_STREQ("mannnsnothot", (string2 + "nothot").c_str());
    ASSERT_STREQ("!!!!boiii", ("!!!!" + string1).c_str());
}

TEST(dynamic_string_tests, equality_operator_tests) {
    dynamic_string string1("boiii");
    dynamic_string string2("mannns");
    dynamic_string string3("x");

    ASSERT_TRUE(string1 == string1);
    ASSERT_FALSE(string2 == string1);
    ASSERT_TRUE(string1 == "boiii");
    ASSERT_TRUE("mannns" == string2);
    ASSERT_TRUE('x' == string3);
    ASSERT_TRUE(string3 == 'x');
}

TEST(dynamic_string_tests, erase_popback_tests) {
    dynamic_string string1("lastone,soclosetofinished");

    string1.erase(0);
    ASSERT_STREQ("astone,soclosetofinished", string1.c_str());
    string1.erase(6);
    ASSERT_STREQ("astonesoclosetofinished", string1.c_str());

    string1.pop_back();

    ASSERT_STREQ("astonesoclosetofinishe", string1.c_str());
}

TEST(dynamic_string_tests, move_tests) {
    char str[] = "Tis an unweeded garden that grows to seed; things rank and gross in Nature possess";
    dynamic_string s1(str);
    dynamic_string s2(move(s1));
    ASSERT_EQ(0, s1.length());
    ASSERT_STREQ("", s1.c_str());
    ASSERT_EQ(strlen(str), s2.length());
    ASSERT_STREQ(str, s2.c_str());
    dynamic_string s3("To sleep; to die");
    s3 = move(s2);
    ASSERT_EQ(0, s2.length());
    ASSERT_STREQ("", s2.c_str());
    ASSERT_EQ(strlen(str), s3.length());
    ASSERT_STREQ(str, s3.c_str());
}

TEST(dynamic_string_tests, resize_length_set) {
    char teststr[] = "Your empire needs you!";
    size_type length = static_cast<size_type>(strlen(teststr));

    dynamic_string str("hello");
    ASSERT_STREQ("hello", str.c_str());
    ASSERT_EQ(strlen("hello"), str.length());

    str.resize(length);
    ASSERT_EQ(0, str.length());
    ASSERT_STREQ("", str.c_str());

    strcpy(str.c_str(), "Your empire needs you!");
    str.length_set(length);
    ASSERT_STREQ(teststr, str.c_str());
    ASSERT_EQ(length, str.length());
}

