#ifndef WLIB_TREE_H
#define WLIB_TREE_H

#include <stdint.h>

// TODO: delete below lines after vector implementation
#include <vector>
using namespace std;

/* Implement a (Binary) Tree using vector
 * TOOO: implement with our vector class instead of std
  */
class Tree {
    public:
        // Constructor - add a root to the tree
        Tree(float rootVal) {
            m_tree.push_back(rootVal);
            m_numLeaves = 1; // number of valid (non-empty) nodes
            m_kNullVal = -1.1999; // value for empty nodes
            m_kTreeOrder = 2; // children per parent (2 for binary)
        }

        // Return tree root if exists
        float GetRoot(void) { 
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
            return -1; // not found
        }
    
        // Return the value at index in the vector
        float GetValFromIndex(int64_t index) {
            if (index < m_tree.size()) {
                return m_tree[index];
            } else {
                return m_kNullVal; 
            }
        }

        // Insert a child node given the index of the parent
        // Return the child's index
        int64_t InsertChildWithParent(int64_t parentIndex, float childVal) {
            int64_t childIndex = m_kTreeOrder*parentIndex + 1;
            int64_t len = m_tree.size();

            // insert null nodes to fill gap if needed
            while (childIndex > len) {
                m_tree.insert(m_tree.begin() + len, m_kNullVal);
                len++;
            }

            // insert the node and increase valid node count
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
        vector<float> m_tree;
        // number of valid (non-empty) nodes in the tree
        int64_t m_numLeaves;
        // tree order (2 for binary)
        int64_t m_kTreeOrder;
        // TODO: find better null value
        float m_kNullVal;
};

#endif
