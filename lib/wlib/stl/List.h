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
#include "memory/Memory.h"
#include "memory/Allocator.h"
#include "Utility.h"

namespace wlp {

template<class T>
struct ListNode {
    T m_val;
    ListNode<T> *pNext,*pPrev;
};

// Forward Declaration of List class
template<class T>
class List;

/**
 * Iterator class over the elements of a List. Specifically,
 * this class iterates through each chain and then the backing array.
 * @tparam T   value type
 */

template<class T>
struct ListIterator {
    typedef ListIterator<T> iterator;
    /**
     * Pointer to the node referenced by this iterator.
     */
    ListNode<T> *m_pCurrent;
    /**
     * Pointer to the iterated List.
     */
    List<T> *m_pList;

    /**
     * Default constructor.
     */
    ListIterator()
            : m_pCurrent(nullptr),
            m_pList(nullptr) {}

    /**
     * Create an iterator to a List node.
     * @param node linked list node
     * @param list  parent linked list
     */
    ListIterator(ListNode<T> *node, List<T> *list)
            : m_pCurrent(node),
            m_pList(list) {}

    /**
     * Copy constructor for const.
     * @param it iterator copy
     */
    ListIterator(const iterator &it)
            : m_pCurrent(it.m_pCurrent),
            m_pList(it.m_pList) {}

    /**
     * Copy constructor.
     * @param it iterator to copy
     */
    ListIterator(iterator &&it)
            : m_pCurrent(move(it.m_pCurrent)),
            m_pList(move(it.m_pList)) {}

    /**
     * @return reference to the value of the node
     * pointed to by the iterator
     */
    T &operator*() const {
        return m_pCurrent->m_val;
    }

    /**
     * @return pointer to the value of the node
     * pointed to by the iterator
     */
    T *operator->() const {
        return &(operator*());
    }

    /**
     * Increment the iterator to the next available element in
     * the List. If no such element exists, returns pass-the-end
     * iterator. This is pre-fix unary operator.
     * @return this iterator
     */
    iterator &operator++() {
        m_pCurrent = m_pCurrent->pNext;
        return *this;
    }

    /**
     * Post-fix unary operator.
     * @return this iterator
     */
    iterator operator++(int) {
        iterator tmp = *this;
        m_pCurrent = m_pCurrent->pNext;
        return tmp;
    }

    /**
     * Equality operator for const.
     * @param it iterator to compare
     * @return true if they are equal
     */
    bool operator==(const iterator &it) const {
        return m_pCurrent == it.m_pCurrent;
    }

    /**
     * Compare two iterators, equal of they point to the
     * same node.
     * @param it iterator to compare
     * @return true if both point to the same node
     */
    bool operator==(iterator &it) const {
        return m_pCurrent == it.m_pCurrent;
    }

    /**
     * Compare two iterators, unequal if they point to
     * different nodes.
     * @param it iterator to compare
     * @return true if they point to different nodes
     */
    bool operator!=(const iterator &it) const {
        return m_pCurrent != it.m_pCurrent;
    }

    /**
     * Inequality operator.
     * @param it iterator to compare
     * @return true if they point to different nodes
     */
    bool operator!=(iterator &it) const {
        return m_pCurrent != it.m_pCurrent;
    }

    /**
     * Assignment operator copies pointer to node
     * and pointer to linked list.
     * @param it iterator to copy
     * @return reference to this iterator
     */
    iterator &operator=(const iterator &it) {
        m_pCurrent = it.m_pCurrent;
        m_pList = it.m_pList;
        return this;
    }

    /**
     * Assignment operator copies pointer to node
     * and pointer to linked list.
     * @param it iterator to copy
     * @return a reference to this iterator
     */
    iterator &operator=(iterator &&it) {
        m_pCurrent = move(it.m_pCurrent);
        m_pList = move(it.m_pList);
        return *this;
    }
};
/**
 * Constant iterator over a List. Values iterated by
 * this class cannot be modified.
 *
 * @see ListIterator
 *
 * @tparam T   value type
 */
template<class T>
struct ListConstIterator {
    typedef ListConstIterator<T> const_iterator;

    const ListNode<T> *m_pCurrent;
    const List<T> *m_pList;

    ListConstIterator()
            : m_pCurrent(nullptr),
            m_pList(nullptr) {}

    ListConstIterator(ListNode<T> *node, const List<T> *list)
            : m_pCurrent(node),
            m_pList(list) {}

    ListConstIterator(const const_iterator &it)
            : m_pCurrent(it.m_pCurrent),
            m_pList(it.m_pList) {}

    ListConstIterator(const_iterator &&it)
            : m_pCurrent(move(it.m_pCurrent)),
            m_pList(move(it.m_pList)) {}

    const T &operator*() const {
        return m_pCurrent->m_val;
    }

    const T *operator->() const {
        return &(operator*());
    }

    const_iterator &operator++() {
        m_pCurrent = m_pCurrent->pNext;
        return this;
    }

    const_iterator operator++(int) {
        const_iterator tmp = *this;
        m_pCurrent = m_pCurrent->pNext;
        return tmp;
    }

    bool operator==(const const_iterator &it) const {
        return m_pCurrent == it.m_pCurrent;
    }

    bool operator==(const_iterator &it) const {
        return m_pCurrent == it.m_pCurrent;
    }

    bool operator!=(const const_iterator &it) const {
        return m_pCurrent != it.m_pCurrent;
    }

    bool operator!=(const_iterator &it) const {
        return m_pCurrent != it.m_pCurrent;
    }

    const_iterator &operator=(const const_iterator &it) {
        m_pCurrent = it.m_pCurrent;
        m_pList = it.m_pList;
        return this;
    }

    const_iterator &operator=(const_iterator &&it) {
        m_pCurrent = move(it.m_pCurrent);
        m_pList = move(it.m_pList);
        return this;
    }
};

/**
 * Doubly-Linked List class
 *
 * @tparam T value type
 */
template<class T>
class List {
    typedef ListIterator<T> iterator;
    typedef ListConstIterator<T> const_iterator;
    MEMORY_OVERLOAD
private:
    size_type m_len; // Size of list
    ListNode<T> *m_pStart;
    ListNode<T> *m_pEnd; // Pointers to start and end
    Allocator m_allocator = Allocator(static_cast<size_type>(sizeof(ListNode<T>)));

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
    ~List();

    /**
     * Creates a new node and pushes it to the back of the list
     *
     * @param value the value to store at the new node
     */
    void push_back(const T& value) {
        ListNode<T> *pNewNode = static_cast<ListNode<T>*>(m_allocator.Allocate());
        pNewNode->m_val = value;
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
    void push_front(const T& value) {
        ListNode<T>* pNewNode = static_cast<ListNode<T>*>(m_allocator.Allocate());
        pNewNode->m_val = value;
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
        ListNode<T> *pTmp = m_pEnd;
        m_pEnd = m_pEnd->pPrev;
        if (m_pEnd != nullptr) {
            m_pEnd->pNext = nullptr;
        } else {
            m_pStart = nullptr;
        }
        m_allocator.Deallocate(pTmp);
        m_len--;
    }

    void pop_front() {
        ListNode<T> *pTmp = m_pStart;
        m_pStart = m_pStart->pNext;
        if (m_pStart != nullptr) {
            m_pStart->pPrev = nullptr;
        } else {
            m_pEnd = nullptr;
        }
        m_allocator.Deallocate(pTmp);
        m_len--;
    }

    /**
     * @return a reference to the value at the start of the List
     */
    T& front() {
        return m_pStart->m_val;
    }

    /**
     * @return a const reference to the value at the start of the List
     */
    const T& front() const {
        return m_pStart->m_val;
    }

    /**
     * @return a reference to the value at the end of the List
     */
    T& back() {
        return m_pEnd->m_val;
    }

    /**
     * @return a const reference to the value at the end of the List
     */
    const T& back() const {
        return m_pEnd->m_val;
    }

    /**
     * Returns the length of the List
     *
     * @return the length of the list
     */
    size_type size() {
        return m_len;
    }

    void clear();

    /**
     * Removes the value stored at the given index
     *
     * @param index the index to remove at
     */
    void remove_at(size_type index);

    /**
     * Return a copy of the value stored at the `index`
     *
     * @param index to get
     * @return copy of the value at that index
     */
    T get(size_type index);

    /**
     * Return a reference to the value stored at the `index`
     *
     * @param index to get
     * @return reference to the value at that index
     */
    T& at(size_type index);

    /**
     * Return a reference to the value stored at the `index` (const variant)
     *
     * @param index to get
     * @return reference to the value at that index
     */
    const T& at(size_type index) const;

    /**
     * Array indexing operator
     * `at` behind the scenes
     *
     * @param index to get
     * @return reference to the value at that index
     */
    T& operator[](size_type index) {
        return at(index);
    }

    /**
     * Array indexing operator const variant
     * `at` behind the scenes
     *
     * @param index to get
     * @return reference to the value at that index
     */
    const T& operator[](size_type index) const {
        return at(index);
    }

    /**
     * indexOf from Javascript land that finds the index of the
     * first element from the beginning of the List that equals `value`
     * but returns the length of the array instead of -1 on failure
     *
     * @param value value to search for
     * @return reference to the value at that index
     */
    size_type indexOf(const T& value);

    /**
     * Obtain an iterator to the first element in the list.
     * Returns pass-the-end iterator (nullptr) if
     * there are no elements (m_pStart == nullptr)
     *
     * @return iterator the first element
     */
    iterator begin() {
        return iterator(m_pStart, this);
    }

    /**
     * @return a pass-the-end iterator for this map
     */
    iterator end() {
        return iterator(nullptr, this);
    }

    /**
     * @see List<T>::begin()
     * @return a constant iterator to the first element in the list
     */
    const const_iterator begin() const {
        return const_iterator(m_pStart, this);
    }

    /**
     * @see List<T>::end()
     * @return a constant pass-the-end iterator
     */
    const const_iterator end() const {
        return const_iterator(nullptr, this);
    }
};

template<class T>
List<T>::~List() {
    ListNode<T> *pTmp;
    while (m_pStart != nullptr) {
        pTmp = m_pStart;
        m_pStart = m_pStart->pNext;
        m_allocator.Deallocate(pTmp);
    }
}

template<class T>
void List<T>::clear() {
    ListNode<T> *pTmp;
    while (m_pStart != nullptr) {
        pTmp = m_pStart;
        m_pStart = m_pStart->pNext;
        m_allocator.Deallocate(pTmp);
    }
    m_len = 0;
    m_pEnd = nullptr;
}

template<class T>
void List<T>::remove_at(size_type index) {
    ListNode<T> *pTmp = m_pStart;
    while (index-- > 0) {
        if (pTmp == nullptr) {
            return;
        }
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

    m_allocator.Deallocate(pTmp);
    m_len--;
}

template<class T>
T List<T>::get(size_type index) {
    ListNode<T> *pTmp = m_pStart;
    while (index-- > 0) {
        pTmp = pTmp->pNext;
    }
    return pTmp->m_val;
}

template<class T>
T& List<T>::at(size_type index) {
    ListNode<T> *pTmp = m_pStart;
    while (index-- > 0) {
        pTmp = pTmp->pNext;
    }
    return pTmp->m_val;
}

template<class T>
const T& List<T>::at(size_type index) const {
    ListNode<T> *pTmp = m_pStart;
    while (index-- > 0) {
        pTmp = pTmp->pNext;
    }
    return pTmp->m_val;
}

template<class T>
size_type List<T>::indexOf(const T& value) {
    ListNode<T> *pTmp = m_pStart;
    for (size_type i = 0; i < m_len; i++) {
        if (pTmp->m_val == value) {
            return i;
        }
        pTmp = pTmp->pNext;
    }
    return m_len;
}

} // namespace wlp
#endif // CORE_STL_LIST_H
