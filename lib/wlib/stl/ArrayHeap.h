/*
 * TODO: move assignment/ctors/functions, open access to heap functions
 */

#ifndef EMBEDDEDCPLUSPLUS_ARRAYHEAP_H
#define EMBEDDEDCPLUSPLUS_ARRAYHEAP_H

#include "Comparator.h"
#include "ArrayList.h"

namespace wlp {

    template<typename T, class Cmp = Comparator<T>>
    class ArrayHeap {
    public:
        typedef Cmp comparator;
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

    private:
        void push_heap(iterator first, size_type hole_index, size_type top_index, val_type value);

        void push_heap(iterator first, iterator last) {
            if (last - first < 2) {
                return;
            }
            push_heap(first, static_cast<size_type>(last - first - 1), 0, *(last - 1));
        }

        void adjust_heap(iterator first, size_type hole_index, size_type length, val_type value);

        void pop_heap(iterator first, iterator last, iterator result, val_type value) {
            *result = *first;
            adjust_heap(first, 0, last - first, value);
        }

        void pop_heap(iterator first, iterator last) {
            pop_heap(first, last - 1, last - 1, *(last - 1));
        }

        void make_heap(iterator first, iterator last);

        void sort_heap(iterator first, iterator last);

    public:
        void make() {
            make_heap(m_list.begin(), m_list.end());
        }

        void sort() {
            sort_heap(m_list.begin(), m_list.end());
        }

        void push(val_type value) {
            m_list.push_back(value);
            push_heap(m_list.begin(), m_list.end());
        }

        void push(val_type &value) {
            m_list.push_back(value);
            push_heap(m_list.begin(), m_list.end());
        }

        void pop() {
            pop_heap(m_list.begin(), m_list.end());
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
    };

    template<typename T, class Cmp>
    void ArrayHeap<T, Cmp>::push_heap(
            iterator first,
            size_type hole_index,
            size_type top_index,
            val_type value
    ) {
        size_type parent = static_cast<size_type>((hole_index - 1) / 2);
        while (hole_index > top_index && m_cmp.__lt__(*(first + parent), value)) {
            *(first + hole_index) = *(first + parent);
            hole_index = parent;
            parent = static_cast<size_type>((hole_index - 1) / 2);
        }
        *(first + hole_index) = value;
    }

    template<typename T, class Cmp>
    void ArrayHeap<T, Cmp>::adjust_heap(
            iterator first,
            size_type hole_index,
            size_type length,
            val_type value
    ) {
        size_type top_index = hole_index;
        size_type second_child = static_cast<unsigned short>(2 * hole_index + 2);
        while (second_child < length) {
            if (m_cmp.__lt__(*(first + second_child), *(first + second_child - 1))) {
                --second_child;
            }
            *(first + hole_index) = *(first + second_child);
            hole_index = second_child;
            second_child = static_cast<unsigned short>(2 * (second_child + 1));
        }
        if (second_child == length) {
            *(first + hole_index) = *(first + second_child - 1);
            hole_index = static_cast<unsigned short>(second_child - 1);
        }
        push_heap(first, hole_index, top_index, value);
    }

    template<typename T, class Cmp>
    void ArrayHeap<T, Cmp>::make_heap(
            iterator first,
            iterator last
    ) {
        size_type length = last - first;
        if (length < 2) {
            return;
        }
        size_type parent = static_cast<size_type>((length - 2) / 2);
        for (;;) {
            adjust_heap(first, parent, length, *(first + parent));
            if (parent == 0) {
                return;
            }
            --parent;
        }
    }

    template<typename T, class Cmp>
    void ArrayHeap<T, Cmp>::sort_heap(
            iterator first,
            iterator last
    ) {
        while (last - first > 1) {
            pop_heap(first, last--);
        }
    }

    template<typename T>
    void heap_sort(ArrayList<T> &list) {
        ArrayHeap<T> heap(0);
        heap.get_array_list()->swap(list);
        heap.make();
        heap.sort();
        heap.get_array_list()->swap(list);
    }

}

#endif //EMBEDDEDCPLUSPLUS_ARRAYHEAP_H
