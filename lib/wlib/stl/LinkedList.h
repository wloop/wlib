/**
 * @file List.h
 * @brief List is an implementation of the stl doubly-linked link
 *
 * It provides functionality for pushing popping from front and back
 * and search and indexing
 *
 * @author Ambareesh Balaji
 * @author Jeff Niu
 * @date November 4, 2017
 * @bug No Known bugs
 */

#ifndef CORE_STL_LIST_H
#define CORE_STL_LIST_H

#include "memory/Memory.h"
#include "memory/Allocator.h"
#include "../utility/Utility.h"

namespace wlp {

    template<typename T>
    struct LinkedListNode {
        typedef T val_type;
        typedef LinkedListNode<T> node_type;

        val_type m_val;
        node_type *m_next;
        node_type *m_prev;
    };

    // Forward Declaration of List class
    template<typename T>
    class LinkedList;

    /**
     * Iterator class over the elements of a @code LinkedList @endcode.
     *
     * @tparam T value type
     */
    template<typename T, typename Ref, typename Ptr>
    struct LinkedListIterator {
        typedef T val_type;
        typedef wlp::size_type size_type;
        typedef LinkedListNode<T> node_type;
        typedef LinkedList<T> list_type;
        typedef LinkedListIterator<T, Ref, Ptr> self_type;

        /**
         * Pointer to the node referenced by this iterator.
         */
        node_type *m_current;
        /**
         * Pointer to the iterated List.
         */
        const list_type *m_list;

        /**
         * Default constructor.
         */
        LinkedListIterator()
                : m_current(nullptr),
                  m_list(nullptr) {}

        /**
         * Create an iterator to a List node.
         *
         * @param node linked list node
         * @param list  parent linked list
         */
        LinkedListIterator(node_type *node, const list_type *list)
                : m_current(node),
                  m_list(list) {}

        /**
         * Copy constructor.
         *
         * @param it iterator copy
         */
        LinkedListIterator(const self_type &it)
                : m_current(it.m_current),
                  m_list(it.m_list) {}

        /**
         * @return reference to the value of the node
         * pointed to by the iterator
         */
        val_type &operator*() const {
            return m_current->m_val;
        }

        /**
         * @return pointer to the value of the node
         * pointed to by the iterator
         */
        val_type *operator->() const {
            return &(operator*());
        }

        /**
         * Increment the iterator to the next available element in
         * the List. If no such element exists, returns pass-the-end
         * iterator. This is pre-fix unary operator.
         *
         * @return this iterator
         */
        self_type &operator++() {
            if (m_current) {
                m_current = m_current->m_next;
            }
            return *this;
        }

        /**
         * Post-fix unary operator.
         *
         * @return this iterator
         */
        self_type operator++(int) {
            self_type clone(*this);
            ++*this;
            return clone;
        }

        /**
         * Decrement the iterator by moving it
         * to the previous element in the list. If the iterator
         * is already at the beginning of the list,
         * the iterator does not move.
         *
         * @return reference to this iterator
         */
        self_type &operator--() {
            if (m_current && m_current != m_list->m_head) {
                m_current = m_current->m_prev;
            }
            return *this;
        }

        /**
         * Post-fix decrement operator.
         *
         * @return reference to this iterator
         */
        self_type operator--(int) {
            self_type clone(*this);
            --*this;
            return *this;
        }

        /**
         * Equality operator.
         *
         * @param it iterator to compare
         * @return true if they are equal
         */
        bool operator==(const self_type &it) const {
            return m_current == it.m_current;
        }

        /**
         * Compare two iterators, unequal if they point to
         * different nodes.
         *
         * @param it iterator to compare
         * @return true if they point to different nodes
         */
        bool operator!=(const self_type &it) const {
            return m_current != it.m_current;
        }

        /**
         * Assignment operator copies pointer to node
         * and pointer to linked list.
         *
         * @param it iterator to copy
         * @return reference to this iterator
         */
        self_type &operator=(const self_type &it) {
            m_current = it.m_current;
            m_list = it.m_list;
            return *this;
        }
    };

    /**
     * List implementation as a Doubly-Linked list.
     *
     * @tparam T value type
     */
    template<typename T>
    class LinkedList {
    public:
        typedef T val_type;
        typedef wlp::size_type size_type;
        typedef LinkedList<T> list_type;
        typedef LinkedListNode<T> node_type;
        typedef LinkedListIterator<T, T &, T *> iterator;
        typedef LinkedListIterator<T, const T &, const T *> const_iterator;

    private:
        /**
         * The number of elements in the list.
         */
        size_type m_size;
        /**
         * Pointer to first node in the list.
         */
        node_type *m_head;
        /**
         * Pointer to last node in the list.
         */
        node_type *m_tail;

        friend struct LinkedListIterator<T, T &, T *>;
        friend struct LinkedListIterator<T, const T &, const T *>;

    public:
        /**
         * Default Constructor creates an empty List.
         */
        LinkedList()
                : m_size(0),
                  m_head(nullptr),
                  m_tail(nullptr) {}

        /**
         * Disable copy construction.
         */
        LinkedList(const list_type &) = delete;

        /**
         * Move constructor takes the pointers to the head and tail
         * nodes and copies them into this list. The transferred list
         * is set to an empty list.
         *
         * @param list the list to move
         */
        LinkedList(list_type &&list) :
                m_size(move(list.m_size)),
                m_head(move(list.m_head)),
                m_tail(move(list.m_tail)) {
            list.m_size = 0;
            list.m_head = nullptr;
            list.m_tail = nullptr;
        }

        /**
         * Default Destructor deallocates all the nodes.
         */
        ~LinkedList();

        /**
         * Creates a new node and pushes it to the back of the list.
         *
         * @param value the value to store at the new node
         */
        template<typename V>
        void push_back(V &&val) {
            node_type *node = malloc<node_type>();
            node->m_val = forward<V>(val);
            node->m_next = nullptr;
            if (m_head == nullptr) {
                m_head = node;
                node->m_prev = nullptr;
            } else {
                m_tail->m_next = node;
                node->m_prev = m_tail;
            }
            m_tail = node;
            ++m_size;
        }

        /**
         * Creates a new node and pushes it to the front of the list.
         *
         * @param value the value to store at the new node
         */
        template<typename V>
        void push_front(V &&val) {
            node_type *node = malloc<node_type>();
            node->m_val = forward<V>(val);
            node->m_prev = nullptr;

            if (m_tail == nullptr) {
                m_tail = node;
                node->m_next = nullptr;
            } else {
                m_head->m_prev = node;
                node->m_next = m_head;
            }
            m_head = node;
            m_size++;
        }

        void pop_back() {
            if (!m_tail) {
                return;
            }
            node_type *pTmp = m_tail;
            m_tail = m_tail->m_prev;
            if (m_tail != nullptr) {
                m_tail->m_next = nullptr;
            } else {
                m_head = nullptr;
            }
            free<node_type>(pTmp);
            m_size--;
        }

        void pop_front() {
            if (!m_head) {
                return;
            }
            node_type *pTmp = m_head;
            m_head = m_head->m_next;
            if (m_head != nullptr) {
                m_head->m_prev = nullptr;
            } else {
                m_tail = nullptr;
            }
            free<node_type>(pTmp);
            m_size--;
        }

        /**
         * @return a reference to the value at the start of the List
         */
        val_type &front() {
            return m_head->m_val;
        }

        /**
         * @return a const reference to the value at the start of the List
         */
        const val_type &front() const {
            return m_head->m_val;
        }

        /**
         * @return a reference to the value at the end of the List
         */
        val_type &back() {
            return m_tail->m_val;
        }

        /**
         * @return a const reference to the value at the end of the List
         */
        const val_type &back() const {
            return m_tail->m_val;
        }

        /**
         * Returns the length of the List
         *
         * @return the length of the list
         */
        size_type size() const {
            return m_size;
        }

        void clear() noexcept;

        /**
         * Removes the value stored at the given index
         *
         * @param index the index to remove at
         */
        void erase(size_type index);

        /**
         * Return a reference to the value stored at the `index`
         *
         * @param index to get
         * @return reference to the value at that index
         */
        val_type &at(size_type index);

        /**
         * Return a reference to the value stored at the `index` (const variant)
         *
         * @param index to get
         * @return reference to the value at that index
         */
        const val_type &at(size_type index) const;

        /**
         * Array indexing operator
         * `at` behind the scenes
         *
         * @param index to get
         * @return reference to the value at that index
         */
        val_type &operator[](size_type index) {
            return at(index);
        }

        /**
         * Array indexing operator const variant
         * `at` behind the scenes
         *
         * @param index to get
         * @return reference to the value at that index
         */
        const val_type &operator[](size_type index) const {
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
        size_type indexOf(const T &value);

        /**
         * Obtain an iterator to the first element in the list.
         * Returns pass-the-end iterator (nullptr) if
         * there are no elements (m_pStart == nullptr)
         *
         * @return iterator the first element
         */
        iterator begin() {
            return iterator(m_head, this);
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
        const_iterator begin() const {
            return const_iterator(m_head, this);
        }

        /**
         * @see List<T>::end()
         * @return a constant pass-the-end iterator
         */
        const_iterator end() const {
            return const_iterator(nullptr, this);
        }

        /**
         * Delete copy assignment.
         *
         * @return reference to this list
         */
        list_type &operator=(const list_type &) = delete;

        /**
         * Move assignment operator.
         *
         * @param list the list to move
         * @return a reference to this list
         */
        list_type &operator=(list_type &&list) {
            m_size = list.m_size;
            m_head = list.m_head;
            m_tail = list.m_tail;
            list.m_size = 0;
            list.m_head = nullptr;
            list.m_tail = nullptr;
            return *this;
        }
    };

    template<typename T>
    LinkedList<T>::~LinkedList() {
        clear();
    }

    template<typename T>
    void LinkedList<T>::clear() noexcept {
        node_type *pTmp;
        while (m_head != nullptr) {
            pTmp = m_head;
            m_head = m_head->m_next;
            free<node_type>(pTmp);
        }
        m_size = 0;
        m_tail = nullptr;
    }

    template<typename T>
    void LinkedList<T>::erase(size_type index) {
        if (!m_size) {
            return;
        }
        if (index >= m_size) {
            index %= m_size;
        }
        node_type *pTmp = m_head;
        while (index-- > 0) {
            pTmp = pTmp->m_next;
        }
        if (pTmp->m_prev != nullptr) {
            pTmp->m_prev->m_next = pTmp->m_next;
        } else {
            m_head = pTmp->m_next;
        }
        if (pTmp->m_next != nullptr) {
            pTmp->m_next->m_prev = pTmp->m_prev;
        } else {
            m_tail = pTmp->m_prev;
        }
        free<node_type>(pTmp);
        m_size--;
    }

    template<typename T>
    typename LinkedList<T>::val_type &
    LinkedList<T>::at(size_type index) {
        if (index >= m_size) {
            if (m_size) { index %= m_size; }
            else { index = 0; }
        }
        node_type *pTmp = m_head;
        while (index-- > 0) {
            pTmp = pTmp->m_next;
        }
        return pTmp->m_val;
    }

    template<typename T>
    const typename LinkedList<T>::val_type &
    LinkedList<T>::at(size_type index) const {
        if (index >= m_size) {
            if (m_size) { index %= m_size; }
            else { index = 0; }
        }
        node_type *pTmp = m_head;
        while (index-- > 0) {
            pTmp = pTmp->m_next;
        }
        return pTmp->m_val;
    }

    template<typename T>
    typename LinkedList<T>::size_type
    LinkedList<T>::indexOf(const T &value) {
        node_type *pTmp = m_head;
        for (size_type i = 0; i < m_size; i++) {
            if (pTmp->m_val == value) {
                return i;
            }
            pTmp = pTmp->m_next;
        }
        return m_size;
    }

}

#endif // CORE_STL_LIST_H
