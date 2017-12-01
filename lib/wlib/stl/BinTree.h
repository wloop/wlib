#ifndef WLIB_TREE_H
#define WLIB_TREE_H

#include <stdint.h>
#include <stdlib.h>
#include  "ArrayList.h"
#include "../memory/Memory.h"

// TODO: delete below lines after array list implementation
//#include <vector>
using namespace std;

/* Implement a (Binary) Tree using array list
 * TOOO: implement with our array list class instead of stdvetor
  */
class Tree {
    public:
        // Constructor - add a root to the tree
        Tree(float rootVal) {
            m_tree.push_back(rootVal);
            m_numLeaves = 1; // number of valid (non-empty) nodes
            m_kNullVal = (float)-1.1999; // value for empty nodes
            m_kTreeOrder = 2; // children per parent (2 for binary)
        }

        // Return tree root if exists
        float GetRoot(void) { 
            if ((uint64_t) m_tree.size() > 0) {
              return m_tree[0];
            } 
            return m_kNullVal;
        }

        // Return the value at index in the vector
        float GetValFromIndex(size_t index) {
            return m_tree[index];
        }

        // Insert a child node given the index of the parent
        // Return the child's index
        int64_t InsertChildWithParent(size_t parentIndex, float childVal) {
            int64_t childIndex = ((int64_t) m_kTreeOrder)*((int64_t)parentIndex) + 1;
            int64_t len = (int64_t) m_tree.size();

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
         
        // returns pointer to array of the two children
        float* GetChildrenValsFromParent(size_t parentIndex){
            float *p_children = (float *) malloc(2*sizeof(float));
            bool child = 0;

            if (m_tree[2*parentIndex + 1] != m_kNullVal) {
                p_children[child] = m_tree[2*parentIndex + 1];
                child = 1;
            }

            if (m_tree[2*parentIndex + 2] != m_kNullVal) {
                p_children[child] = m_tree[2*parentIndex + 2];
            }

            return p_children;
        }
        
        // returns pointer to array of the two children
        size_t* GetChildrenIndexFromParent(size_t parentIndex){
            size_t *p_children = (size_t *) malloc(2*sizeof(size_t));
            bool child = 0;

            if (m_tree[2*parentIndex + 1] != m_kNullVal) {
                p_children[child] = 2*parentIndex + 1;
                child = 1;
            }

            if (m_tree[2*parentIndex + 2] != m_kNullVal) {
                p_children[child] = 2*parentIndex + 2;
            }

            return p_children;
        }

        // returns parent index given child index
        size_t GetParentIndexFromChild(size_t childIndex){
          return ((childIndex-1)/2);
        }
        
        // returns parent index given child index
        float GetParentValFromChild(size_t childIndex){
          return m_tree[(1/2)*(childIndex-1)];
        }

        // Clear the tree
        void Clear() {
            m_tree.clear();
            m_numLeaves = 0;
        }

        // Get the number of leaves in the tree
        uint64_t GetNumLeaves() {
          return m_numLeaves;
        }

    private:
        // vector to store the tree
        vector<float> m_tree;
        // number of valid (non-empty) nodes in the tree
        uint64_t m_numLeaves;
        // tree order (2 for binary)
        uint64_t m_kTreeOrder;
        // TODO: find better null value
        float m_kNullVal;
};

#endif
