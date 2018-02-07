#include <gtest/gtest.h>
#include <wlib/stl/UniquePtr.h>

using namespace wlp;

static int __constructs;
static int __deconstructs;
static int __assignments;

void __reset_test() {
    __constructs = 0;
    __deconstructs = 0;
    __assignments = 0;
}

struct __TestObject {

    int value = 0;

    __TestObject() { ++__constructs; }

    __TestObject(int val) : value(val) { ++__constructs; }

    ~__TestObject() { ++__deconstructs; }

    template<typename U>
    __TestObject &operator=(U &&u) {
        value = forward<int>(u.value);
        ++__assignments;
        return *this;
    }

};

TEST(unique_ptr_test, test_constructor_access) {
    UniquePtr<int> int_ptr = UniquePtr<int>(malloc<int>());
    *int_ptr = 5;
    ASSERT_EQ(*int_ptr, 5);
    bool bool_val = !!int_ptr;
    ASSERT_TRUE(bool_val);
    int *ptr = int_ptr.release();
    bool_val = !!int_ptr;
    ASSERT_FALSE(bool_val);
    ASSERT_EQ(nullptr, int_ptr.get());
    ASSERT_EQ(5, *ptr);
    int_ptr = UniquePtr<int>(ptr);
    ASSERT_EQ(5, *int_ptr);
    bool_val = !!int_ptr;
    ASSERT_TRUE(bool_val);
    int_ptr.reset();
    bool_val = !!int_ptr;
    ASSERT_FALSE(bool_val);
}

TEST(unique_ptr_test, test_default_ctor) {
    UniquePtr<const char *> string_ptr = UniquePtr<const char *>();
    ASSERT_FALSE(!!string_ptr);
    ASSERT_EQ(nullptr, string_ptr.get());
}

TEST(unique_ptr_test, test_custom_deleter_move_ctor) {
    __reset_test();
    UniquePtr<__TestObject> ptr = UniquePtr<__TestObject>(malloc<__TestObject>());
    ASSERT_EQ(1, __constructs);
    ptr->value = 10;
    ASSERT_EQ(10, ptr->value);
    ptr.reset();
    ASSERT_EQ(1, __deconstructs);
}

TEST(unique_ptr_test, test_deleter_ctor) {
    __reset_test();
    UniquePtr<const char *> cstr_ptr = UniquePtr<const char *>(malloc<const char *>());
    const char *string = "Stars, hide your fires; Let not "
            "light see my black and deep desires";
    *cstr_ptr = string;
    ASSERT_STREQ(string, *cstr_ptr);
    cstr_ptr.reset(malloc<const char *>());
    cstr_ptr = UniquePtr<const char *>(malloc<const char *>());
}

TEST(unique_ptr_test, test_array_ptr) {
    __reset_test();
    using obj = __TestObject;
    UniquePtr<obj[]> arr = UniquePtr<obj[]>(malloc<obj[]>(5));
    // malloc should call constructor
    ASSERT_EQ(5, __constructs);
    int values[] = {1, 2, 3, 4, 5};
    for (size_type i = 0; i < 5; i++) {
        // explicit operator call due to clion parsing issues
        // array subscript syntax is still valid
        arr.operator[](i) = obj(values[i]);
    }
    ASSERT_EQ(5, __assignments);
    ASSERT_EQ(10, __constructs);
    ASSERT_EQ(5, __deconstructs);
    for (size_type i = 0; i < 5; i++) {
        ASSERT_EQ(values[i], arr.operator[](i).value);
    }
    arr.reset(malloc<obj>());
    ASSERT_EQ(10, __deconstructs);
    ASSERT_EQ(11, __constructs);
    arr.operator[](0).value = 10;
    obj *ptr = arr.release();
    free<obj>(ptr);
    ASSERT_EQ(11, __deconstructs);
}

TEST(unique_ptr_test, test_comparison_operators) {
    uint32_t *twice16 = malloc<uint32_t>();
    uint16_t *first = reinterpret_cast<uint16_t *>(twice16);
    uint16_t *second = (first + 1);
    ASSERT_LT(first, second); // first < second
    uint16_t *l_first = first; // first == l_first
    // careful not to let unique ptr free these
    using intptr = UniquePtr<uint16_t>;
    intptr first_ptr = intptr(first);
    intptr second_ptr = intptr(second);
    intptr l_first_ptr = intptr(l_first);
    ASSERT_TRUE(first_ptr < second_ptr);
    ASSERT_TRUE(l_first_ptr < second_ptr);
    ASSERT_FALSE(first_ptr < l_first_ptr);
    ASSERT_FALSE(l_first_ptr < first_ptr);
    ASSERT_TRUE(first_ptr <= second_ptr);
    ASSERT_TRUE(l_first_ptr <= second_ptr);
    ASSERT_TRUE(l_first_ptr <= first_ptr);
    ASSERT_TRUE(first_ptr <= l_first_ptr);
    ASSERT_FALSE(second_ptr <= first_ptr);
    ASSERT_FALSE(second_ptr <= l_first_ptr);
    ASSERT_TRUE(second_ptr > first_ptr);
    ASSERT_FALSE(first_ptr > l_first_ptr);
    ASSERT_TRUE(second_ptr > l_first_ptr);
    ASSERT_FALSE(first_ptr > second_ptr);
    ASSERT_TRUE(first_ptr >= l_first_ptr);
    ASSERT_TRUE(second_ptr >= l_first_ptr);
    ASSERT_FALSE(first_ptr >= second_ptr);
    ASSERT_TRUE(l_first_ptr == first_ptr);
    ASSERT_FALSE(l_first_ptr == second_ptr);
    ASSERT_TRUE(l_first_ptr != second_ptr);
    ASSERT_FALSE(l_first_ptr != first_ptr);
    // release and manually free
    first_ptr.release();
    second_ptr.release();
    l_first_ptr.release();
    free<uint32_t>(twice16);
}
