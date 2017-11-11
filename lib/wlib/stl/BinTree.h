#ifndef WLIB_TREE_H
#define WLIB_TREE_H

#include <stdint.h>
#include <vector>

using namespace std; //TODO: delete after vector implementation


/* Implement a (Binary) Tree using vector
 * TOOO: implement with our vector class instead of std
  */
class Tree {
    public:
        // Constructor - add a root to the tree
        Tree(int64_t rootVal) {
            m_tree.push_back(rootVal);
            m_numLeaves = 1;
            m_kNullVal = -1;
            m_kTreeOrder = 2;
        }

        // Return tree root if exists
        int64_t GetRoot(void) { 
            if (m_tree.size() > 0) {
              return m_tree[0];
            } 
            return m_kNullVal;
        }

        // Returns index of node from value
        // Assumes unique values in tree
        int64_t GetIndexFromVal(int64_t val) {
            int64_t len = m_tree.size();
            for (int64_t i = 0; i < len; ++i) {
                if (m_tree[i] == val) {
                    return i;
                }
            }
            return m_kNullVal; // not found

        }

    
        // Return the value at index in the vector
        int64_t GetValFromIndex(int64_t index) {
            if (index < m_tree.size()) {
                return m_tree[index];
            } else {
                return m_kNullVal; 
            }
        }

        // Insert a child node given the index of the parent
        // Return the child's index
        int64_t InsertChildWithParent(int64_t parentIndex, int64_t childVal) {
            int64_t childIndex = m_kTreeOrder*parentIndex + 1;
            int64_t len = m_tree.size();

            // insert null nodes to fill gap if needed
            while (childIndex > len) {
                m_tree.insert(m_tree.begin() + len, m_kNullVal);
                len++;
            }

            m_tree.insert(m_tree.begin() + childIndex, childVal);
            m_numLeaves++;
            return childIndex;
        }

        // Remove node from tree with index
        int64_t DeleteNodeAtIndex(int64_t index) {
            m_tree.erase(m_tree.begin() + index);
            m_numLeaves--;
            return 1;
        }
  
        // Clear the vector
        void Clear() {
            m_tree.clear();
            m_numLeaves = 0;
        }

        // Get the number of leaves in the tree
        int64_t GetNumLeaves() {
          return m_numLeaves;
        }

    private:
        // vector to store the tree
        vector<int64_t> m_tree;
        // number of valid (non-empty) nodes in the tree
        int64_t m_numLeaves;
        // tree order (2 for binary)
        int64_t m_kTreeOrder;
        //TODO: find better null value
        int64_t m_kNullVal;

};

#endif
