/**
 * @file OpenMap.h
 * @brief Hash map implementation.
 *
 * Hash map is implemented using open addressing and linear probing,
 * based on the assumption that it will be used with good knowledge
 * of the needed map size. The Hash Map provides the same basic
 * functionality as std::unordered_map or std::hash_map.
 *
 * @author Jeff Niu
 * @date November 1, 2017
 * @bug No known bugs
 */

#ifndef CORE_STL_MAP_H
#define CORE_STL_MAP_H

#include "Hash.h"
#include "Equal.h"
#include "Pair.h"
#include "../memory/Allocator.h"
#include "../memory/Memory.h"

namespace wlp {

    // Forward declaration of OpenHashMap
    template<class Key,
            class Val,
            class Hash,
            class Equal>
    class OpenHashMap;

    // Forward declaration of OpenHashMap iterator
    template<class Key,
            class Val,
            class Hash,
            class Equal>
    struct OpenHashMapIterator;

    // Forward declaration of const OpenHashMap iterator
    template<class Key,
            class Val,
            class Hash,
            class Equal>
    struct OpenHashMapConstIterator;

    /**
     * Hash map node comprise the elements of a hash map's
     * backing array, containing an element key and corresponding value.
     * @tparam Key key type
     * @tparam Val value type
     */
    template<class Key, class Val>
    struct OpenHashMapNode {
        typedef OpenHashMapNode<Key, Val> map_node;
        typedef Key key_type;
        typedef Val val_type;
        /**
         * The key of the node element.
         */
        key_type m_key;
        /**
         * The value of the node element.
         */
        val_type m_val;

        /**
         * Nodes are equal if the keys and values are equal.
         * @param node node to compare
         * @return true if the keys and values are equal
         */
        bool operator==(const map_node &node) const {
            return m_key == node.m_key && m_val == node.m_val;
        }

        /**
         * Equality operator for non const.
         * @param node node to compare
         * @return true if they are equal
         */
        bool operator==(map_node &node) const {
            return m_key == node.m_key && m_val == node.m_val;
        }
    };

    /**
     * Iterator class over the elements of a OpenHashMap. Specifically,
     * this class iterates from start to end of the OpenHashMap's backing
     * array, returning past-the-end afterwards.
     * @tparam Key   key type
     * @tparam Val   value type
     * @tparam Hash  hash function
     * @tparam Equal key equality function
     */
    template<class Key,
            class Val,
            class Hash,
            class Equal>
    struct OpenHashMapIterator {
        typedef OpenHashMap<Key, Val, Hash, Equal> hash_map;
        typedef OpenHashMapIterator<Key, Val, Hash, Equal> iterator;
        typedef OpenHashMapNode<Key, Val> map_node;

        typedef Val val_type;

        typedef uint16_t size_type;

        /**
         * Pointer to the node referenced by this iterator.
         */
        map_node *m_current;
        /**
         * Pointer to the iterated OpenHashMap.
         */
        hash_map *m_hash_map;

        /**
         * Default constructor.
         */
        OpenHashMapIterator() {}

        /**
         * Create an iterator to a OpenHashMap node.
         * @param node hash map node
         * @param map  parent hash map
         */
        OpenHashMapIterator(map_node *node, hash_map *map)
                : m_current(node), m_hash_map(map) {}

        /**
         * Copy constructor for const.
         * @param it iterator to copy
         */
        OpenHashMapIterator(const iterator &it)
                : m_current(it.m_current), m_hash_map(it.m_hash_map) {}

        /**
         * Copy constructor.
         * @param it iterator to copy
         */
        OpenHashMapIterator(iterator &it)
                : m_current(it.m_current), m_hash_map(it.m_hash_map) {}

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
         * the OpenHashMap. If no such element exists, returns pass-the-end
         * iterator. This is pre-fix unary operator.
         * @return this iterator
         */
        iterator &operator++();

        /**
         * Post-fix unary operator.
         * @return this iterator
         */
        iterator operator++(int);

        /**
         * Compare two iterators, equal of they point to the
         * same node.
         * @param it iterator to compare
         * @return true if both point to the same node
         */
        bool operator==(const iterator &it) const {
            return m_current == it.m_current;
        }

        /**
         * Non const equality operator.
         * @param it iterator to compare
         * @return true if both point to the same node
         */
        bool operator==(iterator &it) const {
            return m_current == it.m_current;
        }

        /**
         * Compare two iterators, unequal if they point to
         * different nodes.
         * @param it iterator to compare
         * @return true if they point to different nodes
         */
        bool operator!=(const iterator &it) const {
            return m_current != it.m_current;
        }

        /**
         * Non const equality operator.
         * @param it iterator to compare
         * @return true if they point to different nodes
         */
        bool operator!=(iterator &it) const {
            return m_current != it.m_current;
        }

        /**
         * Assignment operator copies pointers to node
         * and hash map.
         * @param it iterator to copy
         * @return a reference to this iterator
         */
        iterator &operator=(const iterator &it) {
            m_current = it.m_current;
            m_hash_map = it.m_hash_map;
            return *this;
        }

        /**
         * Assignment operator for non const.
         * @param it iterator to copy
         * @return a reference to this iterator
         */
        iterator &operator=(iterator &it) {
            m_current = it.m_current;
            m_hash_map = it.m_hash_map;
            return *this;
        }
    };

    /**
     * Constant iterator over a OpenHashMap. Values iterated by
     * this class cannot be modified.
     *
     * @see OpenHashMapIterator
     *
     * @tparam Key   key type
     * @tparam Val   value type
     * @tparam Hash  hash function
     * @tparam Equal key equality function
     */
    template<class Key,
            class Val,
            class Hash,
            class Equal>
    struct OpenHashMapConstIterator {
        typedef OpenHashMap<Key, Val, Hash, Equal> hash_map;
        typedef OpenHashMapConstIterator<Key, Val, Hash, Equal> const_iterator;
        typedef OpenHashMapNode<Key, Val> map_node;

        typedef Val val_type;

        typedef uint16_t size_type;

        const map_node *m_current;
        const hash_map *m_hash_map;

        OpenHashMapConstIterator() {}

        OpenHashMapConstIterator(map_node *node, const hash_map *map)
                : m_current(node), m_hash_map(map) {}

        OpenHashMapConstIterator(const const_iterator &it)
                : m_current(it.m_current), m_hash_map(it.m_hash_map) {}

        OpenHashMapConstIterator(const_iterator &it)
                : m_current(it.m_current), m_hash_map(it.m_hash_map) {}

        const val_type &operator*() const {
            return m_current->m_val;
        }

        const val_type *operator->() const {
            return &(operator*());
        }

        const_iterator &operator++();

        const_iterator operator++(int);

        bool operator==(const const_iterator &it) const {
            return m_current == it.m_current;
        }

        bool operator==(const_iterator &it) const {
            return m_current == it.m_current;
        }

        bool operator!=(const const_iterator &it) const {
            return m_current != it.m_current;
        }

        bool operator!=(const_iterator &it) const {
            return m_current != it.m_current;
        }

        const_iterator &operator=(const const_iterator &it) {
            m_current = it.m_current;
            m_hash_map = it.m_hash_map;
            return *this;
        }

        const_iterator &operator=(const_iterator &it) {
            m_current = it.m_current;
            m_hash_map = it.m_hash_map;
            return *this;
        }
    };

    /**
     * Hash map implemented using open addressing and linear probing,
     * in the spirit of std::unordered_map.
     * @tparam Key   key type
     * @tparam Val   value type
     * @tparam Hash  hash function
     * @tparam Equal key equality function
     */
    template<class Key,
            class Val,
            class Hash = hash <Key, uint16_t>,
            class Equal = equals <Key>>
    class OpenHashMap {
    public:
        typedef OpenHashMap<Key, Val, Hash, Equal> hash_map;
        typedef OpenHashMapIterator<Key, Val, Hash, Equal> iterator;
        typedef OpenHashMapConstIterator<Key, Val, Hash, Equal> const_iterator;
        typedef OpenHashMapNode<Key, Val> map_node;

        typedef Key key_type;
        typedef Val val_type;

        typedef uint16_t size_type;
        typedef uint8_t percent_type;

        friend struct OpenHashMapIterator<Key, Val, Hash, Equal>;
        friend struct OpenHashMapConstIterator<Key, Val, Hash, Equal>;

    private:
        /**
         * Class hash function instance. Used to hash
         * element keys.
         */
        Hash m_hash;
        /**
         * Class key equality function. Used to test
         * equality of element keys.
         */
        Equal m_equal;

        /**
         * Allocator to create memory for hash map nodes.
         */
        Allocator m_node_allocator;

        /**
         * Hash map backing array.
         */
        map_node **m_buckets;

        /**
         * The current number of elements that have been inserted
         * into the map.
         */
        size_type m_num_elements;
        /**
         * The size of the backing array.
         */
        size_type m_capacity;
        /**
         * The load factor in integer percent
         * before rehashing occurs. This number
         * cannot be larger than 100.
         */
        percent_type m_max_load;

    public:
        /**
         * Create and initialize an empty hash map. The hash map uses
         * {@code Allocator} to handle memory. The hash map is implemented
         * with open addressing and linear probing.
         *
         * @pre the hash map requires definition of an initial bucket array size
         *      and a maximum load factor before rehashing
         *
         * @param n        initial size of the bucket list; each bucket is initialized to nullptr
         * @param max_load an integer value denoting the max percent load factory, e.g. 75 = 0.75
         * @param hash     hash function for the key type, default is @code wlp::Hash @endcode
         * @param equal    equality function for the key type, default is @code wlp::Equal @endcode
         */
        explicit OpenHashMap(
                size_type n = 12,
                percent_type max_load = 75)
                : m_hash(Hash()),
                  m_equal(Equal()),
                  m_node_allocator{sizeof(map_node), static_cast<size_type>(n * sizeof(map_node))},
                  m_num_elements(0),
                  m_capacity(n),
                  m_max_load(max_load) {
            init_buckets(n);
            if (max_load > 100) {
                max_load = 100;
            }
        }

        OpenHashMap(const hash_map &map);

        OpenHashMap(hash_map &map);

        /**
         * Destroy the hash map, freeing allocated nodes and
         * memory allocated for the array.
         */
        ~OpenHashMap();

    private:
        /**
         * Function called when creating the hash map. This function
         * will allocate memory for the backing array and initialize each
         * element to nullptr.
         * @param n the size of the backing array
         */
        void init_buckets(size_type n);

        /**
         * Obtain the bucket index in an array with the specified
         * number of maximum elements.
         * @param key          the key to hash
         * @param max_elements the maximum number of buckets
         * @return an index i such that 0 <= i < max_elements
         */
        size_type bucket_index(key_type key, size_type max_elements) const {
            return m_hash(key) % max_elements;
        }

        /**
         * Obtain the bucket index of a key in the backing array.
         * @param key the key to hash
         * @return an index i such that 0 <= i < m_max_elements
         */
        size_type hash(key_type key) const {
            return m_hash(key) % m_capacity;
        }

        /**
         * Resize and rehash the hash map if the current load factor
         * exceeds or equals the maximum load factor. This function
         * will double the size of the backing array, allocating a new
         * array, and fully deallocating the previous array.
         *
         * @pre this function will create a new array allocator, however
         *      the same node allocator will be used, which means that
         *      the node allocator will start drawing from heap memory
         *      if the number of elements exceeds the initial max elements
         */
        void ensure_capacity();

    public:
        /**
         * @return the current number of elements that have been
         * inserted into the map
         */
        size_type size() const {
            return m_num_elements;
        }

        /**
         * @return the current size of the backing array
         */
        size_type capacity() const {
            return m_capacity;
        }

        /**
         * @return the maximum load before before rehash
         */
        percent_type max_load() const {
            return m_max_load;
        }

        /**
         * @return true if the map is empty
         */
        bool empty() const {
            return m_num_elements == 0;
        }

        /**
         * @return the node allocator of the map
         */
        const Allocator *get_node_allocator() const {
            return &m_node_allocator;
        }

        /**
         * Obtain an iterator to the first element in the hash map.
         * Returns pass-the-end iterator if there are no elements
         * in the hash map.
         * @return iterator the first element
         */
        iterator begin() {
            if (m_num_elements == 0) {
                return end();
            }
            for (size_type i = 0; i < m_capacity; ++i) {
                if (m_buckets[i]) {
                    return iterator(m_buckets[i], this);
                }
            }
            return end();
        }

        /**
         * @return a pass-the-end iterator for this map
         */
        iterator end() {
            return iterator(nullptr, this);
        }

        /**
         * @see OpenHashMap<Key, Val, Hash, Equal>::begin()
         * @return a constant iterator to the first element
         */
        const_iterator begin() const {
            if (m_num_elements == 0) {
                return end();
            }
            for (size_type i = 0; i < m_capacity; ++i) {
                if (m_buckets[i]) {
                    return const_iterator(m_buckets[i], this);
                }
            }
            return end();
        }

        /**
         * @see OpenHashMap<Key, Val, Hash, Equal>::end()
         * @return a constant pass-the-end iterator
         */
        const_iterator end() const {
            return const_iterator(nullptr, this);
        }

        /**
         * Erase all elements in the map, deallocating them
         * and resetting the element count to zero.
         */
        void clear() noexcept;

        /**
         * Attempt to insert an element into the map.
         * Insertion is prevented if there already exists
         * an element with the provided key
         * @param key inserted element key
         * @param val inserted element value
         * @return a pair consisting of an iterator pointing to the
         * inserted element or the element that prevented insertion
         * and a bool indicating whether insertion occurred
         */
        Pair<iterator, bool> insert(key_type key, val_type val);

        /**
         * Attempt to insert an element into the map.
         * If an element with the same key already exists,
         * override the value mapped to by the provided key.
         * @param key inserted element key
         * @param val inserted element value
         * @return a pair consisting of an iterator pointing to the
         * inserted element or the assigned element, and a bool
         * indicating whether insertion occurred
         */
        Pair<iterator, bool> insert_or_assign(key_type key, val_type val);

        /**
         * Returns the value corresponding to a provided key.
         * @param key the key for which to find the value
         * @return the value mapped to by the key
         * @throws KeyException if the key does not map to a value
         */
        iterator at(const key_type &key);

        /**
         * @see OpenHashMap<Key, Val, Hash, Equal>::at()
         * @param key key for which to find the value
         * @return the mapped value
         * @throws KeyException if the key does not exist
         */
        const_iterator at(const key_type &key) const;

        /**
         * @param key key for which to check existence of a value
         * @return true if the key maps to a value
         */
        bool contains(const key_type &key) const;

        /**
         * Return an iterator to the map element corresponding
         * to the provided key, or pass-the-end if the key does
         * not map to any value in the map.
         * @param key the key to map
         * @return an iterator to the element mapped by the key
         */
        iterator find(const key_type &key);

        /**
         * @see OpenHashMap<Key, Val, Hash, Equal>::find()
         * @param key the key to map
         * @return a const iterator to the element mapped by the key
         */
        const_iterator find(const key_type &key) const;

        /**
         * Access an element in the hash map by the given key.
         * If the key does not map to any value in the map,
         * then a new value is created and inserted using the default
         * constructor.
         * @param key the key whose value to access
         * @return a reference to the mapped value
         */
        val_type &operator[](const key_type &key);

        /**
         * Assignment operator performs a deep copy of the
         * contents of the map. Therefore, pass by reference
         * or by pointer value is generally desired.
         * @param map map to copy
         * @return a reference to this map
         */
        hash_map &operator=(const hash_map &map);

        /**
         * Assignment operator for non const.
         * @param map map to copy
         * @return a reference to this map
         */
        hash_map &operator=(hash_map &map);
    };

    template<class Key, class Val, class Hash, class Equal>
    void OpenHashMap<Key, Val, Hash, Equal>::init_buckets(OpenHashMap<Key, Val, Hash, Equal>::size_type n) {
        m_buckets = static_cast<map_node **>(memory_alloc(n * sizeof(map_node *)));
        for (size_type i = 0; i < n; ++i) {
            m_buckets[i] = nullptr;
        }
    }

    template<class Key, class Val, class Hash, class Equal>
    void OpenHashMap<Key, Val, Hash, Equal>::ensure_capacity() {
        if (m_num_elements * 100 < m_max_load * m_capacity) {
            return;
        }
        size_type new_capacity = static_cast<size_type>(m_capacity * 2);
        map_node **new_buckets = static_cast<map_node **>(memory_alloc(new_capacity * sizeof(map_node *)));
        for (size_type i = 0; i < new_capacity; ++i) {
            new_buckets[i] = nullptr;
        }
        for (size_type i = 0; i < m_capacity; ++i) {
            if (!m_buckets[i]) {
                continue;
            }
            map_node *node = m_buckets[i];
            size_type k = bucket_index(node->m_key, new_capacity);
            while (new_buckets[k]) {
                if (++k >= new_capacity) {
                    k = 0;
                }
            }
            new_buckets[k] = node;
        }
        memory_free(m_buckets);
        m_buckets = new_buckets;
        m_capacity = new_capacity;
    }

    template<class Key, class Val, class Hash, class Equal>
    void OpenHashMap<Key, Val, Hash, Equal>::clear() noexcept {
        for (size_type i = 0; i < m_capacity; ++i) {
            if (m_buckets[i]) {
                m_node_allocator.Deallocate(m_buckets[i]);
                m_buckets[i] = nullptr;
            }
        }
        m_num_elements = 0;
    }

    template<class Key, class Val, class Hash, class Equal>
    Pair<typename OpenHashMap<Key, Val, Hash, Equal>::iterator, bool>
    OpenHashMap<Key, Val, Hash, Equal>::insert(key_type key, val_type val) {
        ensure_capacity();
        size_type i = hash(key);
        while (m_buckets[i] && !m_equal(key, m_buckets[i]->m_key)) {
            if (++i >= m_capacity) {
                i = 0;
            }
        }
        if (m_buckets[i]) {
            return Pair<iterator, bool>(iterator(m_buckets[i], this), false);
        } else {
            ++m_num_elements;
            map_node *node = static_cast<map_node *>(m_node_allocator.Allocate());
            node->m_key = key;
            node->m_val = val;
            m_buckets[i] = node;
            return Pair<iterator, bool>(iterator(node, this), true);
        }
    };

    template<class Key, class Val, class Hash, class Equal>
    Pair<typename OpenHashMap<Key, Val, Hash, Equal>::iterator, bool>
    OpenHashMap<Key, Val, Hash, Equal>::insert_or_assign(key_type key, val_type val) {
        ensure_capacity();
        size_type i = hash(key);
        while (m_buckets[i] && !m_equal(key, m_buckets[i]->m_key)) {
            if (++i >= m_capacity) {
                i = 0;
            }
        }
        if (m_buckets[i]) {
            m_buckets[i]->m_val = val;
            return Pair<iterator, bool>(iterator(m_buckets[i], this), false);
        } else {
            ++m_num_elements;
            map_node *node = static_cast<map_node *>(m_node_allocator.Allocate());
            node->m_key = key;
            node->m_val = val;
            m_buckets[i] = node;
            return Pair<iterator, bool>(iterator(node, this), true);
        }
    };

    template<class Key, class Val, class Hash, class Equal>
    typename OpenHashMap<Key, Val, Hash, Equal>::iterator
    OpenHashMap<Key, Val, Hash, Equal>::at(const key_type &key) {
        size_type i = hash(key);
        while (m_buckets[i] && !m_equal(key, m_buckets[i]->m_key)) {
            if (++i >= m_capacity) {
                i = 0;
            }
        }
        if (m_buckets[i]) {
            return iterator(m_buckets[i], this);
        } else {
            return end();
        }
    }

    template<class Key, class Val, class Hash, class Equal>
    typename OpenHashMap<Key, Val, Hash, Equal>::const_iterator
    OpenHashMap<Key, Val, Hash, Equal>::at(const key_type &key) const {
        size_type i = hash(key);
        while (m_buckets[i] && !m_equal(key, m_buckets[i]->m_key)) {
            if (++i >= m_capacity) {
                i = 0;
            }
        }
        if (m_buckets[i]) {
            return const_iterator(m_buckets[i], this);
        } else {
            return end();
        }
    }

    template<class Key, class Val, class Hash, class Equal>
    bool OpenHashMap<Key, Val, Hash, Equal>::contains(const key_type &key) const {
        size_type i = hash(key);
        while (m_buckets[i]) {
            if (m_equal(key, m_buckets[i]->m_key)) {
                return true;
            }
            if (++i > +m_capacity) {
                i = 0;
            }
        }
        return false;
    }

    template<class Key, class Val, class Hash, class Equal>
    typename OpenHashMap<Key, Val, Hash, Equal>::val_type &
    OpenHashMap<Key, Val, Hash, Equal>::operator[](const key_type &key) {
        ensure_capacity();
        size_type i = hash(key);
        while (m_buckets[i] && !m_equal(key, m_buckets[i]->m_key)) {
            if (++i >= m_capacity) {
                i = 0;
            }
        }
        if (m_buckets[i]) {
            return m_buckets[i]->m_val;
        } else {
            ++m_num_elements;
            map_node *node = static_cast<map_node *>(m_node_allocator.Allocate());
            node->m_key = key;
            node->m_val = val_type();
            m_buckets[i] = node;
            return node->m_val;
        }
    }

    template<class Key, class Val, class Hash, class Equal>
    typename OpenHashMap<Key, Val, Hash, Equal>::iterator
    OpenHashMap<Key, Val, Hash, Equal>::find(const key_type &key) {
        size_type i = hash(key);
        while (m_buckets[i] && !m_equal(key, m_buckets[i]->m_key)) {
            if (++i >= m_capacity) {
                i = 0;
            }
        }
        if (m_buckets[i]) {
            return iterator(m_buckets[i], this);
        } else {
            return end();
        }
    }

    template<class Key, class Val, class Hash, class Equal>
    typename OpenHashMap<Key, Val, Hash, Equal>::const_iterator
    OpenHashMap<Key, Val, Hash, Equal>::find(const key_type &key) const {
        size_type i = hash(key);
        while (m_buckets[i] && !m_equal(key, m_buckets[i]->m_key)) {
            if (++i >= m_capacity) {
                i = 0;
            }
        }
        if (m_buckets[i]) {
            return const_iterator(m_buckets[i], this);
        } else {
            return end();
        }
    }

    template<class Key, class Val, class Hash, class Equal>
    OpenHashMap<Key, Val, Hash, Equal>::~OpenHashMap() {
        for (size_type i = 0; i < m_capacity; ++i) {
            if (m_buckets[i]) {
                m_node_allocator.Deallocate(m_buckets[i]);
                m_buckets[i] = nullptr;
            }
        }
        memory_free(m_buckets);
        m_buckets = nullptr;
    }

    template<class Key, class Val, class Hash, class Equal>
    OpenHashMap<Key, Val, Hash, Equal> &
    OpenHashMap<Key, Val, Hash, Equal>::operator=(const OpenHashMap<Key, Val, Hash, Equal> &map) {
        clear();
        memory_free(m_buckets);
        m_hash = map.m_hash;
        m_equal = map.m_equal;
        m_capacity = map.m_capacity;
        m_max_load = map.m_max_load;
        m_num_elements = map.m_num_elements;
        m_buckets = static_cast<map_node **>(memory_alloc(map.m_capacity * sizeof(map_node *)));
        for (size_type i = 0; i < map.m_capacity; ++i) {
            if (map.m_buckets[i]) {
                map_node *cur = static_cast<map_node *>(m_node_allocator.Allocate());
                map_node *m_cur = map.m_buckets[i];
                cur->m_key = m_cur->m_key;
                cur->m_val = m_cur->m_val;
                m_buckets[i] = cur;
            } else {
                m_buckets[i] = nullptr;
            }
        }
        return *this;
    }

    template<class Key, class Val, class Hash, class Equal>
    OpenHashMap<Key, Val, Hash, Equal> &
    OpenHashMap<Key, Val, Hash, Equal>::operator=(OpenHashMap<Key, Val, Hash, Equal> &map) {
        clear();
        memory_free(m_buckets);
        m_hash = map.m_hash;
        m_equal = map.m_equal;
        m_capacity = map.m_capacity;
        m_max_load = map.m_max_load;
        m_num_elements = map.m_num_elements;
        m_buckets = static_cast<map_node **>(memory_alloc(map.m_capacity * sizeof(map_node *)));
        for (size_type i = 0; i < map.m_capacity; ++i) {
            if (map.m_buckets[i]) {
                map_node *cur = static_cast<map_node *>(m_node_allocator.Allocate());
                map_node *m_cur = map.m_buckets[i];
                cur->m_key = m_cur->m_key;
                cur->m_val = m_cur->m_val;
                m_buckets[i] = cur;
            } else {
                m_buckets[i] = nullptr;
            }
        }
        return *this;
    }

    template<class Key, class Val, class Hash, class Equal>
    OpenHashMap<Key, Val, Hash, Equal>::OpenHashMap(const hash_map &map) :
            m_hash(map.m_hash),
            m_equal(map.m_equal),
            m_node_allocator{sizeof(map_node), static_cast<size_type>(map.m_capacity * sizeof(map_node))},
            m_num_elements(map.m_num_elements),
            m_capacity(map.m_capacity),
            m_max_load(map.m_max_load) {
        m_buckets = static_cast<map_node **>(memory_alloc(map.m_capacity * sizeof(map_node *)));
        for (size_type i = 0; i < map.m_capacity; i++) {
            m_buckets[i] = nullptr;
            if (map.m_buckets[i]) {
                map_node *m_cur = map.m_buckets[i];
                map_node *cur = static_cast<map_node *>(m_node_allocator.Allocate());
                cur->m_key = m_cur->m_key;
                cur->m_val = m_cur->m_val;
                m_buckets[i] = cur;
            }
        }
    }

    template<class Key, class Val, class Hash, class Equal>
    OpenHashMap<Key, Val, Hash, Equal>::OpenHashMap(hash_map &map) :
            m_hash(map.m_hash),
            m_equal(map.m_equal),
            m_node_allocator{sizeof(map_node), static_cast<size_type>(map.m_capacity * sizeof(map_node))},
            m_num_elements(map.m_num_elements),
            m_capacity(map.m_capacity),
            m_max_load(map.m_max_load) {
        m_buckets = static_cast<map_node **>(memory_alloc(map.m_capacity * sizeof(map_node *)));
        for (size_type i = 0; i < map.m_capacity; i++) {
            m_buckets[i] = nullptr;
            if (map.m_buckets[i]) {
                map_node *m_cur = map.m_buckets[i];
                map_node *cur = static_cast<map_node *>(m_node_allocator.Allocate());
                cur->m_key = m_cur->m_key;
                cur->m_val = m_cur->m_val;
                m_buckets[i] = cur;
            }
        }
    }

    template<class Key, class Val, class Hash, class Equal>
    OpenHashMapIterator<Key, Val, Hash, Equal> &
    OpenHashMapIterator<Key, Val, Hash, Equal>::operator++() {
        size_type i = m_hash_map->hash(m_current->m_key);
        while (m_hash_map->m_buckets[i] && !m_hash_map->m_equal(m_current->m_key, m_hash_map->m_buckets[i]->m_key)) {
            if (++i >= m_hash_map->m_capacity) {
                i = 0;
            }
        }
        while (++i < m_hash_map->m_capacity && !m_hash_map->m_buckets[i]);
        if (i == m_hash_map->m_capacity) {
            m_current = nullptr;
        } else {
            m_current = m_hash_map->m_buckets[i];
        }
        return *this;
    }

    template<class Key, class Val, class Hash, class Equal>
    inline OpenHashMapIterator<Key, Val, Hash, Equal>
    OpenHashMapIterator<Key, Val, Hash, Equal>::operator++(int) {
        iterator tmp = *this;
        ++*this;
        return tmp;
    }

    template<class Key, class Val, class Hash, class Equal>
    OpenHashMapConstIterator<Key, Val, Hash, Equal> &
    OpenHashMapConstIterator<Key, Val, Hash, Equal>::operator++() {
        size_type i = m_hash_map->hash(m_current->m_key);
        while (m_hash_map->m_buckets[i] && !m_hash_map->m_equal(m_current->m_key, m_hash_map->m_buckets[i]->m_key)) {
            if (++i >= m_hash_map->m_capacity) {
                i = 0;
            }
        }
        while (++i < m_hash_map->m_capacity && !m_hash_map->m_buckets[i]);
        if (i == m_hash_map->m_capacity) {
            m_current = nullptr;
        } else {
            m_current = m_hash_map->m_buckets[i];
        }
        return *this;
    }

    template<class Key, class Val, class Hash, class Equal>
    inline OpenHashMapConstIterator<Key, Val, Hash, Equal>
    OpenHashMapConstIterator<Key, Val, Hash, Equal>::operator++(int) {
        const_iterator tmp = *this;
        ++*this;
        return tmp;
    }

}

#endif //CORE_STL_MAP_H
