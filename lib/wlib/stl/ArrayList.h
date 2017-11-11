/*
 * TODO: move assignment/constructor when Tmp.h is merged
 */

#ifndef EMBEDDEDCPLUSPLUS_ARRAYLIST_H
#define EMBEDDEDCPLUSPLUS_ARRAYLIST_H

#include <stdint.h>

#include "Tmp.h"
#include "../memory/Memory.h"

namespace wlp {

    template<typename T>
    class ArrayList;

    template<typename T>
    class ArrayListIterator {
    public:
        typedef uint16_t size_type;
        typedef T val_type;
        typedef ArrayList<T> array_list;
        typedef ArrayListIterator<T> iterator;

    private:
        size_type m_i;
        array_list *m_list;

        friend class ArrayList<T>;

    public:
        ArrayListIterator()
                : m_i(static_cast<size_type>(-1)),
                  m_list(nullptr) {
        }

        ArrayListIterator(const iterator &it)
                : m_i(it.m_i),
                  m_list(it.m_list) {
        }

        ArrayListIterator(iterator &&it)
                : m_i(move(it.m_i)),
                  m_list(move(it.m_list)) {
        }

        explicit ArrayListIterator(const size_type &i, array_list *list)
                : m_i(i),
                  m_list(list) {
        }

        explicit ArrayListIterator(size_type &&i, array_list *list)
                : m_i(move(i)),
                  m_list(list) {

        }

        val_type &operator*() const {
            return m_list->m_data[m_i];
        }

        val_type *operator->() const {
            return &(operator*());
        }

        iterator &operator++() {
            if (m_i == m_list->m_size) {
                return *this;
            }
            ++m_i;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }

        iterator &operator+=(const size_type &d) {
            m_i += d;
            if (m_i > m_list->m_size) {
                m_i = m_list->m_size;
            }
            return *this;
        }

        iterator &operator+=(size_type &&d) {
            m_i += d;
            if (m_i > m_list->m_size) {
                m_i = m_list->m_size;
            }
            return *this;
        }

        iterator &operator--() {
            if (m_i == 0) {
                return *this;
            }
            --m_i;
            return *this;
        }

        iterator operator--(int) {
            iterator tmp = *this;
            --*this;
            return tmp;
        }

        iterator &operator-=(const size_type &d) {
            if (d >= m_i) {
                m_i = 0;
            } else {
                m_i -= d;
            }
            return *this;
        }

        iterator &operator-=(size_type &&d) {
            if (d >= m_i) {
                m_i = 0;
            } else {
                m_i -= d;
            }
            return *this;
        }

        bool operator==(iterator &it) const {
            return m_i == it.m_i;
        }

        bool operator==(const iterator &it) const {
            return m_i == it.m_i;
        }

        bool operator!=(iterator &it) const {
            return m_i != it.m_i;
        }

        bool operator!=(const iterator &it) const {
            return m_i != it.m_i;
        }

        iterator &operator=(iterator &&it) {
            m_i = move(it.m_i);
            m_list = move(it.m_list);
            return *this;
        }

        iterator &operator=(const iterator &it) {
            m_i = it.m_i;
            m_list = it.m_list;
            return *this;
        }

        iterator operator+(const size_type &d) const {
            return iterator(move((size_type) (m_i + d)), m_list);
        }

        iterator operator+(size_type &&d) const {
            return iterator(move((size_type) (m_i + d)), m_list);
        }

        iterator operator-(const size_type &d) const {
            return iterator(move((size_type) (m_i - d)), m_list);
        }

        iterator operator-(size_type &&d) const {
            return iterator(move((size_type) (m_i - d)), m_list);
        }

        size_type operator-(const iterator &it) const {
            return m_i - it.m_i;
        }

        size_type operator-(iterator &&it) const {
            return m_i - it.m_i;
        }
    };

    template<typename T>
    class ArrayListConstIterator {
    public:
        typedef uint16_t size_type;
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
        }

        ArrayListConstIterator(const_iterator &&it)
                : m_i(move(it.m_i)),
                  m_list(move(it.m_list)) {
        }

        explicit ArrayListConstIterator(const size_type &i, const array_list *list)
                : m_i(i),
                  m_list(list) {
        }

        explicit ArrayListConstIterator(size_type &&i, const array_list *list)
                : m_i(move(i)),
                  m_list(list) {
        }

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
            m_i += d;
            if (m_i > m_list->m_size) {
                m_i = m_list->m_size;
            }
            return *this;
        }

        const_iterator &operator+=(size_type &&d) {
            m_i += d;
            if (m_i > m_list->m_size) {
                m_i = m_list->m_size;
            }
            return *this;
        }

        const_iterator &operator--() {
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
                m_i -= d;
            }
            return *this;
        }

        const_iterator &operator-=(size_type &&d) {
            if (d >= m_i) {
                m_i = 0;
            } else {
                m_i -= d;
            }
            return *this;
        }

        bool operator==(const_iterator &it) const {
            return m_i == it.m_i;
        }

        bool operator==(const const_iterator &it) const {
            return m_i == it.m_i;
        }

        bool operator!=(const_iterator &it) const {
            return m_i != it.m_i;
        }

        bool operator!=(const const_iterator &it) const {
            return m_i != it.m_i;
        }

        const_iterator &operator=(const_iterator &&it) {
            m_i = move(it.m_i);
            m_list = move(it.m_list);
            return *this;
        }

        const_iterator &operator=(const const_iterator &it) {
            m_i = it.m_i;
            m_list = it.m_list;
            return *this;
        }

        const_iterator operator+(const size_type &d) const {
            return const_iterator(move((size_type) (m_i + d)), m_list);
        }

        const_iterator operator+(size_type &&d) const {
            return const_iterator(move((size_type) (m_i + d)), m_list);
        }

        const_iterator operator-(const size_type &d) const {
            return const_iterator(move((size_type) (m_i - d)), m_list);
        }

        const_iterator operator-(size_type &&d) const {
            return const_iterator(move((size_type) (m_i - d)), m_list);
        }

        size_type operator-(const const_iterator &it) const {
            return m_i - it.m_i;
        }
    };

    template<typename T>
    class ArrayList {
    public:
        typedef uint16_t size_type;
        typedef T val_type;
        typedef ArrayList<T> array_list;
        typedef ArrayListIterator<T> iterator;
        typedef ArrayListConstIterator<T> const_iterator;

    private:
        val_type *m_data;
        size_type m_size;
        size_type m_capacity;

        friend class ArrayListIterator<T>;

        friend class ArrayListConstIterator<T>;

    public:
        explicit ArrayList(size_type initial_capacity = 12)
                : m_size(0),
                  m_capacity(initial_capacity) {
            init_array(initial_capacity);
        }

        ArrayList(const array_list &) = delete;

        ArrayList(array_list &&list)
                : m_data(move(list.m_data)),
                  m_size(move(list.m_size)),
                  m_capacity(move(list.m_capacity)) {
            list.m_data = nullptr;
            list.m_size = 0;
            list.m_capacity = 0;
        }

        ~ArrayList() {
            if (!m_data) {
                return;
            }
            memory_free(m_data);
            m_data = nullptr;
        }

    private:
        void init_array(size_type initial_size) {
            m_data = static_cast<val_type *>(memory_alloc(initial_size * sizeof(val_type)));
        }

        void normalize(size_type &i) const {
            if (m_size == 0) {
                i = 0;
                return;
            }
            i %= m_size;
            if (i < 0) {
                i += m_size;
            }
        }

        void ensure_capacity();

        void shift_right(size_type i);

        void shift_left(size_type i);

    public:
        bool empty() const {
            return m_size == 0;
        }

        size_type size() const {
            return m_size;
        }

        size_type capacity() const {
            return m_capacity;
        }

        void reserve(size_type new_capacity);

        void shrink();

        val_type &at(size_type i) {
            normalize(i);
            return m_data[i];
        }

        val_type const &at(size_type i) const {
            normalize(i);
            return m_data[i];
        }

        val_type &operator[](size_type i) {
            return m_data[i];
        }

        val_type const &operator[](size_type i) const {
            return m_data[i];
        }

        val_type &front() {
            return m_data[0];
        }

        val_type const &front() const {
            return m_data[0];
        }

        val_type &back() {
            return m_data[m_size - 1];
        }

        val_type const &back() const {
            return m_data[m_size - 1];
        }

        val_type *data() {
            return m_data;
        }

        const val_type *data() const {
            return m_data;
        }

        void clear() {
            m_size = 0;
        }

        iterator begin() {
            return iterator(0, this);
        }

        const_iterator begin() const {
            return const_iterator(0, this);
        }

        iterator end() {
            return iterator(m_size, this);
        }

        const_iterator end() const {
            return const_iterator(m_size, this);
        }

        iterator insert(size_type i, const val_type &t) {
            ensure_capacity();
            normalize(i);
            shift_right(i);
            m_data[i] = t;
            ++m_size;
            return iterator(i, this);
        }

        iterator insert(size_type i, val_type &&t) {
            ensure_capacity();
            normalize(i);
            shift_right(i);
            m_data[i] = forward<val_type>(t);
            ++m_size;
            return iterator(i, this);
        }

        iterator insert(iterator &it, const val_type &t) {
            ensure_capacity();
            shift_right(it.m_i);
            m_data[it.m_i] = t;
            ++m_size;
            return it;
        }

        iterator insert(iterator &it, val_type &&t) {
            ensure_capacity();
            shift_right(it.m_i);
            m_data[it.m_i] = t;
            ++m_size;
            return it;
        }

        iterator insert(const_iterator &it, const val_type &t) {
            ensure_capacity();
            shift_right(it.m_i);
            m_data[it.m_i] = t;
            ++m_size;
            return iterator(it.m_i, this);
        }

        iterator insert(const_iterator &it, val_type &&t) {
            ensure_capacity();
            shift_right(it.m_i);
            m_data[it.m_i] = t;
            ++m_size;
            return iterator(it.m_i, this);
        }

        iterator erase(size_type i) {
            if (m_size == 0) {
                return end();
            }
            normalize(i);
            shift_left(i);
            return iterator(i, this);
        }

        iterator erase(iterator &it) {
            if (m_size == 0) {
                return end();
            }
            shift_left(it.m_i);
            return it;
        }

        iterator erase(const_iterator &it) {
            if (m_size == 0) {
                return end();
            }
            shift_left(it.m_i);
            return iterator(it.m_i, this);
        }

        void push_back(const val_type &t) {
            ensure_capacity();
            m_data[m_size] = t;
            ++m_size;
        }

        void push_back(val_type &&t) {
            ensure_capacity();
            m_data[m_size] = move(t);
            ++m_size;
        }

        void pop_back() {
            if (m_size > 0) {
                --m_size;
            }
        }

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

        array_list &operator=(const array_list &) = delete;

        array_list &operator=(array_list &&list) {
            memory_free(m_data);
            m_data = move(list.m_data);
            m_size = move(list.m_size);
            m_capacity = move(list.m_capacity);
            return *this;
        }

    };

    template<typename T>
    void ArrayList<T>::ensure_capacity() {
        if (m_size < m_capacity) {
            return;
        }
        size_type new_capacity = (size_type) (2 * m_capacity);
        val_type *new_data = static_cast<val_type *>(memory_alloc(new_capacity * sizeof(val_type)));
        for (size_type i = 0; i < m_size; i++) {
            new_data[i] = m_data[i];
        }
        memory_free(m_data);
        m_data = new_data;
        m_capacity = new_capacity;
    }

    template<typename T>
    void ArrayList<T>::reserve(size_type new_capacity) {
        if (new_capacity <= m_capacity) {
            return;
        }
        val_type *new_data = static_cast<val_type *>(memory_alloc(new_capacity * sizeof(val_type)));
        for (size_type i = 0; i < m_size; i++) {
            new_data[i] = m_data[i];
        }
        memory_free(m_data);
        m_data = new_data;
        m_capacity = new_capacity;
    }

    template<typename T>
    void ArrayList<T>::shrink() {
        val_type *new_data = static_cast<val_type *>(memory_alloc(m_size * sizeof(val_type)));
        for (size_type i = 0; i < m_size; i++) {
            new_data[i] = m_data[i];
        }
        memory_free(m_data);
        m_data = new_data;
        m_capacity = m_size;
    }

    template<typename T>
    inline void ArrayList<T>::shift_right(size_type i) {
        for (size_type j = static_cast<size_type>(m_size - 1); j >= i; j--) {
            m_data[j + 1] = m_data[j];
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
