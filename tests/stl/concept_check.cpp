#include "gtest/gtest.h"

#include "stl/Concept.h"
#include "stl/Comparator.h"
#include "stl/Hash.h"
#include "stl/ChainMap.h"
#include "stl/ArrayList.h"

using namespace wlp;

TEST(concept_checks, check_comparator_concept) {
    bool c = comparator_concept<Comparator<int>, bool(const int &, const int &)>::value;
    ASSERT_TRUE(c);
    c = comparator_concept<Hash<int, int>,  bool(const int &, const int &)>::value;
    ASSERT_FALSE(c);
}

TEST(concept_checks, check_is_comparator) {
    ASSERT_TRUE((is_comparator<Comparator<const char *>, const char *>()));
    ASSERT_FALSE((is_comparator<Equal<const char *>, const char *>()));
    ASSERT_FALSE((is_comparator<Hash<const char *, uint16_t>, const char *>()));
    ASSERT_TRUE((is_comparator<ReverseComparator<char>, char>()));
    ASSERT_FALSE((is_comparator<ChainHashMap<uint16_t, uint16_t>, uint16_t>()));
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
    c = has_size_type<ChainHashMap<int, int>::iterator>::value;
    ASSERT_TRUE(c);
    c = has_val_type<ChainHashMap<int, int>>::value;
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
    c = random_access_iterator_concept<ChainHashMap<int, int>::iterator>::value;
    ASSERT_FALSE(c);
    c = random_access_iterator_concept<ArrayList<int>::iterator>::value;
    ASSERT_TRUE(c);
    c = random_access_iterator_concept<ArrayList<int>::const_iterator>::value;
    ASSERT_TRUE(c);
}
