/**
 * @file Map.h
 * @brief Hash map implementation.
 *
 * Hash map is implemented using separate chaining
 * because someone wanted remove operations.
 *
 * @author Jeff Niu
 * @date November 3, 2017
 * @bug No known bugs
 */

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#ifndef EMBEDDEDTESTS_CHAINMAP_H
#define EMBEDDEDTESTS_CHAINMAP_H

#include "Hash.h"
#include "Equal.h"
#include "Pair.h"
#include "../memory/Allocator.h"
#include "../memory/Memory.h"
#include "../exception/Exception.h"

namespace wlp {

    // Forward declaration of ChainHashMap
    template<class Key,
            class Val,
            class Hash,
            class Equal>
    class ChainHashMap;

    // Forward declaration of ChainHashMap iterator
    template<class Key,
            class Val,
            class Hash,
            class Equal>
    struct ChainHashMapIterator;

    // Forward declaration of const ChainHashMap iterator
    template<class Key,
            class Val,
            class Hash,
            class Equal>
    struct ChainHashMapConstIterator;

    /**
     * Hash map node comprise the elements of a hash map's
     * backing array, containing an element key and corresponding value.
     * Has pointer to next node in a chain.
     * @tparam Key key type
     * @tparam Val value type
     */
    template<class Key, class Val>
    struct ChainHashMapNode {
        typedef ChainHashMapNode<Key, Val> map_node;
        typedef Key key_type;
        typedef Val val_type;
        map_node* next = nullptr;
        key_type m_key;
        val_type m_val;
    };

    /**
     * Iterator class over the elements of a ChainHashMap. Specifically,
     * this class iterates through each chain and then the backing array.
     * @tparam Key   key type
     * @tparam Val   value type
     * @tparam Hash  hash function
     * @tparam Equal key equality function
     */
    template<class Key,
            class Val,
            class Hash,
            class Equal>
    struct ChainHashMapIterator {
        typedef ChainHashMap<Key, Val, Hash, Equal> hash_map;
        typedef ChainHashMapIterator<Key, Val, Hash, Equal> iterator;
        typedef ChainHashMapNode<Key, Val> map_node;

        typedef Val val_type;

        typedef uint16_t size_type;

        /**
         * Pointer to the node referenced by this iterator.
         */
        map_node* m_current;
        /**
         * Pointer to the iterated ChainHashMap.
         */
        hash_map* m_hash_map;

        /**
         * Default constructor.
         */
        ChainHashMapIterator() {}
        /**
         * Create an iterator to a ChainHashMap node.
         * @param node hash map node
         * @param map  parent hash map
         */
        ChainHashMapIterator(map_node* node, hash_map* map)
                : m_current(node), m_hash_map(map) {}

        /**
         * @return reference to the value of the node
         * pointed to by the iterator
         */
        val_type& operator*() const {
            return m_current->m_val;
        }
        /**
         * @return pointer to the value of the node
         * pointed to by the iterator
         */
        val_type* operator->() const {
            return &(operator*());
        }
        /**
         * Increment the iterator to the next available element in
         * the ChainHashMap. If no such element exists, returns pass-the-end
         * iterator. This is pre-fix unary operator.
         * @return this iterator
         */
        iterator& operator++();
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
        bool operator==(const iterator& it) const {
            return m_current == it.m_current;
        }
        /**
         * Compare two iterators, unequal if they point to
         * different nodes.
         * @param it iterator to compare
         * @return true if they point to different nodes
         */
        bool operator!=(const iterator& it) const {
            return m_current != it.m_current;
        }
    };

    /**
     * Constant iterator over a ChainHashMap. Values iterated by
     * this class cannot be modified.
     *
     * @see ChainHashMapIterator
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
    struct ChainHashMapConstIterator {
        typedef ChainHashMap<Key, Val, Hash, Equal> hash_map;
        typedef ChainHashMapConstIterator<Key, Val, Hash, Equal> const_iterator;
        typedef ChainHashMapNode<Key, Val> map_node;

        typedef Val val_type;

        typedef uint16_t size_type;

        const map_node* m_current;
        const hash_map* m_hash_map;

        ChainHashMapConstIterator() {}
        ChainHashMapConstIterator(map_node* node, const hash_map* map)
                : m_current(node), m_hash_map(map) {}

        val_type& operator*() const {
            return m_current->m_val;
        }
        val_type* operator->() const {
            return &(operator*());
        }
        const_iterator& operator++();
        const_iterator operator++(int);
        bool operator==(const const_iterator& it) const {
            return m_current == it.m_current;
        }
        bool operator!=(const const_iterator& it) const {
            return m_current != it.m_current;
        }
    };

    /**
     * Hash map implemented using separate chaining,
     * in the spirit of std::unordered_map.
     * @tparam Key   key type
     * @tparam Val   value type
     * @tparam Hash  hash function
     * @tparam Equal key equality function
     */
    template<class Key,
            class Val,
            class Hash = hash<Key, uint16_t>,
            class Equal = equals<Key>>
    class ChainHashMap {
    public:
        typedef ChainHashMapIterator<Key, Val, Hash, Equal> iterator;
        typedef ChainHashMapConstIterator<Key, Val, Hash, Equal> const_iterator;
        typedef ChainHashMapNode<Key, Val> map_node;

        typedef Key key_type;
        typedef Val val_type;

        typedef uint16_t size_type;
        typedef uint8_t percent_type;

        friend struct ChainHashMapIterator<Key, Val, Hash, Equal>;
        friend struct ChainHashMapConstIterator<Key, Val, Hash, Equal>;

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
        map_node** m_buckets;

        size_type m_num_elements;
        size_type m_max_elements;
        percent_type m_max_load;

    public:
        /**
         * Create and initialize an empty hash map. The hash map uses
         * {@code Allocator} to handle memory.
         *
         * @pre the hash map requires definition of an initial bucket array size
         *      and a maximum load factor before rehashing
         *
         * @param n        initial size of the bucket list; each bucket is initialized to nullptr
         * @param max_load an integer value denoting the max percent load factor, e.g. 100 = 1.00
         * @param hash     hash function for the key type, default is {@code wlp::Hash}
         * @param equal    equality function for the key type, default is {@code wlp::Equal}
         */
        explicit ChainHashMap(size_type n = 12,
                         percent_type max_load = 75)
                : m_max_load(max_load),
                  m_hash(Hash()),
                  m_equal(Equal()),
                  m_num_elements(0),
                  m_max_elements(n),
                  m_node_allocator{sizeof(map_node), static_cast<size_type>(n * sizeof(map_node))}{
            init_buckets(n);
        }

        /**
         * Destroy the hash map, freeing allocated nodes and
         * memory allocated for the array.
         */
        ~ChainHashMap();

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
            return m_hash(key) % m_max_elements;
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
        size_type max_size() const {
            return m_max_elements;
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
        const Allocator* get_node_allocator() const {
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
            for (size_type i = 0; i < m_max_elements; ++i) {
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
         * @see ChainHashMap<Key, Value, Hash, Equal>::begin()
         * @return a constant iterator to the first element
         */
        const_iterator begin() const {
            if (m_num_elements == 0) {
                return end();
            }
            for (size_type i = 0; i < m_max_elements; ++i) {
                if (m_buckets[i]) {
                    return const_iterator(m_buckets[i], this);
                }
            }
            return end();
        }

        /**
         * @see ChainHashMap<Key, Value, Hash, Equal>::end()
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
         * Erase an element pointed to by the provided pointer.
         * @param pos element to erase
         * @return the iterator to the next element in the map
         * or pass-the-end if there are no more elements afterwards
         */
        iterator erase(iterator& pos);
        /**
         * @see ChainHashMap<Key, Value, Hash, Equal>::erase()
         * @param pos const iterator to the element to erase
         * @return const iterator to the next element or pass-the-end
         */
        const_iterator erase(const_iterator& pos);
        /**
         * Erase
         * @param type
         * @return
         */
        bool erase(key_type& type);

        /**
         * Returns the value corresponding to a provided key.
         * @param key the key for which to find the value
         * @return the value mapped to by the key
         * @throws KeyException if the key does not map to a value
         */
        iterator at(const key_type& key);
        /**
         * @see ChainHashMap<Key, Value, Hash, Equal>::at()
         * @param key key for which to find the value
         * @return the mapped value
         * @throws KeyException if the key does not exist
         */
        const_iterator at(const key_type& key) const;
        /**
         * @param key key for which to check existence of a value
         * @return true if the key maps to a value
         */
        bool contains(const key_type& key) const;
        /**
         * Return an iterator to the map element corresponding
         * to the provided key, or pass-the-end if the key does
         * not map to any value in the map.
         * @param key the key to map
         * @return an iterator to the element mapped by the key
         */
        iterator find(const key_type& key);
        /**
         * @see ChainHashMap<Key, Value, Hash, Equal>::find()
         * @param key the key to map
         * @return a const iterator to the element mapped by the key
         */
        const_iterator find(const key_type& key) const;

        /**
         * Access an element in the hash map by the given key.
         * If the key does not map to any value in the map,
         * then a new value is created and inserted using the default
         * constructor.
         * @param key the key whose value to access
         * @return a reference to the mapped value
         */
        val_type& operator[](const key_type& key);
    };

    template<class Key, class Value, class Hash, class Equal>
    void ChainHashMap<Key, Value, Hash, Equal>::init_buckets(ChainHashMap<Key, Value, Hash, Equal>::size_type n)  {
        m_buckets = static_cast<map_node**>(memory_alloc(n * sizeof(map_node*)));
        for (size_type i = 0; i < n; i++) {
            m_buckets[i] = nullptr;
        }
    }

    template<class Key, class Value, class Hash, class Equal>
    void ChainHashMap<Key, Value, Hash, Equal>::ensure_capacity() {
        if (m_num_elements * 100 < m_max_load * m_max_elements) {
            return;
        }
        size_type new_max = static_cast<size_type>(m_max_elements * 2);
        map_node** new_buckets = static_cast<map_node**>(memory_alloc(new_max * sizeof(map_node*)));
        for (size_type i = 0; i < m_max_elements; ++i) {
            if (!m_buckets[i]) {
                continue;
            }
            map_node* cur = m_buckets[i];
            while (cur) {
                size_type k = bucket_index(cur->m_key, new_max);
                map_node* first = new_buckets[k];
                map_node* next = cur->next;
                cur->next = first;
                new_buckets[k] = cur;
                cur = next;
            }
        }
        memory_free(m_buckets);
        m_buckets = new_buckets;
        m_max_elements = new_max;
    }

    template<class Key, class Value, class Hash, class Equal>
    void ChainHashMap<Key, Value, Hash, Equal>::clear() noexcept {
        for (size_type i = 0; i < m_max_elements; i++) {
            map_node* cur = m_buckets[i];
            map_node* next;
            while (cur) {
                next = cur->next;
                m_node_allocator.Deallocate(cur);
                cur = next;
            }
            m_buckets[i] = nullptr;
        }
        m_num_elements = 0;
    }

    template<class Key, class Value, class Hash, class Equal>
    Pair<typename ChainHashMap<Key, Value, Hash, Equal>::iterator, bool>
    ChainHashMap<Key, Value, Hash, Equal>::insert(key_type key, val_type val) {
        ensure_capacity();
        size_type i = hash(key);
        map_node* first = m_buckets[i];
        for (map_node* cur = first; cur; cur = cur->next) {
            if (m_equal(cur->m_key, key)) {
                return Pair<iterator, bool>(iterator(cur, this), false);
            }
        }
        map_node* tmp = static_cast<map_node*>(m_node_allocator.Allocate());
        tmp->m_key = key;
        tmp->m_val = val;
        tmp->next = first;
        m_buckets[i] = tmp;
        ++m_num_elements;
        return Pair<iterator, bool>(iterator(tmp, this), true);
    };

    template<class Key, class Value, class Hash, class Equal>
    Pair<typename ChainHashMap<Key, Value, Hash, Equal>::iterator, bool>
    ChainHashMap<Key, Value, Hash, Equal>::insert_or_assign(key_type key, val_type val) {
        ensure_capacity();
        size_type i = hash(key);
        map_node* first = m_buckets[i];
        for (map_node* cur = first; cur; cur = cur->next) {
            if (m_equal(cur->m_key, key)) {
                cur->m_val = val;
                return Pair<iterator, bool>(iterator(cur, this), false);
            }
        }
        map_node* tmp = static_cast<map_node*>(m_node_allocator.Allocate());
        tmp->m_key = key;
        tmp->m_val = val;
        tmp->next = first;
        m_buckets[i] = tmp;
        ++m_num_elements;
        return Pair<iterator, bool>(iterator(tmp, this), true);
    };

    template<class Key, class Value, class Hash, class Equal>
    typename ChainHashMap<Key, Value, Hash, Equal>::iterator
    ChainHashMap<Key, Value, Hash, Equal>::erase(iterator& pos) {
        map_node* p_node = pos.m_current;
        if (p_node) {
            map_node* n_node = p_node->next;
            if (n_node) {
                p_node->next = n_node->next;
                p_node->m_key = n_node->m_key;
                p_node->m_val = n_node->m_val;
                m_node_allocator.Deallocate(n_node);
                --m_num_elements;
                return pos;
            } else {
                size_type i = hash(p_node->m_key);
                map_node* c_node = m_buckets[i];
                if (c_node == p_node) {
                    m_node_allocator.Deallocate(c_node);
                    --m_num_elements;
                    m_buckets[i] = nullptr;
                    while (++i < m_max_elements && !m_buckets[i]);
                    if (i == m_max_elements) {
                        pos.m_current = nullptr;
                        return pos;
                    } else {
                        pos.m_current = m_buckets[i];
                        return pos;
                    }
                } else {
                    while (c_node->next != p_node) {
                        c_node = c_node->next;
                    }
                    m_node_allocator.Deallocate(c_node->next);
                    --m_num_elements;
                    c_node->next = nullptr;
                    while (++i < m_max_elements && !m_buckets[i]);
                    if (i == m_max_elements) {
                        pos.m_current = nullptr;
                        return pos;
                    } else {
                        pos.m_current = m_buckets[i];
                        return pos;
                    }
                }
            }
        }
        pos.m_current = nullptr;
        return pos;
    }

    template<class Key, class Value, class Hash, class Equal>
    typename ChainHashMap<Key, Value, Hash, Equal>::const_iterator
    ChainHashMap<Key, Value, Hash, Equal>::erase(const_iterator &pos) {
        const map_node* p_node = pos.m_current;
        if (p_node) {
            size_type i = hash(p_node->m_key);
            map_node* c_node = m_buckets[i];
            if (p_node->next) {
                while (c_node != p_node) {
                    c_node = c_node->next;
                }
                c_node->m_key = c_node->next->m_key;
                c_node->m_val = c_node->next->m_val;
                c_node->next = c_node->next->next;
                m_node_allocator.Deallocate(p_node->next);
                --m_num_elements;
                return pos;
            } else {
                if (c_node == p_node) {
                    m_node_allocator.Deallocate(c_node);
                    --m_num_elements;
                    m_buckets[i] = nullptr;
                    while (++i < m_max_elements && !m_buckets[i]);
                    if (i == m_max_elements) {
                        pos.m_current = nullptr;
                        return pos;
                    } else {
                        pos.m_current = m_buckets[i];
                        return pos;
                    }
                } else {
                    while (c_node->next != p_node) {
                        c_node = c_node->next;
                    }
                    m_node_allocator.Deallocate(c_node->next);
                    --m_num_elements;
                    c_node->next = nullptr;
                    while (++i < m_max_elements && !m_buckets[i]);
                    if (i == m_max_elements) {
                        pos.m_current = nullptr;
                        return pos;
                    } else {
                        pos.m_current = m_buckets[i];
                        return pos;
                    }
                }
            }
        }
        pos.m_current = nullptr;
        return pos;
    }

    template<class Key, class Value, class Hash, class Equal>
    bool ChainHashMap<Key, Value, Hash, Equal>::erase(key_type& key) {
        size_type i = hash(key);
        map_node* first = m_buckets[i];
        if (first) {
            map_node* cur = first;
            map_node* next = cur->next;
            if (next) {
                if (m_equal(key, cur->m_key)) {
                    cur->m_key = next->m_key;
                    cur->m_val = next->m_val;
                    cur->next = next->next;
                    m_node_allocator.Deallocate(next);
                    --m_num_elements;
                    return true;
                }
                while (next && !m_equal(key, next->m_key)) {
                    cur = next;
                    next = cur->next;
                }
                if (next) {
                    cur->next = next->next;
                    m_node_allocator.Deallocate(next);
                    --m_num_elements;
                    return true;
                }
                return false;
            } else {
                if (!m_equal(key, cur->m_key)) {
                    return false;
                }
                m_node_allocator.Deallocate(cur);
                --m_num_elements;
                m_buckets[i] = nullptr;
                return true;
            }
        }
        return false;
    }

    template<class Key, class Value, class Hash, class Equal>
    typename ChainHashMap<Key, Value, Hash, Equal>::iterator
    ChainHashMap<Key, Value, Hash, Equal>::at(const key_type& key) {
        size_type i = hash(key);
        map_node* cur = m_buckets[i];
        if (!cur) {
            return end();
        }
        while (cur && !m_equal(key, cur->m_key)) {
            cur = cur->next;
        }
        if (!cur) {
            return end();
        }
        return iterator(cur, this);
    }

    template<class Key, class Value, class Hash, class Equal>
    typename ChainHashMap<Key, Value, Hash, Equal>::const_iterator
    ChainHashMap<Key, Value, Hash, Equal>::at(const key_type& key) const {
        size_type i = hash(key);
        map_node* cur = m_buckets[i];
        if (!cur) {
            return end();
        }
        while (cur && !m_equal(key, cur->m_key)) {
            cur = cur->next;
        }
        if (!cur) {
            return end();
        }
        return const_iterator(cur, this);
    }

    template<class Key, class Value, class Hash, class Equal>
    bool ChainHashMap<Key, Value, Hash, Equal>::contains(const key_type &key) const {
        size_type i = hash(key);
        map_node* cur = m_buckets[i];
        if (!cur) {
            return false;
        }
        while (cur && !m_equal(key, cur->m_key)) {
            cur = cur->next;
        }
        return cur != nullptr;
    }

    template<class Key, class Value, class Hash, class Equal>
    typename ChainHashMap<Key, Value, Hash, Equal>::val_type&
    ChainHashMap<Key, Value, Hash, Equal>::operator[](const key_type &key) {
        size_type i = hash(key);
        map_node* first = m_buckets[i];
        map_node* cur = first;
        if (!cur) {
            map_node *ele = static_cast<map_node *>(m_node_allocator.Allocate());
            ++m_num_elements;
            ele->m_key = key;
            ele->m_val = val_type();
            ele->next = nullptr;
            m_buckets[i] = ele;
            return ele->m_val;
        }
        while (cur && !m_equal(key, cur->m_key)) {
            cur = cur->next;
        }
        if (cur) {
            return cur->m_val;
        }
        cur = static_cast<map_node *>(m_node_allocator.Allocate());
        ++m_num_elements;
        cur->m_key = key;
        cur->m_val = val_type();
        cur->next = nullptr;
        cur->next = first;
        m_buckets[i] = cur;
        return cur->m_val;
    }

    template<class Key, class Value, class Hash, class Equal>
    typename ChainHashMap<Key, Value, Hash, Equal>::iterator
    ChainHashMap<Key, Value, Hash, Equal>::find(const key_type& key) {
        size_type i = hash(key);
        map_node* cur = m_buckets[i];
        if (!cur) {
            return end();
        }
        while (cur && !m_equal(key, cur->m_key)) {
            cur = cur->next;
        }
        if (!cur) {
            return end();
        }
        return iterator(cur, this);
    }

    template<class Key, class Value, class Hash, class Equal>
    typename ChainHashMap<Key, Value, Hash, Equal>::const_iterator
    ChainHashMap<Key, Value, Hash, Equal>::find(const key_type& key) const {
        size_type i = hash(key);
        map_node* cur = m_buckets[i];
        if (!cur) {
            return end();
        }
        while (cur && !m_equal(key, cur->m_key)) {
            cur = cur->next;
        }
        if (!cur) {
            return end();
        }
        return const_iterator(cur, this);
    }

    template<class Key, class Value, class Hash, class Equal>
    ChainHashMap<Key, Value, Hash, Equal>::~ChainHashMap() {
        for (size_type i = 0; i < m_max_elements; i++) {
            map_node* cur = m_buckets[i];
            if (cur) {
                while (cur) {
                    map_node* next = cur->next;
                    m_node_allocator.Deallocate(cur);
                    cur = next;
                }
            }
        }
        memory_free(m_buckets);
        m_buckets = nullptr;
    }

    template<class Key, class Value, class Hash, class Equal>
    ChainHashMapIterator<Key, Value, Hash, Equal>&
    ChainHashMapIterator<Key, Value, Hash, Equal>::operator++() {
        const map_node* old = m_current;
        m_current = m_current->next;
        if (!m_current) {
            size_type i = m_hash_map->hash(old->m_key);
            while (++i < m_hash_map->m_max_elements && !m_hash_map->m_buckets[i]);
            if (i == m_hash_map->m_max_elements) {
                m_current = nullptr;
            } else {
                m_current = m_hash_map->m_buckets[i];
            }
        }
        return *this;
    }

    template<class Key, class Value, class Hash, class Equal>
    inline ChainHashMapIterator<Key, Value, Hash, Equal>
    ChainHashMapIterator<Key, Value, Hash, Equal>::operator++(int) {
        iterator tmp = *this;
        ++*this;
        return tmp;
    }

    template<class Key, class Value, class Hash, class Equal>
    ChainHashMapConstIterator<Key, Value, Hash, Equal>&
    ChainHashMapConstIterator<Key, Value, Hash, Equal>::operator++() {
        const map_node* old = m_current;
        m_current = m_current->next;
        if (!m_current) {
            size_type i = m_hash_map->hash(old->m_key);
            while (++i < m_hash_map->m_max_elements && !m_hash_map->m_buckets[i]);
            if (i == m_hash_map->m_max_elements) {
                m_current = nullptr;
            } else {
                m_current = m_hash_map->m_buckets[i];
            }
        }
        return *this;
    }

    template<class Key, class Value, class Hash, class Equal>
    inline ChainHashMapConstIterator<Key, Value, Hash, Equal>
    ChainHashMapConstIterator<Key, Value, Hash, Equal>::operator++(int) {
        const_iterator tmp = *this;
        ++*this;
        return tmp;
    }

}

#pragma clang diagnostic pop

#endif //EMBEDDEDTESTS_CHAINMAP_H
