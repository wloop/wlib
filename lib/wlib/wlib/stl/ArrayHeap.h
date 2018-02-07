/**
 * @file ArrayHeap.h
 * @brief Heap manipulation and implementation.
 *
 * This file contains generic heap manipulation functions
 * for data structures that supply a @code RandomAccessIterator @endcode
 * and a general @code ArrayHeap @endcode implementation using
 * @code ArrayList @endcode.
 *
 * @author Jeff Niu
 * @date November 9, 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDCPLUSPLUS_ARRAYHEAP_H
#define EMBEDDEDCPLUSPLUS_ARRAYHEAP_H

#include "ArrayList.h"
#include "Comparator.h"
#include "Concept.h"
#include "TypeTraits.h"

#include "wlib/utility/Utility.h"

namespace wlp {

    /**
     * Push heap helper function. Takes a value located
     * at @code hole_index @endcode and inserts into the
     * heap, preserving heap property. This version uses
     * a supplied comparator.
     *
     * @tparam RandomAccessIterator random access iterator type
     * @tparam SizeType integer size type
     * @tparam ValType the type of the inserted value
     * @tparam Cmp a comparator type
     * @param first iterator to the first element in the backing structure
     * @param hole_index the index of the inserted element
     * @param top_index the index of the first element in the heap
     * @param value the value to insert
     * @param cmp the comparator use for @code __lt__ @endcode
     */
    template<
            typename RandomAccessIterator,
            typename SizeType,
            typename ValType,
            typename Cmp
    >
    void __push_heap(
            RandomAccessIterator first,
            SizeType hole_index,
            SizeType top_index,
            ValType value,
            Cmp cmp
    ) {
        SizeType parent = static_cast<SizeType>((hole_index - 1) / 2);
        while (hole_index > top_index && cmp.__lt__(*(first + parent), value)) {
            *(first + hole_index) = *(first + parent);
            hole_index = parent;
            parent = static_cast<SizeType>((hole_index - 1) / 2);
        }
        *(first + hole_index) = value;
    };

    /**
     * Push heap helper function. Takes a value located
     * at @code hole_index @endcode and inserts into the
     * heap, preserving heap property.
     *
     * @tparam RandomAccessIterator random access iterator type
     * @tparam SizeType integer size type
     * @tparam ValType the type of the inserted value
     * @param first iterator to the first element in the backing structure
     * @param hole_index the index of the inserted element
     * @param top_index the index of the first element in the heap
     * @param value the value to insert
     */
    template<
            typename RandomAccessIterator,
            typename SizeType,
            typename ValType
    >
    void __push_heap(
            RandomAccessIterator first,
            SizeType hole_index,
            SizeType top_index,
            ValType value
    ) {
        SizeType parent = static_cast<SizeType>((hole_index - 1) / 2);
        while (hole_index > top_index && *(first + parent) < value) {
            *(first + hole_index) = *(first + parent);
            hole_index = parent;
            parent = static_cast<SizeType>((hole_index - 1) / 2);
        }
        *(first + hole_index) = value;
    };

    /**
     * Insert a value located at the end of an array-type
     * data structure into a heap structure at @code [first, last) @endcode.
     * This function assumes the aforementioned range is already a heap.
     *
     * @tparam RandomAccessIterator random access iterator type
     * @tparam Cmp comparator type
     * @tparam SizeType integer size type, acquired from the iterator type
     * @tparam ValType type of the inserted value, acquired from the iterator type
     * @param first iterator to the first element in the backing array
     * @param last iterator to the element to be inserted
     * @param cmp comparator to use
     */
    template<
            typename RandomAccessIterator,
            typename Cmp,
            typename SizeType = typename obtain_size_type<RandomAccessIterator>::type,
            typename ValType = typename obtain_val_type<RandomAccessIterator>::type,
            typename = typename enable_if<
                    is_random_access_iterator<RandomAccessIterator>() &&
                    is_comparator<Cmp, ValType>()
            >::type
    >
    void push_heap(
            RandomAccessIterator first,
            RandomAccessIterator last,
            Cmp cmp
    ) {
        __push_heap(first, static_cast<SizeType>(last - first - 1), (SizeType) 0, ValType(*(last - 1)), cmp);
    }

    /**
     * Insert a value located at the end of an array-type
     * data structure into a heap structure at @code [first, last) @endcode.
     * This function assumes the aforementioned range is already a heap.
     *
     * @tparam RandomAccessIterator random access iterator type
     * @tparam SizeType integer size type, acquired from the iterator type
     * @tparam ValType type of the inserted value, acquired from the iterator type
     * @param first iterator to the first element in the backing array
     * @param last iterator to the element to be inserted
     */
    template<
            typename RandomAccessIterator,
            typename = typename enable_if<
                    is_random_access_iterator<RandomAccessIterator>()
            >::type,
            typename SizeType = typename obtain_size_type<RandomAccessIterator>::type,
            typename ValType = typename obtain_val_type<RandomAccessIterator>::type
    >
    void push_heap(
            RandomAccessIterator first,
            RandomAccessIterator last
    ) {
        __push_heap(first, static_cast<SizeType>(last - first - 1), (SizeType) 0, ValType(*(last - 1)));
    }

    /**
     * Helper function performs heapify at the specified index.
     *
     * @tparam RandomAccessIterator random access iterator type
     * @tparam SizeType integer size type
     * @tparam ValType value type for insertion
     * @tparam Cmp comparator type
     * @param first iterator to first element in the random access structure
     * @param hole_index index of the value to be heapified
     * @param length length of the heap section of the structure
     * @param value value to heapify
     * @param cmp comparator to use
     */
    template<
            typename RandomAccessIterator,
            typename SizeType,
            typename ValType,
            typename Cmp
    >
    void __adjust_heap(
            RandomAccessIterator first,
            SizeType hole_index,
            SizeType length,
            ValType value,
            Cmp cmp
    ) {
        SizeType top_index = hole_index;
        SizeType second_child = static_cast<SizeType>(2 * hole_index + 2);
        while (second_child < length) {
            if (cmp.__lt__(*(first + second_child), *(first + second_child - 1))) {
                --second_child;
            }
            *(first + hole_index) = *(first + second_child);
            hole_index = second_child;
            second_child = static_cast<SizeType>(2 * (second_child + 1));
        }
        if (second_child == length) {
            *(first + hole_index) = *(first + second_child - 1);
            hole_index = static_cast<SizeType>(second_child - 1);
        }
        __push_heap(first, hole_index, top_index, value, cmp);
    };

    /**
     * Helper function performs heapify at the specified index.
     *
     * @tparam RandomAccessIterator random access iterator type
     * @tparam SizeType integer size type
     * @tparam ValType value type for insertion
     * @param first iterator to first element in the random access structure
     * @param hole_index index of the value to be heapified
     * @param length length of the heap section of the structure
     * @param value value to heapify
     */
    template<
            typename RandomAccessIterator,
            typename SizeType,
            typename ValType
    >
    void __adjust_heap(
            RandomAccessIterator first,
            SizeType hole_index,
            SizeType length,
            ValType value
    ) {
        SizeType top_index = hole_index;
        SizeType second_child = static_cast<SizeType>(2 * hole_index + 2);
        while (second_child < length) {
            if (*(first + second_child) < *(first + second_child - 1)) {
                --second_child;
            }
            *(first + hole_index) = *(first + second_child);
            hole_index = second_child;
            second_child = static_cast<SizeType>(2 * (second_child + 1));
        }
        if (second_child == length) {
            *(first + hole_index) = *(first + second_child - 1);
            hole_index = static_cast<SizeType>(second_child - 1);
        }
        __push_heap(first, hole_index, top_index, value);
    };

    /**
     * Helper function to pop an element from the heap.
     *
     * @tparam RandomAccessIterator random access iterator type
     * @tparam Cmp comparator type
     * @tparam SizeType integer size type
     * @tparam ValType value type
     * @param first iterator to first element in random access structure
     * @param last iterator to last element in random access structure
     * @param result iterator to the location where the popped element will be stored
     * @param value the value that is popped
     * @param cmp comparator to use
     */
    template<
            typename RandomAccessIterator,
            typename Cmp,
            typename SizeType = typename obtain_size_type<RandomAccessIterator>::type,
            typename ValType
    >
    void __pop_heap(
            RandomAccessIterator first,
            RandomAccessIterator last,
            RandomAccessIterator result,
            ValType value,
            Cmp cmp
    ) {
        *result = *first;
        __adjust_heap(first, (SizeType) 0, (SizeType) (last - first), value, cmp);
    };

    /**
     * Helper function to pop an element from the heap.
     *
     * @tparam RandomAccessIterator random access iterator type
     * @tparam SizeType integer size type
     * @tparam ValType value type
     * @param first iterator to first element in random access structure
     * @param last iterator to last element in random access structure
     * @param result iterator to the location where the popped element will be stored
     * @param value the value that is popped
     */
    template<
            typename RandomAccessIterator,
            typename SizeType = typename obtain_size_type<RandomAccessIterator>::type,
            typename ValType
    >
    void __pop_heap(
            RandomAccessIterator first,
            RandomAccessIterator last,
            RandomAccessIterator result,
            ValType value
    ) {
        *result = *first;
        __adjust_heap(first, (SizeType) 0, (SizeType) (last - first), value);
    };

    /**
     * Pop an element from heap ranging from @code [first, last) @endcode
     * such that the popped element is then stored at @code last @endcode
     * and the heap now ranges @code [first, last - 1) @endcode.
     *
     * @tparam RandomAccessIterator random access iterator type
     * @tparam Cmp comparator type
     * @tparam ValType value type acquired from the iterator type
     * @param first iterator to first element in random access structure
     * @param last iterator to last element in random access structure
     * @param cmp comparator to use
     */
    template<
            typename RandomAccessIterator,
            typename Cmp,
            typename ValType = typename obtain_val_type<RandomAccessIterator>::type,
            typename = typename enable_if<
                    is_random_access_iterator<RandomAccessIterator>() &&
                    is_comparator<Cmp, ValType>()
            >::type
    >
    void pop_heap(
            RandomAccessIterator first,
            RandomAccessIterator last,
            Cmp cmp
    ) {
        __pop_heap(first, last - 1, last - 1, (ValType) *(last - 1), cmp);
    };

    /**
     * Pop an element from heap ranging from @code [first, last) @endcode
     * such that the popped element is then stored at @code last @endcode
     * and the heap now ranges @code [first, last - 1) @endcode.
     *
     * @tparam RandomAccessIterator random access iterator type
     * @tparam ValType value type acquired from the iterator type
     * @param first iterator to first element in random access structure
     * @param last iterator to last element in random access structure
     */
    template<
            typename RandomAccessIterator,
            typename = typename enable_if<
                    is_random_access_iterator<RandomAccessIterator>()
            >::type,
            typename ValType = typename obtain_val_type<RandomAccessIterator>::type
    >
    void pop_heap(
            RandomAccessIterator first,
            RandomAccessIterator last
    ) {
        __pop_heap(first, last - 1, last - 1, (ValType) *(last - 1));
    };

    /**
     * Make heap will traverse a random access structure
     * (typically an array or vector) and create a heap
     * that ranges from @code [first, last) @endcode.
     *
     * @tparam RandomAccessIterator random access iterator type
     * @tparam Cmp comparator type
     * @tparam SizeType size type acquired from iterator type
     * @tparam ValType value type acquired from iterator type
     * @param first iterator to first element in random access structure
     * @param last iterator to last element in random acces structure
     * @param cmp comparator to use
     */
    template<
            typename RandomAccessIterator,
            typename Cmp,
            typename SizeType = typename obtain_size_type<RandomAccessIterator>::type,
            typename ValType = typename obtain_val_type<RandomAccessIterator>::type,
            typename = typename enable_if<
                    is_random_access_iterator<RandomAccessIterator>() &&
                    is_comparator<Cmp, ValType>()
            >::type
    >
    void make_heap(
            RandomAccessIterator first,
            RandomAccessIterator last,
            Cmp cmp
    ) {
        SizeType length = static_cast<SizeType>(last - first);
        if (length < 2) {
            return;
        }
        SizeType parent = static_cast<SizeType>((length - 2) / 2);
        for (;;) {
            __adjust_heap(first, parent, length, (ValType) *(first + parent), cmp);
            if (parent == 0) {
                return;
            }
            --parent;
        }
    };

    /**
     * Make heap will traverse a random access structure
     * (typically an array or vector) and create a heap
     * that ranges from @code [first, last) @endcode.
     *
     * @tparam RandomAccessIterator random access iterator type
     * @tparam SizeType size type acquired from iterator type
     * @tparam ValType value type acquired from iterator type
     * @param first iterator to first element in random access structure
     * @param last iterator to last element in random acces structure
     */
    template<
            typename RandomAccessIterator,
            typename SizeType = typename obtain_size_type<RandomAccessIterator>::type,
            typename ValType = typename obtain_val_type<RandomAccessIterator>::type,
            typename = typename enable_if<
                    is_random_access_iterator<RandomAccessIterator>()
            >::type
    >
    void make_heap(
            RandomAccessIterator first,
            RandomAccessIterator last
    ) {
        SizeType length = static_cast<SizeType>(last - first);
        if (length < 2) {
            return;
        }
        SizeType parent = static_cast<SizeType>((length - 2) / 2);
        for (;;) {
            __adjust_heap(first, parent, length, (ValType) *(first + parent));
            if (parent == 0) {
                return;
            }
            --parent;
        }
    };

    /**
     * Rearrange the elements in a heap from @code [first, last) @endcode
     * such that they are ordered from smallest to largest between
     * @code [first, last) @endcode. This eliminates the heap property.
     *
     * @tparam RandomAccessIterator random access iterator type
     * @tparam Cmp comparator type
     * @param first iterator to first element in heap
     * @param last iterator to last element in heap
     * @param cmp comparator type
     */
    template<
            typename RandomAccessIterator,
            typename Cmp,
            typename = typename enable_if<
                    is_random_access_iterator<RandomAccessIterator>() &&
                    is_comparator<Cmp, typename obtain_val_type<RandomAccessIterator>::type>()
            >::type>
    void sort_heap(
            RandomAccessIterator first,
            RandomAccessIterator last,
            Cmp cmp
    ) {
        while (last - first > 1) {
            pop_heap(first, last--, cmp);
        }
    };

    /**
     * Rearrange the elements in a heap from @code [first, last) @endcode
     * such that they are ordered from smallest to largest between
     * @code [first, last) @endcode. This eliminates the heap property.
     *
     * @tparam RandomAccessIterator random access iterator type
     * @param first iterator to first element in heap
     * @param last iterator to last element in heap
     */
    template<
            typename RandomAccessIterator,
            typename = typename enable_if<
                    is_random_access_iterator<RandomAccessIterator>()
            >::type>
    void sort_heap(
            RandomAccessIterator first,
            RandomAccessIterator last
    ) {
        while (last - first > 1) {
            pop_heap(first, last--);
        }
    };

    /**
     * A basic heap implementation using @code ArrayList @endcode
     * as the backing structure. May be used as a priority queue.
     *
     * @tparam T data type
     * @tparam Cmp comparator type, which uses the default
     */
    template<typename T, class Cmp = Comparator<T>>
    class ArrayHeap {
    public:
        typedef Cmp comparator;
        typedef ArrayHeap<T> array_heap;
        typedef typename ArrayList<T>::val_type val_type;
        typedef typename ArrayList<T>::size_type size_type;
        typedef typename ArrayList<T>::list_type array_list;
        typedef typename ArrayList<T>::iterator iterator;
        typedef typename ArrayList<T>::const_iterator const_iterator;

    private:
        /**
         * The backing array list.
         */
        array_list m_list;
        /**
         * The comparator instance.
         */
        comparator m_cmp;

    public:
        /**
         * Constructor with a specified initial capacity for the
         * backing array list.
         *
         * @param initial_capacity initial capacity of the backing array
         */
        explicit ArrayHeap(size_type initial_capacity = 12)
                : m_list(initial_capacity),
                  m_cmp(Cmp()) {
        }

        /**
         * Disable copy construction.
         */
        ArrayHeap(const array_heap &) = delete;

        /**
         * Move copy constructor.
         *
         * @param heap array heap whose resources to transfer
         */
        ArrayHeap(array_heap &&heap)
                : m_list(move(heap.m_list)),
                  m_cmp(move(heap.m_cmp)) {
        }

        /**
         * Push an element onto the heap.
         *
         * @param value value to insert
         */
        void push(const val_type &value) {
            m_list.push_back(value);
            push_heap(m_list.begin(), m_list.end(), m_cmp);
        }

        /**
         * Push an rvalue onto the heap.
         *
         * @param value rvalue to insert
         */
        void push(val_type &&value) {
            m_list.push_back(forward<val_type>(value));
            push_heap(m_list.begin(), m_list.end(), m_cmp);
        }

        /**
         * Pop the top element from the heap.
         */
        void pop() {
            pop_heap(m_list.begin(), m_list.end(), m_cmp);
            m_list.pop_back();
        }

        /**
         * Get a reference to the top element on the heap.
         * Modification of the element is disabled in order
         * to preserve heap property.
         *
         * @return reference to the top element
         */
        const val_type &top() {
            return m_list.front();
        }

        /**
         * @return whether the heap is empty
         */
        bool empty() const {
            return m_list.empty();
        }

        /**
         * @return the number of elements in the heap
         */
        size_type size() const {
            return m_list.size();
        }

        /**
         * @return the size of the backing array
         */
        size_type capacity() const {
            return m_list.capacity();
        }

        /**
         * @return a pointer to the backing array list
         */
        array_list *get_array_list() {
            return &m_list;
        }

        /**
         * Disable copy assignment.
         *
         * @return reference to this heap
         */
        array_heap &operator=(const array_heap &) = delete;

        /**
         * Move assignment operator.
         *
         * @param heap array heap whose resources to transfer
         * @return reference to this heap
         */
        array_heap &operator=(array_heap &&heap) {
            m_list = move(heap.m_list);
            return *this;
        }
    };

    /**
     * Sort an array list using heap sort.
     * This function uses the array list
     * elements' default ordering operations,
     * which may be overloaded.
     *
     * @tparam T list element type, inferred from array list
     * @param list array list to sort
     */
    template<typename T>
    void heap_sort(ArrayList<T> &list) {
        make_heap(list.begin(), list.end());
        sort_heap(list.begin(), list.end());
    }

    /**
     * Sort an array list using heap sort.
     * This function uses a supplied comparator
     * type for the array list element
     *
     * @tparam T list element type, inferred from array list
     * @tparam Cmp comparator type
     * @param list array list to sort
     * @param cmp comparator to use
     */
    template<typename T, typename Cmp>
    void heap_sort(ArrayList<T> &list, Cmp cmp) {
        make_heap(list.begin(), list.end(), cmp);
        sort_heap(list.begin(), list.end(), cmp);
    };

}

#endif //EMBEDDEDCPLUSPLUS_ARRAYHEAP_H
