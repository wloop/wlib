/**
 * @file OpenMap.h
 * @brief Hash map implementation.
 *
 * Hash map is implemented using open addressing and linear probing,
 * based on the assumption that it will be used with good knowledge
 * of the needed map size. The Hash Map provides the same basic
 * functionality as std::unordered_map or std::map_type.
 *
 * @author Jeff Niu
 * @date November 1, 2017
 * @bug No known bugs
 */

#ifndef CORE_STL_MAP_H
#define CORE_STL_MAP_H

#include "Equal.h"
#include "Hash.h"
#include "Pair.h"

#include "../memory/Memory.h"
#include "../utility/Utility.h"

namespace wlp {

    // Forward declaration of OpenHashMap
    template<typename Key,
            typename Val,
            typename Hasher,
            typename Equals>
    class OpenHashMap;

    /**
     * Hasher map node comprise the elements of a hash map's
     * backing array, containing an element key and corresponding value.
     * @tparam Key key type
     * @tparam Val value type
     */
    template<typename Key, class Val>
    struct OpenHashMapNode {
        typedef OpenHashMapNode<Key, Val> node_type;
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
        bool operator==(const node_type &node) const {
            return m_key == node.m_key && m_val == node.m_val;
        }
    };

    /**
     * Iterator class over the elements of a OpenHashMap. Specifically,
     * this class iterates from start to end of the OpenHashMap's backing
     * array, returning past-the-end afterwards.
     * @tparam Key   key type
     * @tparam Val   value type
     * @tparam Hasher  hash function
     * @tparam Equals key equality function
     */
    template<typename Key,
            typename Val,
            typename Ref,
            typename Ptr,
            typename Hasher,
            typename Equals>
    struct OpenHashMapIterator {
        typedef OpenHashMap<Key, Val, Hasher, Equals> map_type;
        typedef OpenHashMapNode<Key, Val> node_type;
        typedef OpenHashMapIterator<Key, Val, Ref, Ptr, Hasher, Equals> self_type;

        typedef Val val_type;
        typedef Ref reference;
        typedef Ptr pointer;

        typedef wlp::size_type size_type;

        /**
         * Pointer to the node referenced by this iterator.
         */
        node_type *m_current;
        /**
         * Pointer to the iterated OpenHashMap.
         */
        const map_type *m_hash_map;

        /**
         * Default constructor.
         */
        OpenHashMapIterator()
                : m_current(nullptr),
                  m_hash_map(nullptr) {
        }

        /**
         * Create an iterator to a OpenHashMap node.
         * @param node hash map node
         * @param map  parent hash map
         */
        OpenHashMapIterator(node_type *node, const map_type *map)
                : m_current(node),
                  m_hash_map(map) {
        }

        /**
         * Copy constructor for const.
         * @param it iterator to copy
         */
        OpenHashMapIterator(const self_type &it)
                : m_current(it.m_current),
                  m_hash_map(it.m_hash_map) {
        }

        /**
         * @return reference to the value of the node
         * pointed to by the iterator
         */
        reference operator*() const {
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
         * the OpenHashMap. If no such element exists, returns pass-the-end
         * iterator. This is pre-fix unary operator.
         * @return this iterator
         */
        self_type &operator++();

        /**
         * Post-fix unary operator.
         * @return this iterator
         */
        self_type operator++(int);

        /**
         * Compare two iterators, equal of they point to the
         * same node.
         * @param it iterator to compare
         * @return true if both point to the same node
         */
        bool operator==(const self_type &it) const {
            return m_current == it.m_current;
        }

        /**
         * Compare two iterators, unequal if they point to
         * different nodes.
         * @param it iterator to compare
         * @return true if they point to different nodes
         */
        bool operator!=(const self_type &it) const {
            return m_current != it.m_current;
        }

        /**
         * Assignment operator copies pointers to node
         * and hash map.
         * @param it iterator to copy
         * @return a reference to this iterator
         */
        self_type &operator=(const self_type &it) {
            m_current = it.m_current;
            m_hash_map = it.m_hash_map;
            return *this;
        }

    };

    /**
     * Hasher map implemented using open addressing and linear probing,
     * in the spirit of std::unordered_map.
     * @tparam Key   key type
     * @tparam Val   value type
     * @tparam Hasher  hash function
     * @tparam Equals key equality function
     */
    template<typename Key,
            typename Val,
            typename Hasher = Hash <Key, uint16_t>,
            typename Equals = Equal <Key>>
    class OpenHashMap {
    public:
        typedef OpenHashMap<Key, Val, Hasher, Equals> map_type;
        typedef OpenHashMapNode<Key, Val> node_type;
        typedef OpenHashMapIterator<Key, Val, Val &, Val *, Hasher, Equals> iterator;
        typedef OpenHashMapIterator<Key, Val, const Val &, const Val *, Hasher, Equals> const_iterator;

        typedef Key key_type;
        typedef Val val_type;

        typedef wlp::size_type size_type;
        typedef uint8_t percent_type;

        friend struct OpenHashMapIterator<Key, Val, Val &, Val *, Hasher, Equals>;
        friend struct OpenHashMapIterator<Key, Val, const Val &, const Val *, Hasher, Equals>;

    private:
        /**
         * Class hash function instance. Used to hash
         * element keys.
         */
        Hasher m_hash;
        /**
         * Class key equality function. Used to test
         * equality of element keys.
         */
        Equals m_equal;

        /**
         * Hasher map backing array.
         */
        node_type **m_buckets;

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
         * The hash map is implemented with open addressing and linear probing.
         *
         * @pre the hash map requires definition of an initial bucket array size
         *      and a maximum load factor before rehashing
         *
         * @param n        initial size of the bucket list; each bucket is initialized to nullptr
         * @param max_load an integer value denoting the max percent load factory, e.g. 75 = 0.75
         * @param hash     hash function for the key type, default is @code wlp::Hasher @endcode
         * @param equal    equality function for the key type, default is @code wlp::Equals @endcode
         */
        explicit OpenHashMap(
                size_type n = 12,
                percent_type max_load = 75)
                : m_hash(Hasher()),
                  m_equal(Equals()),
                  m_num_elements(0),
                  m_capacity(n),
                  m_max_load(max_load) {
            init_buckets(n);
            if (max_load > 100) {
                m_max_load = 100;
            }
        }

        /**
         * Copy constructor is deleted to prevent
         * copying of complex structures.
         */
        OpenHashMap(const map_type &) = delete;

        /**
         * Move constructor transfers resources from
         * rvalue hash map into this hash map.
         * @param map map from which to transfer
         */
        OpenHashMap(map_type &&map) :
                m_hash(move(map.m_hash)),
                m_equal(move(map.m_equal)),
                m_buckets(move(map.m_buckets)),
                m_num_elements(move(map.m_num_elements)),
                m_capacity(move(map.m_capacity)),
                m_max_load(move(map.m_max_load)) {
            map.m_num_elements = 0;
            map.m_capacity = 0;
            map.m_buckets = nullptr;
        }

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
        size_type bucket_index(const key_type &key, size_type max_elements) const {
            return m_hash(key) % max_elements;
        }

        /**
         * Obtain the bucket index of a key in the backing array.
         * @param key the key to hash
         * @return an index i such that 0 <= i < m_max_elements
         */
        size_type hash(const key_type &key) const {
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
         * @see OpenHashMap<Key, Val, Hasher, Equals>::begin()
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
         * @see OpenHashMap<Key, Val, Hasher, Equals>::end()
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
         *
         * @param key inserted element key
         * @param val inserted element value
         * @return a pair consisting of an iterator pointing to the
         * inserted element or the element that prevented insertion
         * and a bool indicating whether insertion occurred
         */
        template<typename K, typename V>
        Pair<iterator, bool> insert(K &&key, V &&val);

        /**
         * Attempt to insert an element into the map.
         * If an element with the same key already exists,
         * override the value mapped to by the provided key.
         *
         * @param key inserted element key
         * @param val inserted element value
         * @return a pair consisting of an iterator pointing to the
         * inserted element or the assigned element, and a bool
         * indicating whether insertion occurred
         */
        template<typename K, typename V>
        Pair<iterator, bool> insert_or_assign(K &&key, V &&val);

        /**
         * Erase the element from the map pointed to by the provided
         * iterator. Be aware that erasure operations on an openly
         * addressed hash map will trigger a rehash and invalidate
         * all iterators other than the return value.
         *
         * @param pos iterator pointing to the element to erase
         * @return iterator to the next element in the map or end
         */
        iterator erase(const iterator &pos);

        /**
         * Erase the element from the map with the provided key, if such
         * an element exists. Be aware that if erasure if performed, the
         * hash map will undergo a rehash.
         *
         * @pre If frequency use of erase operations is needed, one is
         *      recommended to use @code ChainHashMap @encode or a
         *      tree-based implementation.
         *
         * @param key the key whose corresponding element to erase
         * @return true if an element was erased
         */
        bool erase(const key_type &key);

        /**
         * Returns the value corresponding to a provided key.
         *
         * @param key the key for which to find the value
         * @return the value mapped to by the key
         * @throws KeyException if the key does not map to a value
         */
        val_type &at(const key_type &key) {
            return *find(key);
        }

        /**
         * @see OpenHashMap<Key, Val, Hasher, Equals>::at()
         * @param key key for which to find the value
         * @return the mapped value
         * @throws KeyException if the key does not exist
         */
        const val_type &at(const key_type &key) const {
            return *find(key);
        }

        /**
         * @param key key for which to check existence of a value
         * @return true if the key maps to a value
         */
        bool contains(const key_type &key) const;

        /**
         * Return an iterator to the map element corresponding
         * to the provided key, or pass-the-end if the key does
         * not map to any value in the map.
         *
         * @param key the key to map
         * @return an iterator to the element mapped by the key
         */
        iterator find(const key_type &key);

        /**
         * @see OpenHashMap<Key, Val, Hasher, Equals>::find()
         * @param key the key to map
         * @return a const iterator to the element mapped by the key
         */
        const_iterator find(const key_type &key) const;

        /**
         * Access an element in the hash map by the given key.
         * If the key does not map to any value in the map,
         * then a new value is created and inserted using the default
         * constructor.
         *
         * @param key the key whose value to access
         * @return a reference to the mapped value
         */
        template<typename K>
        val_type &operator[](K &&key);

        /**
         * Copy assignment operators are disabled.
         *
         * @return hash map reference
         */
        map_type &operator=(const map_type &) = delete;

        /**
         * Move assignment operator. Assigned hash map
         * will have its resources transferred into this
         * hash map. Existing resources will be released.
         *
         * @param map map to move
         * @return reference to this map
         */
        map_type &operator=(map_type &&map);
    };

    template<typename Key, typename Val, typename Hasher, typename Equals>
    void OpenHashMap<Key, Val, Hasher, Equals>::init_buckets(OpenHashMap<Key, Val, Hasher, Equals>::size_type n) {
        m_buckets = malloc<node_type *[]>(n);
        for (size_type i = 0; i < n; ++i) {
            m_buckets[i] = nullptr;
        }
    }

    template<typename Key, typename Val, typename Hasher, typename Equals>
    void OpenHashMap<Key, Val, Hasher, Equals>::ensure_capacity() {
        if (m_num_elements * 100 < m_max_load * m_capacity) {
            return;
        }
        size_type new_capacity = static_cast<size_type>(m_capacity * 2);
        node_type **new_buckets = malloc<node_type *[]>(new_capacity);
        for (size_type i = 0; i < new_capacity; ++i) {
            new_buckets[i] = nullptr;
        }
        for (size_type i = 0; i < m_capacity; ++i) {
            if (!m_buckets[i]) {
                continue;
            }
            node_type *node = m_buckets[i];
            size_type k = bucket_index(node->m_key, new_capacity);
            while (new_buckets[k]) {
                if (++k >= new_capacity) {
                    k = 0;
                }
            }
            new_buckets[k] = node;
        }
        free<node_type *>(m_buckets);
        m_buckets = new_buckets;
        m_capacity = new_capacity;
    }

    template<typename Key, typename Val, typename Hasher, typename Equals>
    void OpenHashMap<Key, Val, Hasher, Equals>::clear() noexcept {
        for (size_type i = 0; i < m_capacity; ++i) {
            if (m_buckets[i]) {
                free<node_type>(m_buckets[i]);
                m_buckets[i] = nullptr;
            }
        }
        m_num_elements = 0;
    }

    template<typename Key, typename Val, typename Hasher, typename Equals>
    template<typename K, typename V>
    Pair<typename OpenHashMap<Key, Val, Hasher, Equals>::iterator, bool>
    OpenHashMap<Key, Val, Hasher, Equals>::insert(K &&key, V &&val) {
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
            node_type *node = malloc<node_type>();
            node->m_key = forward<K>(key);
            node->m_val = forward<V>(val);
            m_buckets[i] = node;
            return Pair<iterator, bool>(iterator(node, this), true);
        }
    };

    template<typename Key, typename Val, typename Hasher, typename Equals>
    template<typename K, typename V>
    Pair<typename OpenHashMap<Key, Val, Hasher, Equals>::iterator, bool>
    OpenHashMap<Key, Val, Hasher, Equals>::insert_or_assign(K &&key, V &&val) {
        ensure_capacity();
        size_type i = hash(key);
        while (m_buckets[i] && !m_equal(key, m_buckets[i]->m_key)) {
            if (++i >= m_capacity) {
                i = 0;
            }
        }
        if (m_buckets[i]) {
            m_buckets[i]->m_val = forward<V>(val);
            return Pair<iterator, bool>(iterator(m_buckets[i], this), false);
        } else {
            ++m_num_elements;
            node_type *node = malloc<node_type>();
            node->m_key = forward<K>(key);
            node->m_val = forward<V>(val);
            m_buckets[i] = node;
            return Pair<iterator, bool>(iterator(node, this), true);
        }
    };

    template<typename Key, typename Val, typename Hasher, typename Equals>
    typename OpenHashMap<Key, Val, Hasher, Equals>::iterator
    OpenHashMap<Key, Val, Hasher, Equals>::erase(const iterator &pos) {
        const node_type *cur_node = pos.m_current;
        if (!cur_node || pos.m_hash_map != this) {
            return end();
        }
        size_type i = hash(cur_node->m_key);
        while (m_buckets[i] && !m_equal(cur_node->m_key, m_buckets[i]->m_key)) {
            if (++i >= m_capacity) {
                i = 0;
            }
        }
        if (!m_buckets[i]) {
            return end();
        }
        --m_num_elements;
        free<node_type>(m_buckets[i]);
        m_buckets[i] = nullptr;
        while (++i < m_capacity && !m_buckets[i]) {}
        node_type *next_node = i >= m_capacity ? nullptr : m_buckets[i];
        node_type **new_buckets = malloc<node_type *[]>(m_capacity);
        for (size_type k = 0; k < m_capacity; k++) {
            new_buckets[k] = nullptr;
        }
        for (size_type k = 0; k < m_capacity; k++) {
            if (!m_buckets[k]) {
                continue;
            }
            node_type *node = m_buckets[k];
            size_type j = hash(node->m_key);
            while (new_buckets[j]) {
                if (++j >= m_capacity) {
                    j = 0;
                }
            }
            new_buckets[j] = node;
        }
        free<node_type *>(m_buckets);
        m_buckets = new_buckets;
        return iterator(next_node, this);
    }

    template<typename Key, typename Val, typename Hasher, typename Equals>
    bool OpenHashMap<Key, Val, Hasher, Equals>::erase(const key_type &key) {
        size_type i = hash(key);
        while (m_buckets[i] && !m_equal(key, m_buckets[i]->m_key)) {
            if (++i >= m_capacity) {
                i = 0;
            }
        }
        if (!m_buckets[i]) {
            return false;
        }
        --m_num_elements;
        free<node_type>(m_buckets[i]);
        m_buckets[i] = nullptr;
        node_type **new_buckets = malloc<node_type *[]>(m_capacity);
        for (size_type k = 0; k < m_capacity; k++) {
            new_buckets[k] = nullptr;
        }
        for (size_type k = 0; k < m_capacity; k++) {
            if (!m_buckets[k]) {
                continue;
            }
            node_type *node = m_buckets[k];
            size_type j = hash(node->m_key);
            while (new_buckets[j]) {
                if (++j >= m_capacity) {
                    j = 0;
                }
            }
            new_buckets[j] = node;
        }
        free<node_type *>(m_buckets);
        m_buckets = new_buckets;
        return true;
    }

    template<typename Key, typename Val, typename Hasher, typename Equals>
    bool OpenHashMap<Key, Val, Hasher, Equals>::contains(const key_type &key) const {
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

    template<typename Key, typename Val, typename Hasher, typename Equals>
    template<typename K>
    typename OpenHashMap<Key, Val, Hasher, Equals>::val_type &
    OpenHashMap<Key, Val, Hasher, Equals>::operator[](K &&key) {
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
            node_type *node = malloc<node_type>();
            node->m_key = forward<K>(key);
            node->m_val = val_type();
            m_buckets[i] = node;
            return node->m_val;
        }
    }

    template<typename Key, typename Val, typename Hasher, typename Equals>
    inline typename OpenHashMap<Key, Val, Hasher, Equals>::iterator
    OpenHashMap<Key, Val, Hasher, Equals>::find(const key_type &key) {
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

    template<typename Key, typename Val, typename Hasher, typename Equals>
    inline typename OpenHashMap<Key, Val, Hasher, Equals>::const_iterator
    OpenHashMap<Key, Val, Hasher, Equals>::find(const key_type &key) const {
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

    template<typename Key, typename Val, typename Hasher, typename Equals>
    OpenHashMap<Key, Val, Hasher, Equals>::~OpenHashMap() {
        if (!m_buckets) {
            return;
        }
        for (size_type i = 0; i < m_capacity; ++i) {
            if (m_buckets[i]) {
                free<node_type>(m_buckets[i]);
                m_buckets[i] = nullptr;
            }
        }
        free<node_type *>(m_buckets);
        m_buckets = nullptr;
    }

    template<typename Key, typename Val, typename Hasher, typename Equals>
    OpenHashMap<Key, Val, Hasher, Equals> &
    OpenHashMap<Key, Val, Hasher, Equals>::operator=(OpenHashMap<Key, Val, Hasher, Equals> &&map) {
        clear();
        free<node_type *>(m_buckets);
        m_capacity = move(map.m_capacity);
        m_max_load = move(map.m_max_load);
        m_num_elements = move(map.m_num_elements);
        m_buckets = move(map.m_buckets);
        map.m_capacity = 0;
        map.m_num_elements = 0;
        map.m_buckets = nullptr;
        return *this;
    }

    template<typename Key, typename Val, typename Ref, typename Ptr, typename Hasher, typename Equals>
    OpenHashMapIterator<Key, Val, Ref, Ptr, Hasher, Equals> &
    OpenHashMapIterator<Key, Val, Ref, Ptr, Hasher, Equals>::operator++() {
        size_type i = m_hash_map->hash(m_current->m_key);
        while (m_hash_map->m_buckets[i] && !m_hash_map->m_equal(m_current->m_key, m_hash_map->m_buckets[i]->m_key)) {
            if (++i >= m_hash_map->m_capacity) {
                i = 0;
            }
        }
        while (++i < m_hash_map->m_capacity && !m_hash_map->m_buckets[i]) {}
        if (i == m_hash_map->m_capacity) {
            m_current = nullptr;
        } else {
            m_current = m_hash_map->m_buckets[i];
        }
        return *this;
    }

    template<typename Key, typename Val, typename Ref, typename Ptr, typename Hasher, typename Equals>
    inline OpenHashMapIterator<Key, Val, Ref, Ptr, Hasher, Equals>
    OpenHashMapIterator<Key, Val, Ref, Ptr, Hasher, Equals>::operator++(int) {
        self_type tmp = *this;
        ++*this;
        return tmp;
    }

}

#endif //CORE_STL_MAP_H
