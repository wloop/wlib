#include "../../lib/wlib/stl/BinTree.h"
#include "gtest/gtest.h"


// test initialization
TEST(tree_test, test_root_val){
    float rootVal = 5;
    Tree tree = Tree(rootVal);
    ASSERT_EQ(tree.GetRoot(), rootVal);
}

// test inserting and deleting
TEST(tree_test, test_insert_and_delete){
    Tree tree = Tree(0);

    tree.InsertChildWithParent(0, 1);
    tree.InsertChildWithParent(0, 2);
    ASSERT_EQ(tree.GetNumLeaves(), 3);

    float *childrenVals = tree.GetChildrenValsFromParent(0);
    ASSERT_EQ(childrenVals[0], 2);
    ASSERT_EQ(childrenVals[1], 1);

    tree.InsertChildWithParent(1, 3);
    childrenVals = tree.GetChildrenValsFromParent(1);
    ASSERT_EQ(childrenVals[0], 3);

    size_t *childrenIndex = tree.GetChildrenIndexFromParent(1);
    size_t parentIndex = tree.GetParentIndexFromChild(childrenIndex[0]);
    ASSERT_EQ(parentIndex, 1);

    ASSERT_EQ(tree.GetNumLeaves(), 4);

    tree.Clear();
    ASSERT_EQ(tree.GetNumLeaves(), 0);
}
