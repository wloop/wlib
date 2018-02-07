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

#include "wlib/memory/Memory.h"

#include "wlib/utility/Utility.h"
#include "wlib/exceptions/Exceptions.h"

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
        typedef Ref reference;
        typedef Ptr pointer;
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
        reference operator*() const {
            if (m_current == nullptr) {
                THROW(INDEX_EXCEPTION("Accessing invalid iterator"))
            }
            return m_current->m_val;
        }

        /**
         * @return pointer to the value of the node
         * pointed to by the iterator
         */
        pointer operator->() const {
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
        ~LinkedList() {
            clear();
        }

        /**
         * @return whether the list has no elements
         */
        bool empty() const {
            return m_size == 0;
        };

        /**
         * @return the length of the list
         */
        size_type size() const {
            return m_size;
        }

        /**
         * @return the maximum number of elements storable in the list
         */
        size_type capacity() const {
            return static_cast<size_type>(-1);
        }

        /**
         * Return a reference to the value stored at the `index`
         *
         * @param i to get
         * @return reference to the value at that index
         */
        val_type &at(size_type i);

        /**
         * Return a reference to the value stored at the `index` (const variant)
         *
         * @param i to get
         * @return reference to the value at that index
         */
        const val_type &at(size_type i) const;

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
         * @return a reference to the value at the start of the List
         */
        val_type &front() {
            if (m_head == nullptr) {
                THROW(INDEX_EXCEPTION("Accessing empty list"))
            }
            return m_head->m_val;
        }

        /**
         * @return a const reference to the value at the start of the List
         */
        const val_type &front() const {
            if (m_head == nullptr) {
                THROW(INDEX_EXCEPTION("Accessing empty list"))
            }
            return m_head->m_val;
        }

        /**
         * @return a reference to the value at the end of the List
         */
        val_type &back() {
            if (m_tail == nullptr) {
                THROW(INDEX_EXCEPTION("Accessing empty list"))
            }
            return m_tail->m_val;
        }

        /**
         * @return a const reference to the value at the end of the List
         */
        const val_type &back() const {
            if (m_tail == nullptr) {
                THROW(INDEX_EXCEPTION("Accessing empty list"))
            }
            return m_tail->m_val;
        }

        /**
         * Removes all the elements in the list.
         */
        void clear() noexcept;

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
         * Insert a value at the given index.
         *
         * @param i the index of the node to insert
         * @param val the value to insert
         * @return iterator to the inserted element
         */
        template<typename V>
        iterator insert(size_type i, V &&val) {
            if (!m_size) { i = 0; }
            else { i %= m_size; }
            node_type *node = malloc<node_type>();
            node->m_val = forward<V>(val);
            if (m_head == nullptr) {
                node->m_next = nullptr;
                node->m_prev = nullptr;
                m_head = node;
                m_tail = node;
                ++m_size;
                return iterator(m_head, this);
            }
            node_type *pTmp = m_head;
            while(i-- > 0) {
                pTmp = pTmp->m_next;
            }
            node->m_next = pTmp;
            node->m_prev = pTmp->m_prev;
            node->m_next->m_prev = node;
            if (node->m_prev) { node->m_prev->m_next = node; }
            else { m_head = node; }
            ++m_size;
            return iterator(node, this);
        }

        /**
         * Insert a value at the given iterator.
         *
         * @param it the iterator to the node to insert
         * @param val the value to insert
         * @return iterator to the inserted value
         */
        template<typename V>
        iterator insert(const iterator &it, V &&val) {
            if (it == end()) {
                push_back(forward<V>(val));
                return iterator(m_tail, this);
            }
            node_type *node = malloc<node_type>();
            node->m_val = forward<V>(val);
            node->m_next = it.m_current;
            node->m_prev = it.m_current->m_prev;
            node->m_next->m_prev = node;
            if (node->m_prev) { node->m_prev->m_next = node; }
            else { m_head = node; }
            ++m_size;
            return iterator(node, this);
        }

        /**
         * Removes the value stored at the given index
         *
         * @param i the index to remove at
         * @return iterator to the next element, or pass-the-end
         */
        iterator erase(size_type i);

        /**
         * Removes the value pointed to by an iterator.
         *
         * @param it the iterator whose value to remove
         * @return iterator to the next element, or pass-the-end
         */
        iterator erase(const iterator &it) {
            if (m_size == 0 || !it.m_current) {
                return end();
            }
            node_type *pTmp = it.m_current;
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
            node_type *next = pTmp->m_next;
            free<node_type>(pTmp);
            --m_size;
            return iterator(next, this);
        }

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

        /**
         * Remove the last element from the list.
         */
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

        /**
         * Remove the first element from the list.
         */
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
         * Find the index of the first element from the beginning of
         * the List that equals @code val @endcode. Returns the size
         * of the list if not found.
         *
         * @param val value to search for
         * @return the index of the first occurrence of the element
         */
        size_type index_of(const val_type &val) const;

        /**
         * Find the first occurrence of a value in the list.
         *
         * @param val the value to find
         * @return iterator to the first occurrence of the element
         * or pass-the-end if not found
         */
        iterator find(const val_type &val) {
            node_type *pTmp = m_head;
            while(pTmp) {
                if (pTmp->m_val == val) {
                    return iterator(pTmp, this);
                }
                pTmp = pTmp->m_next;
            }
            return end();
        }

        /**
         * Find the first occurrence of a value in the list.
         *
         * @param val the value to find
         * @return iterator to the first occurrence of the element
         * or pass-the-end if not found
         */
        const_iterator find(const val_type &val) const {
            node_type *pTmp = m_head;
            while(pTmp) {
                if (pTmp->m_val == val) {
                    return const_iterator(pTmp, this);
                }
                pTmp = pTmp->m_next;
            }
            return end();
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
            clear();
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
    inline void LinkedList<T>::clear() noexcept {
        node_type *pTmp;
        while (m_head != nullptr) {
            pTmp = m_head;
            m_head = m_head->m_next;
            free<node_type>(pTmp);
        }
        m_size = 0;
        m_tail = nullptr;
        m_head = nullptr;
    }

    template<typename T>
    typename LinkedList<T>::iterator
    LinkedList<T>::erase(size_type i) {
        if (!m_size) {
            return end();
        }
        if (i >= m_size) {
            i %= m_size;
        }
        node_type *pTmp = m_head;
        while (i-- > 0) {
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
        node_type *next = pTmp->m_next;
        free<node_type>(pTmp);
        m_size--;
        return iterator(next, this);
    }

    template<typename T>
    inline typename LinkedList<T>::val_type &
    LinkedList<T>::at(size_type i) {
        if (i >= m_size) {
            if (m_size) { i %= m_size; }
            else { THROW(INDEX_EXCEPTION("Accessing empty list")) }
        }
        node_type *pTmp = m_head;
        while (i-- > 0) {
            pTmp = pTmp->m_next;
        }
        return pTmp->m_val;
    }

    template<typename T>
    inline const typename LinkedList<T>::val_type &
    LinkedList<T>::at(size_type i) const {
        if (i >= m_size) {
            if (m_size) { i %= m_size; }
            else { THROW(INDEX_EXCEPTION("Accessing empty list")) }
        }
        node_type *pTmp = m_head;
        while (i-- > 0) {
            pTmp = pTmp->m_next;
        }
        return pTmp->m_val;
    }

    template<typename T>
    inline typename LinkedList<T>::size_type
    LinkedList<T>::index_of(const val_type &val) const {
        node_type *pTmp = m_head;
        for (size_type i = 0; i < m_size; i++) {
            if (pTmp->m_val == val) {
                return i;
            }
            pTmp = pTmp->m_next;
        }
        return m_size;
    }

}

#endif // CORE_STL_LIST_H
