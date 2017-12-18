#include "gtest/gtest.h"

#include "strings/String.h"

#include "stl/Comparator.h"
#include "stl/HashMap.h"
#include "stl/OpenMap.h"
#include "stl/TreeMap.h"
#include "stl/ArrayList.h"
#include "stl/HashSet.h"
#include "stl/OpenSet.h"
#include "stl/TreeSet.h"
#include "stl/LinkedList.h"

using namespace wlp;

TEST(concept_checks, check_comparator_concept) {
    bool c = comparator_concept<Comparator<int>, int>::value;
    ASSERT_TRUE(c);
    c = comparator_concept<Hash<int, int>, int>::value;
    ASSERT_FALSE(c);
}

TEST(concept_checks, check_is_comparator) {
    ASSERT_TRUE((is_comparator<Comparator<const char *>, const char *>()));
    ASSERT_FALSE((is_comparator<Equal<const char *>, const char *>()));
    ASSERT_FALSE((is_comparator<Hash<const char *, uint16_t>, const char *>()));
    ASSERT_TRUE((is_comparator<ReverseComparator<char>, char>()));
    ASSERT_FALSE((is_comparator<HashMap<uint16_t, uint16_t>, uint16_t>()));
}

TEST(concept_checks, check_has_size_val_type) {
    bool c;
    c = has_size_type<int>::value;
    ASSERT_FALSE(c);
    c = has_size_type<Pair<int, int>>::value;
    ASSERT_FALSE(c);
    c = has_val_type<int>::value;
    ASSERT_FALSE(c);
    c = has_val_type<Pair<int, int>>::value;
    ASSERT_FALSE(c);
    c = has_size_type<ArrayList<int>>::value;
    ASSERT_TRUE(c);
    c = has_size_type<HashMap<int, int>::iterator>::value;
    ASSERT_TRUE(c);
    c = has_val_type<HashMap<int, int>>::value;
    ASSERT_TRUE(c);
    c = has_val_type<ArrayList<int>::iterator>::value;
    ASSERT_TRUE(c);
}

TEST(concept_checks, check_random_access_iterator_concept) {
    bool c;
    c = random_access_iterator_concept<int>::value;
    ASSERT_FALSE(c);
    c = random_access_iterator_concept<ArrayList<int>>::value;
    ASSERT_FALSE(c);
    c = random_access_iterator_concept<HashMap<int, int>::iterator>::value;
    ASSERT_FALSE(c);
    c = random_access_iterator_concept<ArrayList<int>::iterator>::value;
    ASSERT_TRUE(c);
    c = random_access_iterator_concept<ArrayList<int>::const_iterator>::value;
    ASSERT_TRUE(c);

    ASSERT_TRUE((is_random_access_iterator<ArrayList<int>::iterator>()));
    ASSERT_TRUE((is_random_access_iterator<ArrayList<int>::const_iterator>()));

    ASSERT_FALSE((is_random_access_iterator<HashMap<int, int>::iterator>()));
    ASSERT_FALSE((is_random_access_iterator<HashMap<int, int>::const_iterator>()));
    ASSERT_FALSE((is_random_access_iterator<OpenHashMap<int, int>::iterator>()));
    ASSERT_FALSE((is_random_access_iterator<OpenHashMap<int, int>::const_iterator>()));
}

TEST(concept_checks, check_forward_iterator_concept) {
    ASSERT_TRUE((is_iterator<HashMap<int, int>::iterator>()));
    ASSERT_TRUE((is_iterator<HashMap<int, int>::const_iterator>()));
    ASSERT_TRUE((is_iterator<OpenHashMap<int, int>::iterator>()));
    ASSERT_TRUE((is_iterator<OpenHashMap<int, int>::const_iterator>()));
    ASSERT_TRUE((is_iterator<ArrayList<int>::iterator>()));
    ASSERT_TRUE((is_iterator<ArrayList<int>::const_iterator>()));

    ASSERT_FALSE((is_iterator<ArrayList<int>>()));
    ASSERT_FALSE((is_iterator<Comparator<int>>()));
}

TEST(concept_checks, check_map_concept) {
    ASSERT_TRUE((is_map<HashMap<int, int>>()));
    ASSERT_TRUE((is_map<OpenHashMap<int, int>>()));
    ASSERT_TRUE((is_map<TreeMap<int, int>>()));

    ASSERT_FALSE((is_map<int>()));
    ASSERT_FALSE((is_map<ArrayList<int>>()));
}

TEST(concept_checks, check_set_concept) {
    ASSERT_FALSE((is_set<ArrayList<int>>()));
    ASSERT_FALSE((is_set<int>()));

    ASSERT_TRUE((is_set<OpenHashSet<int>>()));
    ASSERT_TRUE((is_set<HashSet<int>>()));
    ASSERT_TRUE((is_set<TreeSet<int>>()));
}

TEST(concept_checks, check_list_concept) {
    ASSERT_FALSE((is_list<HashMap<int, int>>()));
    ASSERT_FALSE((is_list<int>()));
    ASSERT_FALSE((is_list<Comparator<int>>()));

    ASSERT_TRUE((is_list<ArrayList<int>>()));
    ASSERT_TRUE((is_list<LinkedList<int>>()));
}

TEST(concept_checks, check_string_concept) {
    ASSERT_FALSE((is_string<ArrayList<char>>()));
    ASSERT_FALSE((is_string<const char *>()));
    ASSERT_FALSE((is_string<HashMap<char, char>>()));

    ASSERT_TRUE((is_string<StaticString<8>>()));
    ASSERT_TRUE((is_string<StaticString<32>>()));
    ASSERT_TRUE((is_string<DynamicString>()));
}
