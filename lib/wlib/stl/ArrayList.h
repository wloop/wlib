/**
 * @file ArrayList.h
 * @brief List implementation using an array.
 *
 * This file implements a random access list data structure
 * using an array, and contains associated iterator types.
 *
 * @author Jeff Niu
 * @date November 9, 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDCPLUSPLUS_ARRAYLIST_H
#define EMBEDDEDCPLUSPLUS_ARRAYLIST_H

#include "../Types.h"

#include "../memory/Memory.h"
#include "../utility/Utility.h"

namespace wlp {

    // ArrayList forward declaration.
    template<typename T>
    class ArrayList;

    /**
     * Array list forward iterator type.
     * @tparam T list element type
     */
    template<typename T>
    class ArrayListIterator {
    public:
        typedef wlp::size_type size_type;
        typedef T val_type;
        typedef ArrayList<T> array_list;
        typedef ArrayListIterator<T> iterator;

    private:
        /**
         * The array index pointed to by this iterator.
         */
        size_type m_i;
        /**
         * Pointer to the backing array list.
         */
        array_list *m_list;

        friend class ArrayList<T>;

    public:
        /**
         * An empty array list iterator is invalid.
         */
        ArrayListIterator()
                : m_i(static_cast<size_type>(-1)),
                  m_list(nullptr) {
        }

        /**
         * Copy constructor.
         *
         * @param it iterator to copy
         */
        ArrayListIterator(const iterator &it)
                : m_i(it.m_i),
                  m_list(it.m_list) {
            check_bounds();
        }

        /**
         * Constructor from an array index and
         * a backing array list.
         *
         * @param i array index
         * @param list backing array list
         */
        explicit ArrayListIterator(const size_type &i, array_list *list)
                : m_i(i),
                  m_list(list) {
            check_bounds();
        }

    private:
        /**
         * Ensure the iterator is either exactly
         * pass-the-end of at the beginning.
         */
        void check_bounds() {
            if (m_i > m_list->m_size) {
                m_i = m_list->m_size;
            }
        }

    public:
        /**
         * @return a reference to the value pointed to
         * by this iterator
         */
        val_type &operator*() const {
            return m_list->m_data[m_i];
        }

        /**
         * @return a pointer to the value pointer to
         * by this iterator
         */
        val_type *operator->() const {
            return &(operator*());
        }

        /**
         * Move this iterator to the next element in the
         * backing array list. Becomes a pass-the-end
         * iterator if attempting to move past the
         * end of the array list.
         *
         * @return reference to this iterator
         */
        iterator &operator++() {
            if (m_i == m_list->m_size) {
                return *this;
            }
            ++m_i;
            return *this;
        }

        /**
         * Postfix operator to move this iterator to the
         * next element in the array.
         *
         * @return copy of this iterator before increment
         */
        iterator operator++(int) {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }

        /**
         * Addition assignment operator moves the
         * iterator by the specified number of
         * positions.
         *
         * @param d the number of positions to increment
         * @return reference to this iterator
         */
        iterator &operator+=(const size_type &d) {
            m_i = static_cast<size_type>(m_i + d);
            if (m_i > m_list->m_size) {
                m_i = m_list->m_size;
            }
            return *this;
        }

        /**
         * Decrement operator moves the iterator backwards
         * one element. If the iterator points to the first
         * element, does nothing.
         *
         * @return reference to this iterator
         */
        iterator &operator--() {
            if (m_i == 0) {
                return *this;
            }
            --m_i;
            return *this;
        }

        /**
         * Postfix decrement operator.
         *
         * @return a copy of the iterator before moving
         */
        iterator operator--(int) {
            iterator tmp = *this;
            --*this;
            return tmp;
        }

        /**
         * Subtraction assignment operator moves the iterator
         * backwards a certain number of elements.
         *
         * @param d the number of elements to move back
         * @return reference to this iterator
         */
        iterator &operator-=(const size_type &d) {
            if (d >= m_i) {
                m_i = 0;
            } else {
                m_i = static_cast<size_type>(m_i - d);
            }
            return *this;
        }

        /**
         * @param it iterator to compare
         * @return true if they point to the same element
         */
        bool operator==(const iterator &it) const {
            return m_i == it.m_i;
        }

        /**
         * @param it iterator to compare
         * @return true if they point to different elements
         */
        bool operator!=(const iterator &it) const {
            return m_i != it.m_i;
        }

        /**
         * Copy assignment operator.
         *
         * @param it iterator to copy
         * @return reference to this iterator
         */
        iterator &operator=(const iterator &it) {
            m_i = it.m_i;
            m_list = it.m_list;
            return *this;
        }

        /**
         * Addition operator returns a new iterator
         * incremented by the specified number of positions.
         *
         * @param d number of positions to increment
         * @return
         */
        iterator operator+(const size_type &d) const {
            return iterator(static_cast<size_type>(m_i + d), m_list);
        }

        /**
         * Subtraction operator returns a new iterator
         * decremented by the specified number of positions.
         *
         * @param d number of positions to decrement
         * @return a new iterator
         */
        iterator operator-(const size_type &d) const {
            return iterator(static_cast<size_type>(m_i - d), m_list);
        }

        /**
         * Subtraction of two iterators returns the
         * integer distance between them.
         *
         * @param it iterator to subtract
         * @return the integer distance
         */
        size_type operator-(const iterator &it) const {
            if (m_i < it.m_i) {
                return static_cast<size_type>(it.m_i - m_i);
            }
            return static_cast<size_type>(m_i - it.m_i);
        }

    };

    /**
     * Const iterator for array list.
     *
     * @see ArrayListIterator
     * @tparam T iterator value type
     */
    template<typename T>
    class ArrayListConstIterator {
    public:
        typedef wlp::size_type size_type;
        typedef T val_type;
        typedef ArrayList<T> array_list;
        typedef ArrayListConstIterator<T> const_iterator;

    private:
        size_type m_i;
        const array_list *m_list;

        friend class ArrayList<T>;

    public:
        ArrayListConstIterator()
                : m_i(static_cast<size_type>(-1)),
                  m_list(nullptr) {
        }

        ArrayListConstIterator(const const_iterator &it)
                : m_i(it.m_i),
                  m_list(it.m_list) {
            check_bounds();
        }

        explicit ArrayListConstIterator(const size_type &i, const array_list *list)
                : m_i(i),
                  m_list(list) {
            check_bounds();
        }

    private:
        void check_bounds() {
            if (m_i > m_list->m_size) {
                m_i = m_list->m_size;
            }
        }

    public:

        val_type &operator*() const {
            return m_list->m_data[m_i];
        }

        val_type *operator->() const {
            return &(operator*());
        }

        const_iterator &operator++() {
            if (m_i == m_list->m_size) {
                return *this;
            }
            ++m_i;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++*this;
            return tmp;
        }

        const_iterator &operator+=(const size_type &d) {
            m_i = static_cast<size_type>(m_i + d);
            if (m_i > m_list->m_size) {
                m_i = m_list->m_size;
            }
            return *this;
        }

        const_iterator &operator--() {
            if (m_i == 0) {
                return *this;
            }
            --m_i;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --*this;
            return tmp;
        }

        const_iterator &operator-=(const size_type &d) {
            if (d >= m_i) {
                m_i = 0;
            } else {
                m_i = static_cast<size_type>(m_i - d);
            }
            return *this;
        }

        bool operator==(const const_iterator &it) const {
            return m_i == it.m_i;
        }

        bool operator!=(const const_iterator &it) const {
            return m_i != it.m_i;
        }

        const_iterator &operator=(const const_iterator &it) {
            m_i = it.m_i;
            m_list = it.m_list;
            return *this;
        }

        const_iterator operator+(const size_type &d) const {
            return const_iterator(static_cast<size_type>(m_i + d), m_list);
        }

        const_iterator operator-(const size_type &d) const {
            return const_iterator(static_cast<size_type>(m_i - d), m_list);
        }

        size_type operator-(const const_iterator &it) const {
            if (m_i < it.m_i) {
                return static_cast<size_type>(it.m_i - m_i);
            }
            return static_cast<size_type>(m_i - it.m_i);
        }

    };

    /**
     * List implementation using an array. This implementation
     * will resize if attempting to insert into a full array.
     *
     * @tparam T value type
     */
    template<typename T>
    class ArrayList {
    public:
        typedef wlp::size_type size_type;
        typedef T val_type;
        typedef ArrayList<T> array_list;
        typedef ArrayListIterator<T> iterator;
        typedef ArrayListConstIterator<T> const_iterator;

    private:
        /**
         * The backing array.
         */
        val_type *m_data;
        /**
         * The current number of elements in the list.
         */
        size_type m_size;
        /**
         * The current size of the backing array.
         */
        size_type m_capacity;

        friend class ArrayListIterator<T>;

        friend class ArrayListConstIterator<T>;

    public:
        /**
         * Constructor with an initial capacity for the
         * backing array.
         *
         * @param initial_capacity the initial size of the backing array
         */
        explicit ArrayList(size_type initial_capacity = 12)
                : m_size(0),
                  m_capacity(initial_capacity) {
            init_array(initial_capacity);
        }

        /**
         * Disable copy constructor.
         */
        ArrayList(const array_list &) = delete;

        /**
         * Move constructor.
         *
         * @param list array list whose resources to transfer
         */
        ArrayList(array_list &&list)
                : m_data(move(list.m_data)),
                  m_size(move(list.m_size)),
                  m_capacity(move(list.m_capacity)) {
            list.m_data = nullptr;
            list.m_size = 0;
            list.m_capacity = 0;
        }

        /**
         * Constructor from array. The caller may specify
         * a desired initial capacity, but if the capacity is
         * shorter than the length, then the capacity will be
         * set to the length of the array.
         *
         * @param values array of values
         * @param length length of the array
         */
        ArrayList(const val_type *values, size_type length, size_type initial_capacity)
                : m_size(length),
                  m_capacity(initial_capacity) {
            if (m_capacity < length) {
                m_capacity = length;
            }
            init_array(m_capacity);
            for (size_type i = 0; i < length; i++) {
                m_data[i] = values[i];
            }
        }

        /**
         * Constructor from array. This constructor will set
         * the backing array capacity to be equal to the
         * array length.
         *
         * @param values array of values
         * @param length length of the array
         */
        ArrayList(const val_type *values, size_type length)
                : ArrayList(values, length, length) {
        }

        /**
         * Free the memory for the array unless
         * it has already been deallocated elsewhere.
         */
        ~ArrayList() {
            if (!m_data) {
                return;
            }
            free<val_type>(m_data);
            m_data = nullptr;
        }

    private:
        /**
         * Initialize the backing array. This function
         * performs no initialization of the contents of
         * the array itself.
         *
         * @param initial_size the initial capacity for the backing array
         */
        void init_array(size_type initial_size) {
            m_data = malloc<val_type>(initial_size);
        }

        /**
         * Normalize an index such that it is within
         * the range @code [0, length) @endcode.
         *
         * @param i integer to normalize
         */
        void normalize(size_type &i) const {
            if (m_size == 0) {
                i = 0;
                return;
            }
            i %= m_size;
        }

        /**
         * Called before any insertion operation,
         * this function will extend the size of the
         * array to twice its capacity and copy
         * the elements of the previous array.
         */
        void ensure_capacity();

        /**
         * Shift elements in the array at position @code i @endcode
         * to the right once, such that the element at position
         * @code i @endcode is now present twice in the array.
         *
         * @param i the shift begin position
         */
        void shift_right(size_type i);

        /**
         * Shift elements in the array at position @code i @endcode
         * to the left once, such that the element at position
         * @code i @endcode is erased.
         *
         * @param i the shift begin position
         */
        void shift_left(size_type i);

    public:
        /**
         * @return whether the list is empty
         */
        bool empty() const {
            return m_size == 0;
        }

        /**
         * @return the current number of elements in the list
         */
        size_type size() const {
            return m_size;
        }

        /**
         * @return the size of the backing array
         */
        size_type capacity() const {
            return m_capacity;
        }

        /**
         * Copy the elements in the current array into
         * a new array such that the new array has
         * a size corresponding to the new capacity.
         * If the new capacity is smaller than the current
         * capacity, nothing happens.
         *
         * @param new_capacity the size of backing array to reserve
         */
        void reserve(size_type new_capacity);

        /**
         * Copy the elements of the array into a
         * new array whose capacity is equal to the number
         * of elements in the array.
         */
        void shrink();

        /**
         * Get the element at position @code i @endcode.
         *
         * @param i the index of the element to get
         * @return reference to the element
         */
        val_type &at(size_type i) {
            normalize(i);
            return m_data[i];
        }

        /**
         * Get the element at position @code i @endcode.
         *
         * @param i the index of the element to get
         * @return reference to the element
         */
        val_type const &at(size_type i) const {
            normalize(i);
            return m_data[i];
        }

        /**
         * Access operator returns the element
         * at the specified position without bounds
         * checking.
         *
         * @param i position to access
         * @return reference to the element there
         */
        val_type &operator[](size_type i) {
            return m_data[i];
        }

        /**
         * Access operator returns the element
         * at the specified position without bounds
         * checking.
         *
         * @param i position to access
         * @return reference to the element there
         */
        val_type const &operator[](size_type i) const {
            return m_data[i];
        }

        /**
         * @return reference to the first element in the list
         */
        val_type &front() {
            return m_data[0];
        }

        /**
         * @return reference to the first element in the list
         */
        val_type const &front() const {
            return m_data[0];
        }

        /**
         * @return reference to the last element in the list
         */
        val_type &back() {
            if (m_size == 0) {
                return m_data[0];
            }
            return m_data[m_size - 1];
        }

        /**
         * @return reference to the last element in the list
         */
        val_type const &back() const {
            if (m_size == 0) {
                return m_data[0];
            }
            return m_data[m_size - 1];
        }

        /**
         * @return a pointer to the beginning of the backing array
         */
        val_type *data() {
            return m_data;
        }

        /**
         * @return a pointer to the beginning of the backing array
         */
        const val_type *data() const {
            return m_data;
        }

        /**
         * Clear the contents of the array list
         * such that it is empty.
         */
        void clear() {
            m_size = 0;
        }

        /**
         * @return iterator to the start of the array
         */
        iterator begin() {
            return iterator(0, this);
        }

        /**
         * @return iterator to the start of the array
         */
        const_iterator begin() const {
            return const_iterator(0, this);
        }

        /**
         * @return iterator to the end of the array
         */
        iterator end() {
            return iterator(m_size, this);
        }

        /**
         * @return iterator to the end of the array
         */
        const_iterator end() const {
            return const_iterator(m_size, this);
        }

        /**
         * Insert an element in the array at the specified
         * position such that the previous element at the position
         * and all elements after are shifted to the right.
         *
         * @param i position to insert
         * @param t element to insert
         * @return iterator to the inserted element
         */
        template<typename V>
        iterator insert(size_type i, V &&val) {
            ensure_capacity();
            normalize(i);
            shift_right(i);
            m_data[i] = forward<V>(val);
            ++m_size;
            return iterator(i, this);
        }

        /**
         * Insert an element at the position pointed to by
         * the iterator.
         *
         * @param it iterator to the inserted position
         * @param t element to insert
         * @return iterator to the inserted element
         */
        template<typename V>
        iterator insert(const iterator &it, V &&val) {
            ensure_capacity();
            shift_right(it.m_i);
            m_data[it.m_i] = forward<V>(val);
            ++m_size;
            return it;
        }

        /**
         * Remove the element at the specified position.
         *
         * @param i position whose element to erase.
         * @return iterator to the next element in the list
         */
        iterator erase(size_type i) {
            if (m_size == 0) {
                return end();
            }
            normalize(i);
            shift_left(i);
            --m_size;
            return iterator(i, this);
        }

        /**
         * Remove the element at the specified position.
         *
         * @param it position whose element to erase.
         * @return iterator to the next element in the list
         */
        iterator erase(const iterator &it) {
            if (m_size == 0 || it.m_i >= m_size) {
                return end();
            }
            shift_left(it.m_i);
            --m_size;
            return it;
        }

        /**
         * Insert an element to the back of the list.
         *
         * @param t element to insert
         */
        template<typename V>
        void push_back(V &&val) {
            ensure_capacity();
            m_data[m_size] = forward<V>(val);
            ++m_size;
        }

        /**
         * Remove the last element from the list.
         */
        void pop_back() {
            if (m_size > 0) {
                --m_size;
            }
        }

        /**
         * Swap the contents of two array lists.
         *
         * @param list array list with which to swap
         */
        void swap(array_list &list) {
            val_type *tmp = m_data;
            m_data = list.m_data;
            list.m_data = tmp;
            size_type tmp_size = m_size;
            m_size = list.m_size;
            list.m_size = tmp_size;
            size_type tmp_cap = m_capacity;
            m_capacity = list.m_capacity;
            list.m_capacity = tmp_cap;
        }

        /**
         * Disable copy assignment.
         *
         * @return reference to this list
         */
        array_list &operator=(const array_list &) = delete;

        /**
         * Move assignment operator.
         *
         * @param list array list to transfer
         * @return reference to this list
         */
        array_list &operator=(array_list &&list) {
            free<val_type>(m_data);
            m_data = move(list.m_data);
            m_size = move(list.m_size);
            m_capacity = move(list.m_capacity);
            list.m_data = nullptr;
            list.m_size = 0;
            list.m_capacity = 0;
            return *this;
        }

    };

    template<typename T>
    void ArrayList<T>::ensure_capacity() {
        if (m_size < m_capacity) {
            return;
        }
        size_type new_capacity =  static_cast<size_type>(2 * m_capacity);
        val_type *new_data = malloc<val_type>(new_capacity);
        for (size_type i = 0; i < m_size; i++) {
            new_data[i] = m_data[i];
        }
        free<val_type>(m_data);
        m_data = new_data;
        m_capacity = new_capacity;
    }

    template<typename T>
    void ArrayList<T>::reserve(size_type new_capacity) {
        if (new_capacity <= m_capacity) {
            return;
        }
        val_type *new_data = malloc<val_type>(new_capacity);
        for (size_type i = 0; i < m_size; i++) {
            new_data[i] = m_data[i];
        }
        free<val_type>(m_data);
        m_data = new_data;
        m_capacity = new_capacity;
    }

    template<typename T>
    void ArrayList<T>::shrink() {
        if (m_size == m_capacity) {
            return;
        }
        val_type *new_data = malloc<val_type>(m_size);
        for (size_type i = 0; i < m_size; i++) {
            new_data[i] = m_data[i];
        }
        free<val_type>(m_data);
        m_data = new_data;
        m_capacity = m_size;
    }

    template<typename T>
    inline void ArrayList<T>::shift_right(size_type i) {
        for (size_type j = m_size; j > i; j--) {
            m_data[j] = m_data[j - 1];
        }
    }

    template<typename T>
    inline void ArrayList<T>::shift_left(size_type i) {
        for (size_type j = i; j < m_size - 1; j++) {
            m_data[j] = m_data[j + 1];
        }
    }

}

#endif //EMBEDDEDCPLUSPLUS_ARRAYLIST_H
