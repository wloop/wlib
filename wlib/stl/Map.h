/**
 * @file Map.h
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

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#ifndef CORE_STL_MAP_H
#define CORE_STL_MAP_H

#include <exception/Exception.h>
#include "Hash.h"
#include "Equal.h"
#include "Pair.h"
#include "memory/Allocator.h"
#include "exception/Exception.h"

namespace wlp {

    // Forward declaration of HashMap
    template<class Key,
            class Val,
            class Hash,
            class Equal>
    class HashMap;

    // Forward declaration of HashMap iterator
    template<class Key,
            class Val,
            class Hash,
            class Equal>
    struct HashMapIterator;

    // Forward declaration of const HashMap iterator
    template<class Key,
            class Val,
            class Hash,
            class Equal>
    struct HashMapConstIterator;

    /**
     * Hash map node comprise the elements of a hash map's
     * backing array, containing an element key and corresponding value.
     * @tparam Key key type
     * @tparam Val value type
     */
    template<class Key, class Val>
    struct HashMapNode {
        typedef Key key_type;
        typedef Val val_type;
        key_type m_key;
        val_type m_val;
    };

    /**
     * Iterator class over the elements of a HashMap. Specifically,
     * this class iterates from start to end of the HashMap's backing
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
    struct HashMapIterator {
        typedef HashMap<Key, Val, Hash, Equal> hash_map;
        typedef HashMapIterator<Key, Val, Hash, Equal> iterator;
        typedef HashMapNode<Key, Val> map_node;

        typedef Val val_type;

        typedef uint16_t size_type;

        /**
         * Pointer to the node referenced by this iterator.
         */
        map_node* m_current;
        /**
         * Pointer to the iterated HashMap.
         */
        hash_map* m_hash_map;

        /**
         * Create an iterator to a HashMap node.
         * @param node hash map node
         * @param map  parent hash map
         */
        HashMapIterator(map_node* node, hash_map* map)
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
         * the HashMap. If no such element exists, returns pass-the-end
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
     * Constant iterator over a HashMap. Values iterated by
     * this class cannot be modified.
     *
     * @see HashMapIterator
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
    struct HashMapConstIterator {
        typedef HashMap<Key, Val, Hash, Equal> hash_map;
        typedef HashMapConstIterator<Key, Val, Hash, Equal> const_iterator;
        typedef HashMapNode<Key, Val> map_node;

        typedef Val val_type;

        typedef uint16_t size_type;

        const map_node* m_current;
        const hash_map* m_hash_map;

        HashMapConstIterator(map_node* node, const hash_map* map)
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
     * Hash map implemented using open addressing and linear probing,
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
    class HashMap {
    public:
        typedef HashMapIterator<Key, Val, Hash, Equal> iterator;
        typedef HashMapConstIterator<Key, Val, Hash, Equal> const_iterator;
        typedef HashMapNode<Key, Val> map_node;

        typedef Key key_type;
        typedef Val val_type;

        typedef uint16_t size_type;
        typedef uint8_t percent_type;

        friend struct HashMapIterator<Key, Val, Hash, Equal>;
        friend struct HashMapConstIterator<Key, Val, Hash, Equal>;

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
         * Allocator for the hash map backing array of
         * pointers to hash map nodes.
         */
        Allocator m_array_allocator;

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
         * {@code Allocator} to handle memory. The hash map is implemented
         * with open addressing and linear probing.
         *
         * @pre the hash map requires definition of an initial bucket array size
         *      and a maximum load factor before rehashing
         *
         * @param n        initial size of the bucket list; each bucket is initialized to nullptr
         * @param max_load an integer value denoting the max percent load factory, e.g. 75 = 0.75
         * @param hash     hash function for the key type, default is {@code wlp::Hash}
         * @param equal    equality function for the key type, default is {@code wlp::Equal}
         */
        explicit HashMap(size_type n = 12,
                percent_type max_load = 75)
                : m_max_load(max_load),
                  m_hash(Hash()),
                  m_equal(Equal()),
                  m_num_elements(0),
                  m_max_elements(n),
                  m_node_allocator{sizeof(map_node), n * sizeof(map_node)},
                  m_array_allocator{n * sizeof(map_node*), n * sizeof(map_node*)}{
            init_buckets(n);
        }

        /**
         * Destroy the hash map, freeing allocated nodes and
         * memory allocated for the array.
         */
        ~HashMap();

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
            return iterator(0, this);
        }

        /**
         * @see HashMap::begin()
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
         * @see HashMap::end()
         * @return a constant pass-the-end iterator
         */
        const_iterator end() const {
            return const_iterator(0, this);
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
         * @see HashMap::erase()
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
        val_type& at(const key_type& key);
        /**
         * @see HashMap::at()
         * @param key key for which to find the value
         * @return the mapped value
         * @throws KeyException if the key does not exist
         */
        const val_type& at(const key_type& key) const;
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
         * @see HashMap::find()
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

    void HashMap::init_buckets(HashMap::size_type n)  {
        m_buckets = static_cast<map_node**>(m_array_allocator.Allocate());
        for (size_type i = 0; i < n; i++) {
            m_buckets[i] = nullptr;
        }
    }

    void HashMap::ensure_capacity() {
        if (m_num_elements * 100 < m_max_load * m_max_elements) {
            return;
        }
        size_type new_max = static_cast<size_type>(m_max_elements * 2);
        Allocator new_array_allocator = Allocator(new_max * sizeof(map_node*), new_max * sizeof(map_node*));
        map_node** new_buckets = static_cast<map_node**>(new_array_allocator.Allocate());
        for (size_type i = 0; i < m_max_elements; ++i) {
            if (!m_buckets[i]) {
                continue;
            }
            map_node* node = m_buckets[i];
            new_buckets[bucket_index(node->m_key, new_max)] = node;
        }
        m_array_allocator.Deallocate(m_buckets);
        m_buckets = new_buckets;
        m_array_allocator = new_array_allocator;
        m_max_elements = new_max;
    }

    void HashMap::clear() noexcept {
        for (size_type i = 0; i < m_max_elements; i++) {
            if (m_buckets[i]) {
                m_node_allocator.Deallocate(m_buckets[i]);
                m_buckets[i] = nullptr;
            }
        }
        m_num_elements = 0;
    }

    Pair<iterator, bool> HashMap::insert(key_type key, val_type val) {
        ensure_capacity();
        size_type i = hash(key);
        while(m_buckets[i] && !m_equal(key, m_buckets[i]->m_key)) {
            if (++i >= m_max_elements) {
                i = 0;
            }
        }
        if (m_buckets[i]) {
            return Pair<iterator, bool>(iterator(m_buckets[i], this), false);
        } else {
            ++m_num_elements;
            map_node* node = static_cast<map_node *>(m_node_allocator.Allocate());
            node->m_key = key;
            node->m_val = val;
            m_buckets[i] = node;
            return Pair<iterator, bool>(iterator(node, this), true);
        }
    };

    Pair<iterator, bool> HashMap::insert_or_assign(key_type key, val_type val) {
        ensure_capacity();
        size_type i = hash(key);
        while(m_buckets[i] && !m_equal(key, m_buckets[i]->m_key)) {
            if (++i >= m_max_elements) {
                i = 0;
            }
        }
        if (m_buckets[i]) {
            m_buckets[i]->m_val = val;
            return Pair<iterator, bool>(iterator(m_buckets[i], this), false);
        } else {
            ++m_num_elements;
            map_node* node = static_cast<map_node *>(m_node_allocator.Allocate());
            node->m_key = key;
            node->m_val = val;
            m_buckets[i] = node;
            return Pair<iterator, bool>(iterator(node, this), true);
        }
    };

    iterator HashMap::erase(iterator& pos) {
        map_node* p_node = pos.m_current;
        if (p_node) {
            size_type i = hash(p_node->m_key);
            map_node* cur_node = m_buckets[i];
            if (cur_node == p_node) {
                ++pos;
                --m_num_elements;
                m_node_allocator.Deallocate(p_node);
                m_buckets[i] = nullptr;
                return pos;
            } else {
                while (++i < m_max_elements && p_node != m_buckets[i]);
                if (i == m_max_elements) {
                    return end();
                }
                ++pos;
                --m_num_elements;
                m_node_allocator.Deallocate(p_node);
                m_buckets[i] = nullptr;
                return pos;
            }
        }
        return end();
    }

    const_iterator HashMap::erase(const_iterator &pos) {
        const map_node* p_node = pos.m_current;
        if (p_node) {
            size_type i = hash(p_node->m_key);
            map_node* cur_node = m_buckets[i];
            if (cur_node == p_node) {
                ++pos;
                --m_num_elements;
                m_node_allocator.Deallocate(m_buckets[i]);
                m_buckets[i] = nullptr;
                return pos;
            } else {
                while (++i < m_max_elements && p_node != m_buckets[i]);
                if (i == m_max_elements) {
                    return end();
                }
                ++pos;
                --m_num_elements;
                m_node_allocator.Deallocate(m_buckets[i]);
                m_buckets[i] = nullptr;
                return pos;
            }
        }
        return end();
    }

    bool HashMap::erase(key_type& key) {
        size_type i = hash(key);
        while(m_buckets[i] && !m_equal(key, m_buckets[i]->m_key)) {
            if (++i >= m_max_elements) {
                i = 0;
            }
        }
        if (m_buckets[i]) {
            --m_num_elements;
            m_node_allocator.Deallocate(m_buckets[i]);
            m_buckets[i] = nullptr;
            return true;
        } else {
            return false;
        }
    }

    val_type& HashMap::at(const key_type& key) {
        size_type i = hash(key);
        while (m_buckets[i] && !m_equal(key, m_buckets[i]->m_key)) {
            if (++i >= m_max_elements) {
                i = 0;
            }
        }
        if (m_buckets[i]) {
            return m_buckets[i]->m_val;
        } else {
            throw KeyException<key_type>(key);
        }
    }

    const val_type& HashMap::at(const key_type& key) const {
        size_type i = hash(key);
        while (m_buckets[i] && !m_equal(key, m_buckets[i]->m_key)) {
            if (++i >= m_max_elements) {
                i = 0;
            }
        }
        if (m_buckets[i]) {
            return m_buckets[i]->m_val;
        } else {
            throw KeyException<key_type>(key);
        }
    }

    bool HashMap::contains(const key_type &key) const {
        size_type i = hash(key);
        while (m_buckets[i]) {
            if (m_equal(key, m_buckets[i]->m_key)) {
                return true;
            }
            if (++i >+ m_max_elements) {
                i = 0;
            }
        }
        return false;
    }

    val_type& HashMap::operator[](const key_type &key) {
        size_type i = hash(key);
        while (m_buckets[i] && !m_equal(key, m_buckets[i]->m_key)) {
            if (++i >= m_max_elements) {
                i = 0;
            }
        }
        if (m_buckets[i]) {
            return m_buckets[i]->m_val;
        } else {
            ++m_num_elements;
            map_node* node = static_cast<map_node *>(m_node_allocator.Allocate());
            node->m_key = key;
            node->m_val = val_type();
            return node->m_val;
        }
    }

    iterator HashMap::find(const key_type& key) {
        size_type i = hash(key);
        while (m_buckets[i] && !m_equal(key, m_buckets[i]->m_key)) {
            if (++i >= m_max_elements) {
                i = 0;
            }
        }
        if (m_buckets[i]) {
            return iterator(m_buckets[i], this);
        } else {
            return end();
        }
    }

    const_iterator HashMap::find(const key_type& key) const {
        size_type i = hash(key);
        while (m_buckets[i] && !m_equal(key, m_buckets[i]->m_key)) {
            if (++i >= m_max_elements) {
                i = 0;
            }
        }
        if (m_buckets[i]) {
            return const_iterator(m_buckets[i], this);
        } else {
            return end();
        }
    }

    HashMap::~HashMap() {
        for (size_type i = 0; i < m_max_elements; i++) {
            if (m_buckets[i]) {
                m_node_allocator.Deallocate(m_buckets[i]);
                m_buckets[i] = nullptr;
            }
        }
        m_array_allocator.Deallocate(m_buckets);
        m_buckets = nullptr;
    }

    iterator &HashMapIterator::operator++() {
        size_type i = m_hash_map->bucket_index(m_current->m_key, m_hash_map->m_num_elements);
        while(!m_hash_map->m_buckets[++i] && i < m_hash_map->m_max_elements);
        if (m_hash_map->m_buckets[i]) {
            m_current = m_hash_map->m_buckets[i];
        }
        return *this;
    }

    iterator HashMapIterator::operator++(int) {
        iterator tmp = *this;
        ++*this;
        return tmp;
    }

    const_iterator &HashMapConstIterator::operator++() {
        size_type i = m_hash_map->bucket_index(m_current->m_key, m_hash_map->m_num_elements);
        while(!m_hash_map->m_buckets[++i] && i < m_hash_map->m_max_elements);
        if (m_hash_map->m_buckets[i]) {
            m_current = m_hash_map->m_buckets[i];
        }
        return *this;
    }

    const_iterator HashMapConstIterator::operator++(int) {
        const_iterator tmp = *this;
        ++*this;
        return tmp;
    }

}

#endif //CORE_STL_MAP_H

#pragma clang diagnostic pop