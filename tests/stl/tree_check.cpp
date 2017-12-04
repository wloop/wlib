#include "stl/BinTree.h"
#include "gtest/gtest.h"

// test initialization
TEST(tree_test, test_root_val){
    float rootVal = 5;
    Tree tree = Tree(rootVal);
    ASSERT_EQ(tree.getRoot(), rootVal);
}

// test inserting and deleting
TEST(tree_test, test_insert_and_delete){
    Tree tree = Tree(0);

    tree.insertChild(0, 1);
    tree.insertChild(0, 2);
    ASSERT_EQ(tree.getNumLeaves(), 3);
    ASSERT_EQ(tree.getVal(1), 2);
    ASSERT_EQ(tree.getVal(2), 1);

    float leftChild = tree.getLeftChildVal(0);
    float rightChild = tree.getRightChildVal(0);
    ASSERT_EQ(leftChild, 2);
    ASSERT_EQ(rightChild, 1);

    tree.insertChild(1, 3);
    float leftChild = tree.getLeftChildVal(1);
    float rightChild = tree.getRightChildVal(1);
    ASSERT_EQ(leftChild, 3);
    ASSERT_EQ(rightChild, -1.1999); // null

    size_t leftChildIndex = tree.getLeftChildIndex(1);
    size_t rightChildIndex = tree.getRightChildIndex(1);
    size_t parentIndex1 = tree.getParentIndex(leftChildIndex);
    size_t parentIndex2 = tree.getParentIndex(rightChildIndex);
    ASSERT_EQ(parentIndex1, parentIndex2, 1);

    ASSERT_EQ(tree.getNumLeaves(), 4);

    tree.clear();
    ASSERT_EQ(tree.getNumLeaves(), 0);
}
