#ifndef WLIB_TREE_H
#define WLIB_TREE_H

#include <stdint.h>
#include <vector>

using namespace std; //TODO: delete after vector implementation

const int64_t TREE_ORDER = 2;
const int64_t nullVal = -1; //TODO: find better null value

/* Implement a (Binary) Tree using Vector class
 * Will be using our implemented vector class once complete instead of std 
 * TOOO: Style guide + documentation, implement with our vector class 
  */
class Tree {
  private:
    // vector to store the tree
    vector<int64_t> tree;
    // number of valid (non-empty) nodes in the tree
    int64_t numLeaves = 0;

  public:
    // Constructor - add a root to the tree
    Tree(int64_t rootVal){
      tree.push_back(rootVal);
      numLeaves++;
    }

    // Return tree root if exists
    int64_t getRoot(void){
      if (tree.size() > 0) {
        return tree[0];
      } 
      return nullVal;
    }

    // Prints the value at vector index
    void printValAtIndex(int64_t index){
      if (index < tree.size()){
        cout << tree[index] << endl; 
      }
    }
    
    // Returns index of node from value
    // Assumes unique values in tree
    int64_t getIndexFromVal(int64_t val){
      int64_t len = tree.size();
      for (int64_t i = 0; i < len; ++i) {
        if (tree[i] == val){
          return i;
        }
      }
      return nullVal; // not found
    }
    
    // Return the value at index in the vector
    int64_t getValFromIndex(int64_t index){
      if (index < tree.size()){
        return tree[index];
      } else {
        return nullVal; 
      }
    }

    // Insert a child node given the index of the parent
    // Return the child's index
    int64_t insertChildWithParent(int64_t parentIndex, int64_t childVal){
      int64_t childIndex = TREE_ORDER*parentIndex + 1;
      int64_t len = tree.size();
      printf("Size: %lu, Inserted at index: %lld\n", len, childIndex);

      // insert null nodes to fill gap if needed
      while (childIndex > len) {
        tree.insert(tree.begin() + len, nullVal);
        len++;
      }

      tree.insert(tree.begin() + childIndex, childVal);
      numLeaves++;
      return childIndex;
    }

    // Remove node from tree with index
    int64_t deleteNodeAtIndex(int64_t index){
      tree.erase(tree.begin() + index);
      numLeaves--;
      return 1;
    }
  
    // Clear the vector
    void clear(){
      tree.clear();
      numLeaves = 0;
    }

    // Get the number of leaves in the tree
    int64_t getNumLeaves(){
      return numLeaves;
    }

};

#endif
