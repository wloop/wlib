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

#ifndef CORE_STL_HASH_TABLE_H
#define CORE_STL_HASH_TABLE_H

#include "Equal.h"
#include "Hash.h"
#include "Pair.h"

#include "wlib/memory/Memory.h"
#include "wlib/utility/Utility.h"
#include "wlib/exceptions/Exceptions.h"

namespace wlp {

    // Forward declaration of OpenHashTable
    template<typename Element,
            typename Key,
            typename Val,
            typename GetKey,
            typename GetVal,
            typename Hasher,
            typename Equals>
    class OpenHashTable;

    /**
     * Iterator class over the elements of a OpenHashTable. Specifically,
     * this class iterates from start to end of the OpenHashTable's backing
     * array, returning past-the-end afterwards.
     *
     * @tparam Element element type containing the value
     * @tparam Val     value type
     * @tparam Ref     reference type to value
     * @tparam Ptr     pointer type to value
     * @tparam GetVal  functor type to obtain value from element
     */
    template<typename Element,
            typename Key,
            typename Val,
            typename Ref,
            typename Ptr,
            typename GetKey,
            typename GetVal,
            typename Hasher,
            typename Equals>
    struct OpenHashTableIterator {
        typedef OpenHashTableIterator<Element, Key, Val, Ref, Ptr, GetKey, GetVal, Hasher, Equals> self_type;
        typedef OpenHashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals> table_type;

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
        element_type *m_node;
        /**
         * Pointer to the hash map to which this iterator belongs.
         */
        const table_type *m_table;
        /**
         * Functor used to obtain element key.
         */
        get_key m_get_key{};
        /**
         * Functor used to obtain element value.
         */
        get_value m_get_value{};

        /**
         * Default constructor.
         */
        OpenHashTableIterator()
                : m_node(nullptr),
                  m_table(nullptr) {
        }

        /**
         * Create an iterator to a OpenHashTable node.
         * @param node hash map node
         * @param map  parent hash map
         */
        OpenHashTableIterator(element_type *node, const table_type *hash_map)
                : m_node(node),
                  m_table(hash_map) {
        }

        /**
         * Copy constructor for const.
         * @param it iterator to copy
         */
        OpenHashTableIterator(const self_type &it)
                : m_node(it.m_node),
                  m_table(it.m_table) {
        }

        /**
         * @return reference to the value of the node
         * pointed to by the iterator
         */
        reference operator*() const {
            if (m_node == nullptr) {
                THROW(KEY_EXCEPTION("Accessing invalid iterator"))
            }
            return m_get_value(*m_node);
        }

        /**
         * @return pointer to the value of the node
         * pointed to by the iterator
         */
        pointer operator->() const {
            if (m_node == nullptr) {
                THROW(KEY_EXCEPTION("Accessing invalid iterator"))
            }
            return &(operator*());
        }

        /**
         * Increment the iterator to the next available element in
         * the OpenHashTable. If no such element exists, returns pass-the-end
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
            return m_node == it.m_node;
        }

        /**
         * Compare two iterators, unequal if they point to
         * different nodes.
         * @param it iterator to compare
         * @return true if they point to different nodes
         */
        bool operator!=(const self_type &it) const {
            return m_node != it.m_node;
        }

        /**
         * Assignment operator copies pointers to node
         * and hash map.
         * @param it iterator to copy
         * @return a reference to this iterator
         */
        self_type &operator=(const self_type &it) {
            m_node = it.m_node;
            m_table = it.m_table;
            return *this;
        }

    };

    /**
     * Hash map implemented using open addressing and linear probing,
     * in the spirit of std::unordered_map.
     *
     * @tparam Element element type containing the key and value
     * @tparam Key     key type
     * @tparam Val     value type
     * @tparam GetKey  functor for obtaining key from element
     * @tparam GetVal  functor for obtaining value from element
     * @tparam Hasher  hash function functor
     * @tparam Equals  key equality functor
     */
    template<typename Element,
            typename Key,
            typename Val,
            typename GetKey,
            typename GetVal,
            typename Hasher = Hash <Key, uint16_t>,
            typename Equals = Equal <Key>>
    class OpenHashTable {
    public:
        typedef OpenHashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals> table_type;
        typedef OpenHashTableIterator<
                Element, Key,
                Val, Val &, Val *,
                GetKey, GetVal,
                Hasher, Equals
        > iterator;
        typedef OpenHashTableIterator<
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

        friend struct OpenHashTableIterator<
                Element, Key,
                Val, Val &, Val *,
                GetKey, GetVal,
                Hasher, Equals>;
        friend struct OpenHashTableIterator<
                Element, Key, Val,
                const Val &, const Val *,
                GetKey, GetVal,
                Hasher, Equals>;

    private:
        /**
         * Class hash function instance. Used to hash
         * element keys.
         */
        hash_function m_hash_function{};
        /**
         * Class key equality function. Used to test
         * equality of element keys.
         */
        key_equals m_key_equals{};
        /**
         * Functor to obtain key from element.
         */
        get_key m_get_key{};

        /**
         * Hash map backing array.
         */
        element_type **m_buckets;

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
        explicit OpenHashTable(
                size_type n = 12,
                percent_type max_load = 75)
                : m_num_elements(0),
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
        OpenHashTable(const table_type &) = delete;

        /**
         * Move constructor transfers resources from
         * rvalue hash map into this hash map.
         * @param map map from which to transfer
         */
        OpenHashTable(table_type &&map)
                : m_buckets(move(map.m_buckets)),
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
        ~OpenHashTable();

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
            return m_hash_function(key) % max_elements;
        }

        /**
         * Obtain the bucket index of a key in the backing array.
         * @param key the key to hash
         * @return an index i such that 0 <= i < m_max_elements
         */
        size_type hash(const key_type &key) const {
            return m_hash_function(key) % m_capacity;
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
         * @see OpenHashTable<Key, Val, Hasher, Equals>::begin()
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
         * @return a pass-the-end iterator for this map
         */
        iterator end() {
            return iterator(nullptr, this);
        }

        /**
         * @see OpenHashTable<Key, Val, Hasher, Equals>::end()
         * @return a constant pass-the-end iterator
         */
        const_iterator end() const {
            return const_iterator(nullptr, this);
        }

        /**
         * @return true if the map is empty
         */
        bool empty() const {
            return m_num_elements == 0;
        }

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
        template<typename E>
        Pair<iterator, bool> insert_unique(E &&element);

        /**
         * Erase the element from the map pointed to by the provided
         * iterator. Be aware that erasure operations on an openly
         * addressed hash map will trigger a rehash and invalidate
         * all iterators other than the return value.
         *
         * @param pos iterator pointing to the element to erase
         * @return iterator to the next element in the map or end
         */
        void erase(const iterator &pos);

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
        size_type erase(const key_type &key);

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
         * @see OpenHashTable<Key, Val, Hasher, Equals>::find()
         * @param key the key to map
         * @return a const iterator to the element mapped by the key
         */
        const_iterator find(const key_type &key) const;

        /**
         * Copy assignment operators are disabled.
         *
         * @return hash map reference
         */
        table_type &operator=(const table_type &) = delete;

        /**
         * Move assignment operator. Assigned hash map
         * will have its resources transferred into this
         * hash map. Existing resources will be released.
         *
         * @param map map to move
         * @return reference to this map
         */
        table_type &operator=(table_type &&map);
    };

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    void OpenHashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::init_buckets(OpenHashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>::size_type n) {
        m_buckets = malloc<element_type *[]>(n);
        for (size_type i = 0; i < n; ++i) {
            m_buckets[i] = nullptr;
        }
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    void OpenHashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::ensure_capacity() {
        if (m_num_elements * 100 < m_max_load * m_capacity) {
            return;
        }
        size_type new_capacity = static_cast<size_type>(m_capacity * 2);
        element_type **new_buckets = malloc<element_type *[]>(new_capacity);
        for (size_type i = 0; i < new_capacity; ++i) {
            new_buckets[i] = nullptr;
        }
        for (size_type i = 0; i < m_capacity; ++i) {
            if (!m_buckets[i]) {
                continue;
            }
            element_type *node = m_buckets[i];
            size_type k = bucket_index(m_get_key(*node), new_capacity);
            while (new_buckets[k]) {
                if (++k >= new_capacity) {
                    k = 0;
                }
            }
            new_buckets[k] = node;
        }
        free<element_type *>(m_buckets);
        m_buckets = new_buckets;
        m_capacity = new_capacity;
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    void OpenHashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::clear() noexcept {
        for (size_type i = 0; i < m_capacity; ++i) {
            if (m_buckets[i]) {
                free<element_type>(m_buckets[i]);
                m_buckets[i] = nullptr;
            }
        }
        m_num_elements = 0;
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    template<typename E>
    Pair<typename OpenHashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>::iterator, bool>
    OpenHashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::insert_unique(E &&element) {
        ensure_capacity();
        size_type i = hash(m_get_key(element));
        while (m_buckets[i] && !m_key_equals(m_get_key(element), m_get_key(*m_buckets[i]))) {
            if (++i >= m_capacity) {
                i = 0;
            }
        }
        if (m_buckets[i]) {
            return Pair<iterator, bool>(iterator(m_buckets[i], this), false);
        } else {
            ++m_num_elements;
            element_type *node = malloc<element_type>();
            *node = forward<E>(element);
            m_buckets[i] = node;
            return Pair<iterator, bool>(iterator(node, this), true);
        }
    };

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    void OpenHashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::erase(const iterator &pos) {
        const element_type *cur_node = pos.m_node;
        if (!cur_node) {
            return;
        }
        size_type i = hash(m_get_key(*cur_node));
        while (m_buckets[i] && !m_key_equals(m_get_key(*cur_node), m_get_key(*m_buckets[i]))) {
            if (++i >= m_capacity) {
                i = 0;
            }
        }
        if (!m_buckets[i]) {
            return;
        }
        --m_num_elements;
        free<element_type>(m_buckets[i]);
        m_buckets[i] = nullptr;
        element_type **new_buckets = malloc<element_type *[]>(m_capacity);
        for (size_type k = 0; k < m_capacity; k++) {
            new_buckets[k] = nullptr;
        }
        for (size_type k = 0; k < m_capacity; k++) {
            if (!m_buckets[k]) {
                continue;
            }
            element_type *node = m_buckets[k];
            size_type j = hash(m_get_key(*node));
            while (new_buckets[j]) {
                if (++j >= m_capacity) {
                    j = 0;
                }
            }
            new_buckets[j] = node;
        }
        free<element_type *>(m_buckets);
        m_buckets = new_buckets;
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    size_type OpenHashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::erase(const key_type &key) {
        size_type i = hash(key);
        while (m_buckets[i] && !m_key_equals(key, m_get_key(*m_buckets[i]))) {
            if (++i >= m_capacity) {
                i = 0;
            }
        }
        if (!m_buckets[i]) {
            return 0;
        }
        --m_num_elements;
        free<element_type>(m_buckets[i]);
        m_buckets[i] = nullptr;
        element_type **new_buckets = malloc<element_type *[]>(m_capacity);
        for (size_type k = 0; k < m_capacity; k++) {
            new_buckets[k] = nullptr;
        }
        for (size_type k = 0; k < m_capacity; k++) {
            if (!m_buckets[k]) {
                continue;
            }
            element_type *node = m_buckets[k];
            size_type j = hash(m_get_key(*node));
            while (new_buckets[j]) {
                if (++j >= m_capacity) {
                    j = 0;
                }
            }
            new_buckets[j] = node;
        }
        free<element_type *>(m_buckets);
        m_buckets = new_buckets;
        return 1;
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    inline typename OpenHashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>::iterator
    OpenHashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::find(const key_type &key) {
        size_type i = hash(key);
        while (m_buckets[i] && !m_key_equals(key, m_get_key(*m_buckets[i]))) {
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

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    inline typename OpenHashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>::const_iterator
    OpenHashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::find(const key_type &key) const {
        size_type i = hash(key);
        while (m_buckets[i] && !m_key_equals(key, m_get_key(*m_buckets[i]))) {
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

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    OpenHashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::~OpenHashTable() {
        if (!m_buckets) {
            return;
        }
        for (size_type i = 0; i < m_capacity; ++i) {
            if (m_buckets[i]) {
                free<element_type>(m_buckets[i]);
                m_buckets[i] = nullptr;
            }
        }
        free<element_type *>(m_buckets);
        m_buckets = nullptr;
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    OpenHashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals> &
    OpenHashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals>
    ::operator=(OpenHashTable<Element, Key, Val, GetKey, GetVal, Hasher, Equals> &&map) {
        clear();
        free<element_type *>(m_buckets);
        m_capacity = move(map.m_capacity);
        m_max_load = move(map.m_max_load);
        m_num_elements = move(map.m_num_elements);
        m_buckets = move(map.m_buckets);
        map.m_capacity = 0;
        map.m_num_elements = 0;
        map.m_buckets = nullptr;
        return *this;
    }

    template<typename Element, typename Key, typename Val,
            typename Ref, typename Ptr,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    OpenHashTableIterator<Element, Key, Val, Ref, Ptr, GetKey, GetVal, Hasher, Equals> &
    OpenHashTableIterator<Element, Key, Val, Ref, Ptr, GetKey, GetVal, Hasher, Equals>
    ::operator++() {
        if (!m_node) {
            return *this;
        }
        size_type i = m_table->hash(m_get_key(*m_node));
        while (m_table->m_buckets[i] && !m_table->m_key_equals(m_get_key(*m_node), m_get_key(*m_table->m_buckets[i]))) {
            if (++i >= m_table->m_capacity) {
                i = 0;
            }
        }
        while (++i < m_table->m_capacity && !m_table->m_buckets[i]) {}
        if (i == m_table->m_capacity) {
            m_node = nullptr;
        } else {
            m_node = m_table->m_buckets[i];
        }
        return *this;
    }

    template<typename Element, typename Key, typename Val,
            typename Ref, typename Ptr,
            typename GetKey, typename GetVal,
            typename Hasher, typename Equals>
    inline OpenHashTableIterator<Element, Key, Val, Ref, Ptr, GetKey, GetVal, Hasher, Equals>
    OpenHashTableIterator<Element, Key, Val, Ref, Ptr, GetKey, GetVal, Hasher, Equals>::operator++(int) {
        self_type tmp = *this;
        ++*this;
        return tmp;
    }

}

#endif //CORE_STL_HASH_TABLE_H
