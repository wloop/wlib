#ifndef EMBEDDEDCPLUSPLUS_ARRAYHEAP_H
#define EMBEDDEDCPLUSPLUS_ARRAYHEAP_H

#include "ArrayList.h"
#include "Comparator.h"
#include "Concept.h"
#include "TypeTraits.h"

namespace wlp {

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
        SizeType length = last - first;
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
        SizeType length = last - first;
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

    template<typename T, class Cmp = Comparator<T>>
    class ArrayHeap {
    public:
        typedef Cmp comparator;
        typedef ArrayHeap<T> array_heap;
        typedef typename ArrayList<T>::val_type val_type;
        typedef typename ArrayList<T>::size_type size_type;
        typedef typename ArrayList<T>::array_list array_list;
        typedef typename ArrayList<T>::iterator iterator;
        typedef typename ArrayList<T>::const_iterator const_iterator;

    private:
        array_list m_list;
        comparator m_cmp;

    public:
        explicit ArrayHeap(size_type initial_capacity = 12)
                : m_list(initial_capacity),
                  m_cmp(Cmp()) {
        }

        ArrayHeap(const array_heap &) = delete;

        ArrayHeap(array_heap &&heap)
                : m_list(move(heap.m_list)),
                  m_cmp(move(heap.m_cmp)) {
        }

    private:

    public:
        void make() {
            make_heap(m_list.begin(), m_list.end(), m_cmp);
        }

        void sort() {
            sort_heap(m_list.begin(), m_list.end(), m_cmp);
        }

        void push(const val_type &value) {
            m_list.push_back(value);
            push_heap(m_list.begin(), m_list.end(), m_cmp);
        }

        void push(val_type &&value) {
            m_list.push_back(forward<val_type>(value));
            push_heap(m_list.begin(), m_list.end(), m_cmp);
        }

        void pop() {
            pop_heap(m_list.begin(), m_list.end(), m_cmp);
            m_list.pop_back();
        }

        const val_type &top() {
            return m_list.front();
        }

        bool empty() const {
            return m_list.empty();
        }

        size_type size() const {
            return m_list.size();
        }

        size_type capacity() const {
            return m_list.capacity();
        }

        array_list *get_array_list() {
            return &m_list;
        }

        array_heap &operator=(const array_heap &) = delete;

        array_heap &operator=(array_heap &&heap) {
            m_list = move(heap.m_list);
            return *this;
        }
    };

    template<typename T>
    void heap_sort(ArrayList<T> &list) {
        make_heap(list.begin(), list.end());
        sort_heap(list.begin(), list.end());
    }

    template<typename T, typename Cmp>
    void heap_sort(ArrayList<T> &list, Cmp cmp) {
        make_heap(list.begin(), list.end(), cmp);
        sort_heap(list.begin(), list.end(), cmp);
    };

}

#endif //EMBEDDEDCPLUSPLUS_ARRAYHEAP_H
