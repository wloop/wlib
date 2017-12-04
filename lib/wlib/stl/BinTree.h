/** @file BinTree.h
 *  @brief A binary tree implementation using a list.
 *  
 *  This file implements a standard
 *  binary tree stored using the ArrayList class.
 *  Includes functions to intialize, insert,
 *  and modify tree.
 *
 *  @author Ibrahim Irfan
 *  @date December 3, 2017
 *  @bug No known bugs.
 */

#ifndef WLIB_TREE_H
#define WLIB_TREE_H

#include  "ArrayList.h"

using namespace wlp;

/**
 * Implement a (Binary) Tree using array list
  */
template<class T>
class Tree {
    public:
        /**
         * Constructor - initialize the tree with a root value
         *
         * @param rootVal Value for the root of the tree (first element)
         */
        Tree(T rootVal) {
            m_tree.push_back(rootVal);
            m_numLeaves = 1; // number of valid (non-null) nodes
            m_kNullVal = (T)-1.1999; // value for empty nodes
        }

        /**
         * Returns the root value of the tree
         *
         * @return Value at the root of the tree (first element)
         */
        T getRoot() { 
            return m_tree.at(0);
        }

        /**
         * Returns the value at an index of the tree
         *
         * @param index Index of element desired
         * @return Value at index @p index
         */
        T getVal(unsigned short index) {
            return m_tree.at(index);
        }

        /**
         * Insert a child node given the index of the parent
         *
         * @param parentIndex Index of parent node
         * @param childVal Value to insert
         * @return Index of inserted element in the tree.
         */
        size_t insertChildAt(size_t parentIndex, T childVal) {
            size_t childIndex = 2*parentIndex + 1;
            size_t len = m_tree.size();
            size_t indexCount = 0;

            ArrayList<float>::iterator iter = m_tree.begin();

            while(len > 0){
              iter++;
              indexCount++;
              len--;
            }

            // insert null nodes to fill gap if needed
            while (childIndex > indexCount) {
                m_tree.insert(iter, m_kNullVal);
                iter++;
                indexCount++;
            }

            ArrayList<float>::iterator first = m_tree.begin();
            indexCount = childIndex;

            while(indexCount > 0){
              first++;
              indexCount--;
            }

            // insert the node and increase valid node count
            m_tree.insert(first, childVal);
            m_numLeaves++;
            return childIndex;
        }
         
        /**
         * Get the index of the right child of a parent node
         * 
         * @param parentIndex Index of parent node
         * @return Index of right child of parent.
         * @note The right child is the child that is inserted first. 
         * If there is only one child, then it is the left child.
         */
        size_t getRightChildIndex(size_t parentIndex){
            return 2*parentIndex + 2;
        }
        
       /**
         * Get the index of the left child of a parent node
         * 
         * @param parentIndex Index of parent node
         * @return Index of left child of parent.
         * @note The left child is the child that is inserted second.
         * If there is only one child, then it is the left child.
         */
        size_t getLeftChildIndex(size_t parentIndex){
            return 2*parentIndex + 1;
        }

        /**
         * Get the value of the right child of a parent node
         * 
         * @param parentIndex Index of parent node
         * @return Value of right child of parent.
         * @note The right child is the child that is inserted first. 
         * If there is only one child, then it is the left child.
         */
        T getRightChildVal(unsigned short parentIndex){
            return m_tree.at(2*parentIndex + 2);
        }
        
       /**
         * Get the value of the left child of a parent node
         * 
         * @param parentIndex index of parent node
         * @return Index of left child of parent.
         * @note The left child is the child that is inserted second.
         * If there is only one child, then it is the left child.
         */
        T getLeftChildVal(unsigned short parentIndex){
            return m_tree.at(2*parentIndex + 1);
        }
        
        /** 
         * Returns parent index given child index
         *
         * @param childIndex index of child node
         * @return Index of the parent with child at @p childIndex
         */
        size_t getParentIndex(size_t childIndex){
            return ((childIndex-1)/2);
        }
        
        /** 
         * Returns parent value given child index
         *
         * @param childIndex index of child node
         * @return Value of the parent with child at @p childIndex
         */
        T getParentVal(unsigned short childIndex){
            return m_tree.at((1/2)*(childIndex-1));
        }

        /** 
         * Clears the tree.
         */
        void clear() {
            m_tree.clear();
            m_numLeaves = 0;
        }

        /** 
         * Returns the number of leaves (valid, non-empty nodes)
         * in the tree.
         *
         * @return Number of leaves (valid, non-empty nodes) in the tree.
         */
        uint64_t getNumLeaves() {
            return m_numLeaves;
        }

    private:
        /**
         * List to store the tree
         */
        T empty[0] = {};
        ArrayList<T> m_tree{empty, 0};
        /**
         * Number of valid (non-empty) nodes in the tree
         */
        uint64_t m_numLeaves;
        /**
         * Null value for empty nodes
         */
        float m_kNullVal;
};

#endif
