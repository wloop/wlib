#include "gtest/gtest.h"
#include "strings/DynamicString.h"

using namespace wlp;


TEST(dynamic_string_tests, constructor_tests) {
    DynamicString string1;
    DynamicString string2("HELLo WORld!");
    DynamicString string3(string1);
    ASSERT_EQ(string2, "HELLo WORld!");
    ASSERT_TRUE(string1.empty());
    ASSERT_EQ(12, string2.length());
    ASSERT_EQ(0, string3.length()); //This assert fails

    string2.clear();

    ASSERT_TRUE(string2.empty());
}

TEST(dynamic_string_tests, assignment_tests) {
    DynamicString string1;
    DynamicString string2("Waterloop");
    DynamicString string3("is awesome");

    string3 = string2;
    string2 = string1;

    ASSERT_EQ(string3, "Waterloop");
    ASSERT_EQ(string2, "");
    ASSERT_EQ(string1, "");

    ASSERT_EQ(string3.length(), 9);
    ASSERT_EQ(string2.length(), 0);
}

TEST(dynamic_string_tests, character_access_tests) {
    DynamicString string1("Hello my name is BOB");

    EXPECT_TRUE(string1[4] == 'o');
    EXPECT_TRUE(string1.at(3) == 'l');
    EXPECT_TRUE(string1.front() == 'H');
    EXPECT_TRUE(string1.end() == 'B');
}

TEST(dynamic_string_tests, append_operator_tests) {
    DynamicString string1("Hey");
    DynamicString string2("Water");
    DynamicString string3("Loo");

    char array1[] = {'y', 'o', 'o', 'o', '\0'};

    ASSERT_STREQ((string1 += "Water").c_str(), "HeyWater");
    ASSERT_STREQ((string1 += "!").c_str(), "HeyWater!");
    ASSERT_STREQ((string1 += array1).c_str(), "HeyWater!yooo");
    ASSERT_STREQ((string2 += string3).c_str(), "WaterLoo");
}

TEST(dynamic_string_tests, append_tests) {
    DynamicString string1("Hey");
    DynamicString string2("Water");
    DynamicString string3("Loo");

    char array1[] = {'y', 'o', 'o', 'o', '\0'};

    ASSERT_STREQ((string1.append("Water")).c_str(), "HeyWater");
    ASSERT_STREQ((string1.append("!")).c_str(), "HeyWater!");
    ASSERT_STREQ((string1.append(array1)).c_str(), "HeyWater!yooo");
    ASSERT_STREQ((string2.append(string3)).c_str(), "WaterLoo");
    ASSERT_STREQ((string3.push_back('k')).c_str(), "Look");
    ASSERT_STREQ((string2.push_back('x')).c_str(), "WaterLoox");
}

TEST(dynamic_string_tests, substring_tests) {
    DynamicString string1("Heeelllloooo");
    DynamicString string2;

    ASSERT_STREQ("Heee", (string1.substr(0, 4)).c_str());
    ASSERT_STREQ("el", (string1.substr(3, 2)).c_str());
    ASSERT_STREQ("", (string2.substr(0, 3)).c_str());
}

TEST(dynamic_string_tests, addition_operator_tests) {
    DynamicString string1("boiii");
    DynamicString string2("mannns");

    ASSERT_STREQ("boiiimannns", (string1 + string2).c_str());
    ASSERT_STREQ("boiii!", (string1 + '!').c_str());
    ASSERT_STREQ("!mannns", ('!' + string2).c_str());
    ASSERT_STREQ("mannnsnothot", (string2 + "nothot").c_str());
    ASSERT_STREQ("!!!!boiii", ("!!!!" + string1).c_str());
}

TEST(dynamic_string_tests, equality_operator_tests) {
    DynamicString string1("boiii");
    DynamicString string2("mannns");
    DynamicString string3("x");

    ASSERT_TRUE(string1 == string1);
    ASSERT_FALSE(string2 == string1);
    ASSERT_TRUE(string1 == "boiii");
    ASSERT_TRUE("mannns" == string2);
    ASSERT_TRUE('x' == string3);
    ASSERT_TRUE(string3 == 'x');
}

TEST(dynamic_string_tests, erase_popback_tests) {
    DynamicString string1("lastone,soclosetofinished");

    ASSERT_STREQ("astone,soclosetofinished", (string1.erase(0)).c_str());
    ASSERT_STREQ("astonesoclosetofinished", (string1.erase(6)).c_str());

    string1.pop_back();

    ASSERT_STREQ("astonesoclosetofinishe", string1.c_str());
}
