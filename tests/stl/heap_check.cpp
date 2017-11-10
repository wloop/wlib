#include "gtest/gtest.h"

#include "stl/ArrayHeap.h"
#include "../template_defs.h"

using namespace wlp;

TEST(heap_test, test_heap_push_pop) {
    ArrayHeap<int> heap;
    heap.push(5);
    heap.push(10);
    heap.push(1);
    heap.push(-1);
    heap.push(3);
    heap.push(-5);
    ASSERT_EQ(6, heap.size());
    ASSERT_EQ(10, heap.top());
    heap.pop();
    ASSERT_EQ(5, heap.top());
    heap.pop();
    ASSERT_EQ(3, heap.top());
    heap.pop();
    ASSERT_EQ(3, heap.size());
    ASSERT_EQ(1, heap.top());
    heap.pop();
    ASSERT_EQ(-1, heap.top());
    heap.pop();
    ASSERT_EQ(-5, heap.top());
    heap.pop();
    ASSERT_EQ(0, heap.size());
}

TEST(heap_test, test_heap_sort) {
    ArrayList<int> list;
    list.push_back(5);
    list.push_back(10);
    list.push_back(-1);
    list.push_back(9);
    list.push_back(4);
    list.push_back(2);
    list.push_back(7);
    heap_sort(list);
    int expected[] = {-1, 2, 4, 5, 7, 9, 10};
    for (ArrayList<int>::size_type i = 0; i < list.size(); ++i) {
        ASSERT_EQ(expected[i], list[i]);
    }
}
