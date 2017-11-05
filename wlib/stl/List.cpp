/**
 * @file List.cpp
 * @brief List is an implementation of the stl doubly-linked link
 *
 * It provides functionality for pushing popping from front and back
 * and search and indexing
 *
 * @author Ambareesh Balaji
 * @date November 4, 2017
 * @bug No Known bugs
 */

#include "../memory/Memory.h"
#include "List.h"

using namespace wlp;
// Constructor
template<class T>
List<T>::List() :
    m_len(0),
    m_pStart(nullptr),
    m_pEnd(nullptr) {

}

// Destructor
template<class T>
List<T>::~List() {
    clear();
}

// Push at front
template<class T>
void List<T>::push_front(const T value) {
    m_Node *pNewNode = memory_alloc(sizeof(m_Node));
    pNewNode->item = value;
    pNewNode->pNext = nullptr;
    pNewNode->pPrev = nullptr;

    if (m_pStart == nullptr) { // If list is empty
        m_pStart = pNewNode;
    }
    else { // Insert at m_pStart
        pNewNode->pNext = m_pStart;
        m_pStart->pPrev = pNewNode;
    }
    m_pStart = pNewNode;
    m_len++; // Increase size counter
}

// Push at back
template<class T>
void List<T>::push_back(const T value) {
    m_Node *pNewNode = memory_alloc(sizeof(m_Node));
    pNewNode->item = value;
    pNewNode->pNext = nullptr;
    pNewNode->pPrev = nullptr;

    if (m_pEnd == nullptr) { // If list is empty
        m_pStart = pNewNode;
    }
    else { // Insert at the m_pEnd
        pNewNode->pPrev = m_pEnd;
        m_pEnd->pNext = pNewNode;
    }
    m_pEnd = pNewNode;
    m_len++; // Increase size counter
}

// Pop from front
template<class T>
void List<T>::pop_front()
{
    if (m_pStart != nullptr) {
        m_Node *pTmp = m_pStart;
        m_pStart = m_pStart->pNext;
        if (m_pStart != nullptr) { // Re-link pNext item to nullptr
            m_pStart->pPrev = nullptr;
        }
        else { // List became empty so we need to clear m_pEnd
            m_pEnd = nullptr;
        }
        memory_free(pTmp);
        m_len--; // Decrement length
    }
}

// Pop from back
template<class T>
void List<T>::pop_back() {
    if (m_pEnd != nullptr) {
        m_Node *pTmp = m_pEnd;
        m_pEnd = m_pEnd->pPrev;
        if (m_pEnd != nullptr) { //Re-link pPrevious item to nullptr
            m_pEnd->pNext = nullptr;
        }
        else { // List became empty so we need to clear m_pStart
            m_pStart = nullptr;
        }
        memory_free(pTmp);
        m_len--; // Decrement length
    }
}

// Get item from front
template<class T>
T List<T>::front() {
    // dangerous (will die if nullptr)
    return m_pStart->item;
}

//Get item from back
template<class T>
T List<T>::back() {
    // dangerous (will die if nullptr)
    return m_pEnd->item;
}

// Get size
template<class T>
int List<T>::size() {
    return this->m_len;
}

// Clear list
template<class T>
void List<T>::clear() {
    m_Node *pTmp = m_pStart;
    while (m_pStart != nullptr) {
        pTmp = m_pStart;
        m_pStart = m_pStart->pNext;
        memory_free(pTmp); // Delete item
        m_len--; // Decrement length
    }
    m_pEnd = nullptr;
}
template<class T>
void List<T>::remove_at(uint32_t index) {
    // dangerous (will die if out of bounds)
    m_Node *pTmp = m_pStart;
    while (index-- > 0) {
        pTmp = pTmp->pNext;
    }
    if (pTmp->pPrev != nullptr) {
        pTmp->pPrev->pNext = pTmp->pNext;
    }
    else {
        m_pStart = pTmp->pNext;
    }

    if (pTmp->pNext != nullptr) {
        pTmp->pNext->pPrev = pTmp->pPrev;
    }
    else {
        m_pEnd = pTmp->pPrev;
    }

    memory_free(pTmp); // Delete item
    m_len--; // Decrement length
}

// Get at index
template<class T>
T List<T>::get(uint32_t index) {
    // dangerous (will die if nullptr)
    m_Node *pTmp = m_pStart;
    while (index-- > 0) {
        pTmp = pTmp -> pNext;
    }
    return pTmp -> item;
}

template<class T>
T& List<T>::at(uint32_t index) {
    // dangerous (will die if nullptr)
    m_Node *pTmp = m_pStart;
    while (index-- > 0) {
        pTmp = pTmp -> pNext;
    }
    return pTmp -> item;
}

// Get m_len
template<class T>
uint32_t List<T>::length() {
    return m_len;
}

// Get index of value
template<class T>
uint32_t List<T>::indexOf(const T &val) {
    m_Node *pTmp = m_pStart;
    for (uint32_t i = 0; i < m_len; i++) {
        if (pTmp->item == val) {
            return i;
        }
    }
    return m_len;
}

// Array index operator
template<class T>
T& List<T>::operator[](uint32_t index) {
    return at(index);
}

template<class T>
const T& List<T>::operator[](uint32_t index) const {
    return at(index);
}
