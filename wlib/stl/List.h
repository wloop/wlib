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
        T value;
        m_Node *pNext,*pPrev;
    };

    uint32_t m_len; // Size of list
    m_Node* m_pStart;
    m_Node* m_pEnd; // Pointers to start and end

public:
    /**
     * Default Constructor creates an empty List
     */
    List():
        m_len(0),
        m_pStart(nullptr),
        m_pEnd(nullptr) {}

    /**
     * Default Destructor deallocates all the nodes
     */
    ~List() {
        m_Node *pTmp;
        while (m_pStart != nullptr) {
            pTmp = m_pStart;
            m_pStart = m_pStart->pNext;
            memory_free(pTmp);
        }
    }

    /**
     * Creates a new node and pushes it to the back of the list
     *
     * @param value the value to store at the new node
     */
    void push_back(const T value) {
        m_Node *pNewNode = static_cast<m_Node*>(memory_alloc(sizeof(m_Node)));
        pNewNode->value = value;
        pNewNode->pNext = nullptr;

        if (m_pStart == nullptr) {
            m_pStart = pNewNode;
            pNewNode->pPrev = nullptr;
        } else {
            m_pEnd->pNext = pNewNode;
            pNewNode->pPrev = m_pEnd;
        }
        m_pEnd = pNewNode;
        m_len++;
    }

    /**
     * Creates a new node and pushes it to the front of the list
     *
     * @param value the value to store at the new node
     */
    void push_front(const T value) {
        m_Node *pNewNode = static_cast<m_Node*>(memory_alloc(sizeof(m_Node)));
        pNewNode->value = value;
        pNewNode->pPrev = nullptr;

        if (m_pEnd == nullptr) {
            m_pEnd = pNewNode;
            pNewNode->pNext = nullptr;
        } else {
            m_pStart->pPrev = pNewNode;
            pNewNode->pNext = m_pStart;
        }
        m_pStart = pNewNode;
        m_len++;
    }

    void pop_back() {
        m_Node *pTmp = m_pEnd;
        m_pEnd = m_pEnd->pPrev;
        if (m_pEnd != nullptr) {
            m_pEnd->pNext = nullptr;
        } else {
            m_pStart = nullptr;
        }
        memory_free(pTmp);
        m_len--;
    }

    void pop_front() {
        m_Node *pTmp = m_pStart;
        m_pStart = m_pStart->pNext;
        if (m_pStart != nullptr) {
            m_pStart->pPrev = nullptr;
        } else {
            m_pEnd = nullptr;
        }
        memory_free(pTmp);
        m_len--;
    }

    /**
     * Returns the value at the start of the List
     * DANGEROUS: will die if nullptr
     *
     * @return the value stored at the start of the list
     */
    T front() {
        return m_pStart->value;
    }

    /**
     * Returns the value at the end of the List
     * DANGEROUS: will die if nullptr
     *
     * @return the value stored at the end of the list
     */
    T back() {
        return m_pEnd->value;
    }

    /**
     * Returns the length of the List
     * DANGEROUS: will die if nullptr
     *
     * @return the length of the list
     */
    uint32_t size() {
        return m_len;
    }

    void clear() {
        m_Node *pTmp;
        while (m_pStart != nullptr) {
            pTmp = m_pStart;
            m_pStart = m_pStart->pNext;
            memory_free(pTmp);
            m_len--;
        }
        m_pEnd = nullptr;
    }

    /**
     * Removes the value stored at the given index
     * DANGEROUS: will die if out of bounds
     *
     * @param index the index to remove at
     */
    void remove_at(uint32_t index) {
        m_Node *pTmp = m_pStart;
        while (index-- > 0) {
            pTmp = pTmp->pNext;
        }

        if (pTmp->pPrev != nullptr) {
            pTmp->pPrev->pNext = pTmp->pNext;
        } else {
            m_pStart = pTmp->pNext;
        }

        if (pTmp->pNext != nullptr) {
            pTmp->pNext->pPrev = pTmp->pPrev;
        } else {
            m_pEnd = pTmp->pPrev;
        }

        memory_free(pTmp);
        m_len--;
    }

    /**
     * Return a copy of the value stored at the `index`
     * DANGEROUS: will die if out of bounds
     *
     * @param index to get
     * @return copy of the value at that index
     */
    T get(uint32_t index) {
        m_Node *pTmp = m_pStart;
        while (index-- > 0) {
            pTmp = pTmp->pNext;
        }
        return pTmp->value;
    }

    /**
     * Return a reference to the value stored at the `index`
     * DANGEROUS: will die if out of bounds
     *
     * @param index to get
     * @return reference to the value at that index
     */
    T& at(uint32_t index) {
        m_Node *pTmp = m_pStart;
        while (index-- > 0) {
            pTmp = pTmp->pNext;
        }
        return pTmp->value;
    }

    /**
     * Array indexing operator
     * `at` behind the scenes
     * DANGEROUS: will die if out of bounds
     *
     * @param index to get
     * @return reference to the value at that index
     */
    T& operator[](uint32_t index) {
        return at(index);
    }

    /**
     * Array indexing operator const variant
     * `at` behind the scenes
     * DANGEROUS: will die if out of bounds
     *
     * @param index to get
     * @return reference to the value at that index
     */
    const T& operator[](uint32_t index) const {
        return at(index);
    }

    /**
     * indexOf from Javascript land that finds the index of the
     * first element from the beginning of the List that equals `value`
     * but returns the length of the array instead of -1 on failure
     * DANGEROUS: will die if out of bounds
     *
     * @param value value to search for
     * @return reference to the value at that index
     */
    uint32_t indexOf(const T& value) {
        m_Node *pTmp = m_pStart;
        for (uint32_t i = 0; i < m_len; i++) {
            if (pTmp->value == value) {
                return i;
            }
            pTmp = pTmp->pNext;
        }
        return m_len;
    }
};

} // namespace wlp
#endif // CORE_STL_LIST_H
