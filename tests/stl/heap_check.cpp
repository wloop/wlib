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

TEST(heap_test, test_heap_sort_comparator) {
    int array[] = {-5, 6, -1, 10, 16, 32};
    ArrayList<int> list(array, 6, 12);
    list.push_back(0);
    ReverseComparator<int> comparator;
    heap_sort(list, comparator);
    int expected[] = {32, 16, 10, 6, 0, -1, -5};
    for (size_type i = 0; i < 6; i++) {
        ASSERT_EQ(expected[i], list[i]);
    }
}

TEST(heap_test, test_push_heap) {
    ArrayList<int> heap(20);
    heap.push_back(5);
    heap.push_back(10);
    heap.push_back(1);
    make_heap(heap.begin(), heap.end());
    heap.push_back(15);
    push_heap(heap.begin(), heap.end());
    ASSERT_EQ(15, heap.front());
}

TEST(heap_test, test_make_heap) {
    ReverseComparator<int> comparator;
    ArrayList<int> heap(10);
    heap.push_back(5);
    heap.push_back(10);
    heap.push_back(1);
    heap.push_back(-5);
    make_heap(heap.begin(), heap.end(), comparator);
    ASSERT_EQ(-5, heap.front());
    pop_heap(heap.begin(), heap.end(), comparator);
    ASSERT_EQ(1, heap.front());
    ASSERT_EQ(-5, heap.back());
}

TEST(heap_test, test_make_heap_single) {
    ArrayList<int> heap(1);
    heap.push_back(5);
    make_heap(heap.begin(), heap.end());
    ASSERT_EQ(5, heap.back());
    ASSERT_EQ(5, heap.front());
    heap.clear();
    ASSERT_EQ(0, heap.size());
    heap.push_back(10);
    Comparator<int> comparator;
    make_heap(heap.begin(), heap.end(), comparator);
    ASSERT_EQ(10, heap.back());
    ASSERT_EQ(10, heap.front());
}

TEST(heap_test, test_heap_push) {
    ArrayHeap<int> heap(5);
    heap.push(5);
    const int v = 7;
    heap.push(v);
    ASSERT_EQ(2, heap.size());
    ASSERT_EQ(7, heap.top());
    heap.pop();
    ASSERT_EQ(5, heap.top());
    heap.pop();
    ASSERT_EQ(0, heap.size());
}

TEST(heap_test, test_move_constructor) {
    ArrayHeap<int> heap(5);
    heap.push(1);
    heap.push(-1);
    heap.push(5);
    heap.push(-5);
    ArrayHeap<int> heap0(move(heap));
    ASSERT_EQ(4, heap0.size());
    ASSERT_EQ(0, heap.size());
    ASSERT_EQ(5, heap0.capacity());
    ASSERT_EQ(0, heap.capacity());
    ASSERT_EQ(5, heap0.top());
    heap0.pop();
    ASSERT_EQ(1, heap0.top());
    heap0.pop();
    ASSERT_EQ(-1, heap0.top());
    heap0.pop();
    ASSERT_EQ(-5, heap0.top());
    heap0.pop();
    ASSERT_EQ(0, heap0.size());
    ASSERT_TRUE(heap.empty());
    ASSERT_TRUE(heap0.empty());
    ASSERT_EQ(nullptr, heap.get_array_list()->data());
}

TEST(heap_test, test_move_assignment_operator) {
    ArrayHeap<int> heap(5);
    heap.push(5);
    heap.push(2);
    heap.push(11);
    ArrayHeap<int> heap0(10);
    heap0.push(1);
    heap0.push(10);
    heap0 = move(heap);
    ASSERT_EQ(0, heap.size());
    ASSERT_EQ(0, heap.capacity());
    ASSERT_EQ(nullptr, heap.get_array_list()->data());
    ASSERT_EQ(3, heap0.size());
    ASSERT_EQ(5, heap0.capacity());
    ASSERT_EQ(11, heap0.top());
    heap0.pop();
    ASSERT_EQ(5, heap0.top());
    heap0.pop();
    ASSERT_EQ(2, heap0.top());
    heap0.pop();
    ASSERT_EQ(0, heap0.size());
}
