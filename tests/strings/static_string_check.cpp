/**
 * @file static_string_check.cpp
 * @brief Unit Testing for StaticString class present in Wlib library
 *
 * @author Deep Dhillon
 * @date November 4, 2017
 * @bug No known bugs
 */

#include "gtest/gtest.h"
#include "strings/StaticString.h"

TEST(length_and_size, static_string_ctr_test){
    StaticString<8> string1{"helloooo"};    // text given
    StaticString<8> string2;                // no text
    StaticString<8> string3{string1};       // string object given

    // check for length and size
    EXPECT_TRUE(string1.capacity() == 8 && string1.length() == 8);
    EXPECT_TRUE(string2.capacity() == 8 && string2.length() == 0);
    EXPECT_TRUE(string3.capacity() == 8 && string3.length() == 8);
}

TEST(chars_compare, static_string_ctr_test){
    StaticString<8> string1{"helloooo"};    // text given
    StaticString<8> string2;                // no text
    StaticString<8> string3{string1};       // string object given

    // check for chars
    EXPECT_TRUE(strncmp(string1.c_str(), "helloooo", 8) == 0);
    EXPECT_TRUE(strncmp(string2.c_str(), "", 0) == 0);
    EXPECT_TRUE(strncmp(string3.c_str(), "helloooo", 8) == 0);
}

TEST(add_operators, static_string_concat_test){
    StaticString<16> string1{"deep"};
    StaticString<16> string2{"bye"};
    StaticString<16> string3{"hi"};
    StaticString<16> string4{"yo"};
    StaticString<16> string5{"yooooooooooooooo"};
    char char1 = '7';
    char array1[] = {'h', 'e', 'l', 'l', '\0'};
    char array2[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '\0'};

    // object to object addition
    EXPECT_TRUE(strcmp((string1 + string2).c_str(), "deepbye") == 0);
    EXPECT_TRUE(strcmp((string1 + string2 + string4).c_str(), "deepbyeyo") == 0);

    // object plus character array addition
    EXPECT_TRUE(strcmp((string3 + "brooooooo").c_str(), "hibrooooooo") == 0);
    EXPECT_TRUE(strcmp(("1234567890000" + string2).c_str(), "1234567890000bye") == 0);

    // char array
    EXPECT_TRUE(strcmp((array1 + string1).c_str(), "helldeep") == 0);
    EXPECT_TRUE(strcmp((string1 + array1 + array1).c_str(), "deephellhell") == 0);

    // individual car
    EXPECT_TRUE(strcmp((char1 + string1).c_str(), "7deep") == 0);
    EXPECT_TRUE(strcmp((string1 + char1 + char1).c_str(), "deep77") == 0);

    // string beingadded is longer that what we can hold
    EXPECT_TRUE(strcmp((string1 + string5).c_str(), "deepyooooooooooo") == 0);
    EXPECT_TRUE(strcmp((string5 + "boyishero00000").c_str(), "yooooooooooooooo") == 0);
    EXPECT_TRUE(strcmp((string1 + 'c').c_str(), "deepc") == 0);
    EXPECT_TRUE(strcmp((string1 + string1 + array2).c_str(), "deepdeep12345678") == 0);

}

TEST(concat_operator, static_string_concat_test){
    StaticString<16> string1{"deep"};
    StaticString<16> string2{"bye"};
    StaticString<16> string3{"hi"};
    StaticString<16> string4{"yo"};
    StaticString<16> string5{"yooooooooooooooo"};
    char char1 = '7';
    char array1[] = {'h', 'e', 'l', 'l', '\0'};
    char array2[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '\0'};

    // object to object append
    EXPECT_TRUE(strcmp((string1 += string2).c_str(), "deepbye") == 0);
    EXPECT_TRUE(strcmp((string1 += string2 += string4).c_str(), "deepbyebyeyo") == 0);

    // object plus character array append
    EXPECT_TRUE(strcmp((string3 += "brooooooo").c_str(), "hibrooooooo") == 0);
    EXPECT_TRUE(strcmp((string4 += "1234567890000").c_str(), "yo1234567890000") == 0);

    // char array
    EXPECT_TRUE(strcmp((string2 += array1).c_str(), "byeyohell") == 0);

    // individual char
    EXPECT_TRUE(strcmp((string3 += char1).c_str(), "hibrooooooo7") == 0);

    // string being added is longer that what we can hold
    EXPECT_TRUE(strcmp((string2 += string5).c_str(), "byeyohellyoooooo") == 0);
    EXPECT_TRUE(strcmp((string5 += "boyishero00000").c_str(), "yooooooooooooooo") == 0);
    EXPECT_TRUE(strcmp((string5 += 'c').c_str(), "yooooooooooooooo") == 0);
    EXPECT_TRUE(strcmp((string3 += 'd').c_str(), "hibrooooooo7d") == 0);
}

TEST(concat_append, static_string_concat_test){
    StaticString<16> string1{"deep"};
    StaticString<16> string2{"bye"};
    StaticString<16> string3{"hi"};
    StaticString<16> string4{"yo"};
    StaticString<16> string5{"yooooooooooooooo"};
    char char1 = '7';
    char array1[] = {'h', 'e', 'l', 'l', '\0'};
    char array2[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '\0'};

    // object to object append
    EXPECT_TRUE(strcmp((string1.append(string2)).c_str(), "deepbye") == 0);
    EXPECT_TRUE(strcmp((string1.append(string2).append(string4)).c_str(), "deepbyebyeyo") == 0);

    // object plus character array append
    EXPECT_TRUE(strcmp((string3.append("brooooooo")).c_str(), "hibrooooooo") == 0);
    EXPECT_TRUE(strcmp((string4.append("1234567890000")).c_str(), "yo1234567890000") == 0);

    // char array
    EXPECT_TRUE(strcmp((string2.append(array1)).c_str(), "byehell") == 0);
    EXPECT_TRUE(strcmp((string2.append(array1).append(array1)).c_str(), "byehellhellhell") == 0);


    // individual char
    EXPECT_TRUE(strcmp((string2.append(char1)).c_str(), "byehellhellhell7") == 0);

    // string being added is longer that what we can hold
    EXPECT_TRUE(strcmp((string2.append(string5)).c_str(), "byehellhellhell7") == 0);
    EXPECT_TRUE(strcmp((string5.append("boyishero00000")).c_str(), "yooooooooooooooo") == 0);
    EXPECT_TRUE(strcmp((string5.append('c')).c_str(), "yooooooooooooooo") == 0);
    EXPECT_TRUE(strcmp((string3.append('d').append(array2)).c_str(), "hibroooooood1234") == 0);
}

TEST(concat_push_back, static_string_concat_test){
    StaticString<16> string1{"deep"};
    StaticString<16> string2{"bye"};
    StaticString<16> string3{"hi"};
    StaticString<16> string4{"yo"};
    StaticString<16> string5{"yooooooooooooooo"};
    char char1 = '7';
    char array1[] = {'h', 'e', 'l', 'l', '\0'};
    char array2[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '\0'};

    // object to object push back
    EXPECT_TRUE(strcmp((string1.push_back(string2)).c_str(), "deepbye") == 0);
    EXPECT_TRUE(strcmp((string1.push_back(string2).push_back(string4)).c_str(), "deepbyebyeyo") == 0);

    // object plus character array push back
    EXPECT_TRUE(strcmp((string3.push_back("brooooooo")).c_str(), "hibrooooooo") == 0);
    EXPECT_TRUE(strcmp((string4.push_back("1234567890000")).c_str(), "yo1234567890000") == 0);

    // char array
    EXPECT_TRUE(strcmp((string2.push_back(array1)).c_str(), "byehell") == 0);
    EXPECT_TRUE(strcmp((string2.push_back(array1).push_back(array1)).c_str(), "byehellhellhell") == 0);


    // individual char
    EXPECT_TRUE(strcmp((string2.push_back(char1)).c_str(), "byehellhellhell7") == 0);

    // string being added is longer that what we can hold
    EXPECT_TRUE(strcmp((string2.push_back(string5)).c_str(), "byehellhellhell7") == 0);
    EXPECT_TRUE(strcmp((string5.push_back("boyishero00000")).c_str(), "yooooooooooooooo") == 0);
    EXPECT_TRUE(strcmp((string5.push_back('c')).c_str(), "yooooooooooooooo") == 0);
    EXPECT_TRUE(strcmp((string3.push_back('d').append(array2)).c_str(), "hibroooooood1234") == 0);
}

TEST(equalTo_operator, static_string_equalTo_test){
    StaticString<16> string1{"deep"};
    StaticString<16> string2{"bye"};
    StaticString<16> string3{"hi"};
    StaticString<16> string4{"y"};

    EXPECT_FALSE(string1 == string2);
    EXPECT_TRUE(string1 == string1);
    EXPECT_TRUE(string1 == "deep");
    EXPECT_TRUE(string4 == 'y');
    EXPECT_TRUE("deep" == string1);
    EXPECT_TRUE('y' == string4);
}

TEST(compare_methods, static_string_compare_test){
    StaticString<16> string1{"deep"};
    StaticString<16> string2{"bye"};
    StaticString<16> string3{"hi"};
    StaticString<16> string4{"y"};
    StaticString<16> string5{"ye"};

    EXPECT_TRUE(string1.compare(string4) < 0);
    EXPECT_TRUE(string3.compare(string2) > 0);
    EXPECT_TRUE(string4.compare(string5) < 0);
    EXPECT_TRUE(string1.compare(string1) == 0);
    EXPECT_TRUE(string1.compare("dee") > 0);
    EXPECT_TRUE(string2.compare('a') > 0);
}

TEST(access_chars, static_string_access_test){
    StaticString<16> string1{"deep"};
    StaticString<16> string2{"bye"};
    StaticString<16> string3{"hi"};
    StaticString<16> string4{"y"};

    EXPECT_TRUE(string1[0] == 'd');
    EXPECT_TRUE(string2.front() == 'b');
    EXPECT_TRUE(string3.end() == 'i');
    EXPECT_TRUE(string2[2] == 'e');
    EXPECT_TRUE(string4.at(0) == 'y');
}

TEST(clear_string, static_string_clear_test){
    StaticString<16> string1{"deep"};
    StaticString<16> string2{"bye"};
    StaticString<16> string3{"hi"};
    StaticString<16> string4{"y"};

    string1.clear();
    string2.clear();
    string3.clear();
    string4.clear();

    EXPECT_TRUE(string1.length() == 0 && strcmp(string1.c_str(), "") == 0
                && string1.empty() && string1.capacity() == 16);
    EXPECT_TRUE(string2.length() == 0 && strcmp(string2.c_str(), "") == 0
                && string1.empty() && string1.capacity() == 16);
    EXPECT_TRUE(string3.length() == 0 && strcmp(string3.c_str(), "") == 0
                && string1.empty() && string1.capacity() == 16);
    EXPECT_TRUE(string4.length() == 0 && strcmp(string4.c_str(), "") == 0
                && string1.empty() && string1.capacity() == 16);
}

TEST(assign_operator, static_string_assign_test){
    StaticString<16> string1{"deep"};
    StaticString<16> string2{"bye"};
    StaticString<16> string3{"hi"};
    StaticString<16> string4{"y"};

    string1 = string4;
    string2 = "deep2";
    string3 = 'c';

    EXPECT_TRUE(string1 == string4);
    EXPECT_TRUE(string2 == "deep2");
    EXPECT_TRUE(string3 == 'c');
}

TEST(substring, static_string_substring_test) {
    StaticString<16> string1{"deep"};

    EXPECT_TRUE(string1.substr(0, 2) == "de");
    EXPECT_TRUE(string1.substr(0, 4) == "deep");
    EXPECT_TRUE(string1.substr(2, 1) == "e");
}
