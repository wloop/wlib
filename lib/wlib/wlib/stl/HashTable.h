#ifndef EMBEDDEDCPLUSPLUS_HASHTABLE_H
#define EMBEDDEDCPLUSPLUS_HASHTABLE_H

#include <string.h> // memset

#include "Equal.h"
#include "Hash.h"
#include "Pair.h"

#include "wlib/Types.h"
#include "wlib/exceptions/Exceptions.h"
#include "wlib/utility/Utility.h"

namespace wlp {

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    class HashTable;

    template<typename Element>
    struct HashTableNode {
        typedef HashTableNode<Element> node_type;
        typedef Element element_type;

        /**
         * Pointer to the next node in the bucket.
         */
        node_type *m_next = nullptr;
        /**
         * Element contained by this node.
         */
        element_type m_element;
    };

    template<typename Element, typename Key, typename Val,
            typename Ref, typename Ptr,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    struct HashTableIterator {
        typedef HashTableIterator<Element, Key, Val, Ref, Ptr, GetKey, GetVal, Hasher, Equals> self_type;
        typedef HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals> table_type;
        typedef HashTableNode<Element> node_type;

        typedef Element element_type;
        typedef Val val_type;
        typedef Ref reference;
        typedef Ptr pointer;
        typedef GetKey get_key;
        typedef GetVal get_value;

        typedef wlp::size_type size_type;

        /**
         * Pointer to the node referenced by this iterator.
         */
        node_type *m_node;
        /**
         * Functor instance used to get keys.
         */
        get_key m_get_key;
        /**
         * Functor instance used to get values.
         */
        get_value m_get_value{};
        /**
         * Pointer to the iterated HashMap.
         */
        const table_type *m_table;

        /**
         * Default constructor.
         */
        HashTableIterator()
                : m_node(nullptr),
                  m_table(nullptr) {
        }

        /**
         * Create an iterator to a HashMap node.
         *
         * @param node  node to point to
         * @param table parent hash table
         */
        HashTableIterator(node_type *node, const table_type *table)
                : m_node(node),
                  m_table(table) {
        }

        reference operator*() const {
            if (m_node == nullptr) {
                THROW(KEY_EXCEPTION("Accessing invalid iterator"));
            }
            return m_get_value(m_node->m_element);
        }

        pointer operator->() const {
            if (m_node == nullptr) {
                THROW(KEY_EXCEPTION("Accessing invalid operator"));
            }
            return &(operator*());
        }

        /**
         * Increment iterator to the next element in the table.
         * If no element exists, returns pass-the-end iterator.
         *
         * @return reference to this iterator
         */
        self_type &operator++();

        /**
         * Postfix increment operator returns iterator
         * to next element in the table, or pass-the-end.
         *
         * @return next iterator
         */
        self_type operator++(int);

        /**
         * Equality operator. Compares iterators by their
         * pointed to nodes.
         *
         * @param it iterator to compare
         * @return true if iterators are equal
         */
        bool operator==(const self_type &it) const {
            return m_node == it.m_node;
        }

        /**
         * Inequality operator. Compares iterators by their
         * pointed to nodes.
         *
         * @param it iterator to compare
         * @return true if iteratores are not equal
         */
        bool operator!=(const self_type &it) const {
            return m_node != it.m_node;
        }

        /**
         * Assignment operator copies node and table.
         *
         * @param it iterator to copy
         * @return reference to this node.
         */
        self_type &operator=(const self_type &it) {
            m_node = it.m_node;
            m_table = it.m_table;
            return *this;
        }

    };

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher = Hash <Key, uint16_t>,
            typename Equals = Equal <Key>>
    class HashTable {
    public:
        typedef HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals> table_type;
        typedef HashTableNode<Element> node_type;
        typedef HashTableIterator<
                Element, Key, Val,
                Val &, Val *,
                GetKey, GetVal,
                Hasher, Equals
        > iterator;
        typedef HashTableIterator<
                Element, Key, Val,
                const Val &, const Val *,
                GetKey, GetVal,
                Hasher, Equals
        > const_iterator;

        typedef Element element_type;
        typedef Key key_type;
        typedef Val val_type;
        typedef GetKey get_key;
        typedef GetVal get_value;

        typedef wlp::size_type size_type;
        typedef uint8_t percent_type;

        typedef Hasher hash_function;
        typedef Equals key_equals;

        friend struct HashTableIterator<
                Element, Key, Val,
                Val &, Val *,
                GetKey, GetVal,
                Hasher, Equals
        >;
        friend struct HashTableIterator<
                Element, Key, Val,
                const Val &, const Val *,
                GetKey, GetVal,
                Hasher, Equals
        >;

    private:
        /**
         * Hash function functor instance.
         */
        hash_function m_hash_function{};
        /**
         * Key equals functor instance.
         */
        key_equals m_key_equals{};
        /**
         * Functor instance to obtain key;
         */
        get_key m_get_key{};

        /**
         * Hash map backing array.
         */
        node_type **m_buckets;
        /**
         * Number of elements currently in the map.
         */
        size_type m_size;
        /**
         * The maximum size of the hash table. This number
         * represents the current capacity of the backing array.
         */
        size_type m_capacity;
        /**
         * The max load factor of the hash table before rehashing.
         */
        percent_type m_max_load;

    public:
        explicit HashTable(size_type n = 12, percent_type max_load = 75)
                : m_size(0),
                  m_capacity(n),
                  m_max_load(max_load) {
            init_buckets(n);
        }

        HashTable(const table_type &) = delete;

        HashTable(table_type &&table)
                : m_buckets(table.m_buckets),
                  m_size(table.m_size),
                  m_capacity(table.m_capacity),
                  m_max_load(table.m_max_load) {
            table.m_buckets = nullptr;
            table.m_size = 0;
            table.m_capacity = 0;
        }

        ~HashTable() {
            if (!m_buckets) {
                return;
            }
            clear();
            free<node_type *>(m_buckets);
            m_buckets = nullptr;
        }

    private:
        void init_buckets(size_type n);

        size_type bucket_index(const key_type &key, size_type capacity) const {
            return m_hash_function(key) % capacity;
        }

        size_type hash(const key_type &key) const {
            return m_hash_function(key) % m_capacity;
        }

        void ensure_capacity();

    public:
        size_type size() const {
            return m_size;
        }

        size_type capacity() const {
            return m_capacity;
        }

        percent_type max_load() const {
            return m_max_load;
        }

        bool empty() const {
            return m_size == 0;
        }

        iterator begin() {
            for (size_type n = 0; n < m_capacity; ++n) {
                if (m_buckets[n]) {
                    return iterator(m_buckets[n], this);
                }
            }
            return end();
        }

        const_iterator begin() const {
            for (size_type n = 0; n < m_capacity; ++n) {
                if (m_buckets[n]) {
                    return const_iterator(m_buckets[n], this);
                }
            }
            return end();
        }

        iterator end() {
            return iterator(nullptr, this);
        }

        const_iterator end() const {
            return const_iterator(nullptr, this);
        }

        template<typename E>
        Pair<iterator, bool> insert_unique(E &&element);

        template<typename E>
        iterator insert_equal(E &&element);

        template<typename E>
        element_type &find_or_insert(E &&element);

        iterator find(const key_type &key) {
            size_type n = hash(key);
            node_type *first;
            for (first = m_buckets[n];
                 first && !m_key_equals(m_get_key(first->m_element), key);
                 first = first->m_next) {}
            return iterator(first, this);
        }

        const_iterator find(const key_type &key) const {
            size_type n = hash(key);
            node_type *first;
            for (first = m_buckets[n];
                 first && !m_key_equals(m_get_key(first->m_element), key);
                 first = first->m_next) {}
            return const_iterator(first, this);
        }

        size_type count(const key_type &key) const {
            size_type n = hash(key);
            size_type result = 0;
            for (const node_type *cur = m_buckets[n]; cur; cur = cur->m_next) {
                if (m_key_equals(m_get_key(cur->m_element), key)) {
                    ++result;
                }
            }
            return result;
        }

        Pair <iterator, iterator> equal_range(const key_type &key);

        Pair <const_iterator, const_iterator> equal_range(const key_type &key) const;

        void erase(const iterator &pos);

        size_type erase(const key_type &key);

        void clear() noexcept;

        table_type &operator=(const table_type &) = delete;

        table_type &operator=(table_type &&table) {
            if (m_buckets) {
                clear();
                free<node_type *>(m_buckets);
            }
            m_buckets = table.m_buckets;
            m_size = table.m_size;
            m_capacity = table.m_capacity;
            table.m_buckets = nullptr;
            table.m_size = 0;
            table.m_capacity = 0;
            return *this;
        }
    };

    template<typename Element, typename Key, typename Val,
            typename Ref, typename Ptr,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    typename HashTableIterator<Element, Key, Val, Ref, Ptr, GetKey, GetVal, Hasher, Equals>::self_type &
    HashTableIterator<Element, Key, Val, Ref, Ptr, GetKey, GetVal, Hasher, Equals>
    ::operator++() {
        if (!m_node) {
            return *this;
        }
        const node_type *old = m_node;
        m_node = m_node->m_next;
        if (!m_node) {
            size_type n = m_table->hash(m_get_key(old->m_element));
            while (!m_node && ++n < m_table->m_capacity) {
                m_node = m_table->m_buckets[n];
            }
        }
        return *this;
    }

    template<typename Element, typename Key, typename Val,
            typename Ref, typename Ptr,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    typename HashTableIterator<Element, Key, Val, Ref, Ptr, GetKey, GetVal, Hasher, Equals>::self_type
    HashTableIterator<Element, Key, Val, Ref, Ptr, GetKey, GetVal, Hasher, Equals>
    ::operator++(int) {
        self_type tmp = *this;
        ++*this;
        return tmp;
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    template<typename E>
    Pair<typename HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>::iterator, bool>
    HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::insert_unique(E &&element) {
        ensure_capacity();
        const size_type n = hash(m_get_key(element));
        node_type *first = m_buckets[n];
        for (node_type *cur = first; cur; cur = cur->m_next) {
            if (m_key_equals(m_get_key(cur->m_element), m_get_key(element))) {
                return Pair<iterator, bool>(iterator(cur, this), false);
            }
        }
        node_type *tmp = malloc<node_type>();
        tmp->m_element = forward<E>(element);
        tmp->m_next = first;
        m_buckets[n] = tmp;
        ++m_size;
        return Pair<iterator, bool>(iterator(tmp, this), true);
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    template<typename E>
    typename HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>::iterator
    HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::insert_equal(E &&element) {
        ensure_capacity();
        const size_type n = hash(m_get_key(element));
        node_type *first = m_buckets[n];
        for (node_type *cur = first; cur; cur = cur->m_next) {
            if (m_key_equals(m_get_key(cur->m_element), m_get_key(element))) {
                node_type *tmp = malloc<node_type>();
                tmp->m_element = forward<E>(element);
                tmp->m_next = cur->m_next;
                cur->m_next = tmp;
                ++m_size;
                return iterator(tmp, this);
            }
        }
        node_type *tmp = malloc<node_type>();
        tmp->m_element = forward<E>(element);
        tmp->m_next = first;
        m_buckets[n] = tmp;
        ++m_size;
        return iterator(tmp, this);
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    template<typename E>
    typename HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>::element_type &
    HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::find_or_insert(E &&element) {
        ensure_capacity();
        size_type n = hash(m_get_key(element));
        node_type *first = m_buckets[n];
        for (node_type *cur = first; cur; cur = cur->m_next) {
            if (m_key_equals(m_get_key(cur->m_element), m_get_key(element))) {
                return cur->m_element;
            }
        }
        node_type *tmp = malloc<node_type>();
        tmp->m_element = forward<E>(element);
        tmp->m_next = first;
        m_buckets[n] = tmp;
        ++m_size;
        return tmp->m_element;
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    Pair<typename HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>::iterator,
            typename HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>::iterator>
    HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::equal_range(const key_type &key) {
        typedef Pair<iterator, iterator> ret_type;
        const size_type n = hash(key);
        for (node_type *first = m_buckets[n]; first; first = first->m_next) {
            if (m_key_equals(m_get_key(first->m_element), key)) {
                for (node_type *cur = first->m_next; cur; cur = cur->m_next) {
                    if (!m_key_equals(m_get_key(cur->m_element), key)) {
                        return ret_type(iterator(first, this), iterator(cur, this));
                    }
                }
                for (size_type m = static_cast<unsigned short>(n + 1); m < m_size; ++m) {
                    if (m_buckets[m]) {
                        return ret_type(iterator(first, this), iterator(m_buckets[m], this));
                    }
                }
                return ret_type(iterator(first, this), end());
            }
        }
        return ret_type(end(), end());
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    Pair<typename HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>::const_iterator,
            typename HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>::const_iterator>
    HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::equal_range(const key_type &key) const {
        typedef Pair<const_iterator, const_iterator> ret_type;
        const size_type n = hash(key);
        for (node_type *first = m_buckets[n]; first; first = first->m_next) {
            if (m_key_equals(m_get_key(first->m_element), key)) {
                for (node_type *cur = first->m_next; cur; cur = cur->m_next) {
                    if (!m_key_equals(m_get_key(cur->m_element), key)) {
                        return ret_type(const_iterator(first, this), const_iterator(cur, this));
                    }
                }
                for (size_type m = static_cast<unsigned short>(n + 1); m < m_size; ++m) {
                    if (m_buckets[m]) {
                        return ret_type(const_iterator(first, this), const_iterator(m_buckets[m], this));
                    }
                }
                return ret_type(const_iterator(first, this), end());
            }
        }
        return ret_type(end(), end());
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    void HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::erase(const iterator &it) {
        node_type *node = it.m_node;
        if (node) {
            const size_type n = hash(m_get_key(node->m_element));
            node_type *cur = m_buckets[n];
            if (cur == node) {
                m_buckets[n] = cur->m_next;
                free<node_type>(cur);
                --m_size;
                return;
            }
            node_type *next = cur->m_next;
            while (next) {
                if (next == node) {
                    cur->m_next = next->m_next;
                    free<node_type>(next);
                    --m_size;
                    break;
                }
                cur = next;
                next = cur->m_next;
            }
        }
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    typename HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>::size_type
    HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::erase(const key_type &key) {
        const size_type n = hash(key);
        node_type *first = m_buckets[n];
        size_type erased = 0;
        if (first) {
            node_type *cur = first;
            node_type *next = cur->m_next;
            while (next) {
                if (m_key_equals(m_get_key(next->m_element), key)) {
                    cur->m_next = next->m_next;
                    free<node_type>(next);
                    next = cur->m_next;
                    ++erased;
                    --m_size;
                } else {
                    cur = next;
                    next = cur->m_next;
                }
            }
            if (m_key_equals(m_get_key(first->m_element), key)) {
                m_buckets[n] = first->m_next;
                free<node_type>(first);
                ++erased;
                --m_size;
            }
        }
        return erased;
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    void HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::clear() noexcept {
        for (size_type i = 0; i < m_capacity; ++i) {
            node_type *cur = m_buckets[i];
            node_type *next;
            while (cur) {
                next = cur->m_next;
                free<node_type>(cur);
                cur = next;
            }
            m_buckets[i] = nullptr;
        }
        m_size = 0;
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    void HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::init_buckets(size_type n) {
        m_buckets = malloc<node_type *[]>(n);
        memset(m_buckets, 0, n * sizeof(node_type *));
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    void HashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::ensure_capacity() {
        if (m_size * 100 < m_max_load * m_capacity) {
            return;
        }
        size_type new_capacity = static_cast<size_type>(m_capacity * 2);
        node_type **new_buckets = malloc<node_type *[]>(new_capacity);
        memset(new_buckets, 0, new_capacity * sizeof(node_type *));
        for (size_type i = 0; i < m_capacity; ++i) {
            if (!m_buckets[i]) {
                continue;
            }
            node_type *cur = m_buckets[i];
            while (cur) {
                size_type k = bucket_index(m_get_key(cur->m_element), new_capacity);
                node_type *first = new_buckets[k];
                node_type *next = cur->m_next;
                cur->m_next = first;
                new_buckets[k] = cur;
                cur = next;
            }
        }
        free<node_type *>(m_buckets);
        m_buckets = new_buckets;
        m_capacity = new_capacity;
    }

}

#endif //EMBEDDEDCPLUSPLUS_HASHTABLE_H
