/**
 * @file List.h
 * @brief List is an implementation of the stl doubly-linked link
 *
 * It provides functionality for pushing popping from front and back
 * and search and indexing
 *
 * @author Ambareesh Balaji
 * @date November 4, 2017
 * @bug No Known bugs
 */

#ifndef CORE_STL_LIST_H
#define CORE_STL_LIST_H

#include <stdint.h>
#include "../memory/Memory.h"

namespace wlp {

template<class T>
class List {
    MEMORY_OVERLOAD
private:
    struct m_Node {
        T item;
        m_Node *pNext,*pPrev;
    };

    uint32_t m_len; // Size of list
    m_Node* m_pStart;
    m_Node* m_pEnd; // Pointers to start and end

public:
    List(); // Class constructor
    ~List(); // Class destructor

    void push_back(const T i); // Push item at the back of list
    void push_front(const T i);// Push item at the front of the list
    void pop_back(); // Pops item from back
    void pop_front(); // Pops item from front
    T front(); // get item from front
    T back(); // get item from back
    int size(); // Returns size of list
    void clear(); // Clears list
    void remove_at(uint32_t index); // Remove item at given index
    T get(uint32_t index); // Get item at given index
    T& at(uint32_t index); // Get item at given index

    // Array operator
    T& operator[](uint32_t index);
    const T& operator[](uint32_t index) const;

    // Non - critical functions
    uint32_t length();
    uint32_t indexOf(const T& val); // given a item, find its index and return it, return length if not found
};

} // namespace wlp
#endif // CORE_STL_LIST_H
