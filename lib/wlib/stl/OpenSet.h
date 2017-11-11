/**
 * @file OpenSet.h
 * @brief Hash set implementation.
 *
 * Set implementation using an open addressed
 * hash map as the backing structure.
 *
 * @author Jeff Niu
 * @date November 4, 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDCPLUSPLUS_OPENSET_H
#define EMBEDDEDCPLUSPLUS_OPENSET_H

#include "Hash.h"
#include "Equal.h"
#include "OpenMap.h"

namespace wlp {

    /**
     * An open hash set is created using a backing hash map,
     * and all available functions are a subset of the functions
     * of the hash map. The set contains unique elements.
     * @tparam Key   the unique element type
     * @tparam Hash  the hash function of the stored elements
     * @tparam Equal test for equality function of the stored elements
     */
    template<class Key,
            class Hash = Hash<Key, uint16_t>,
            class Equal = Equal<Key>>
    class OpenHashSet {
    public:
        typedef OpenHashSet<Key, Hash, Equal> hash_set;
        typedef OpenHashMap<Key, Key, Hash, Equal> map_type;
        typedef OpenHashMapIterator<Key, Key, Hash, Equal> iterator;
        typedef OpenHashMapConstIterator<Key, Key, Hash, Equal> const_iterator;
        typedef map_type::size_type size_type;
        typedef map_type::percent_type percent_type;
        typedef map_type::key_type key_type;

    private:
        /**
         * The backing hash map.
         */
        map_type m_hash_map;

    public:
        /**
         * Constructor creates a new hash map, and instantiates
         * the backing hash map.
         *
         * @see OpenHashMap
         * @param n        the initial size of the backing array
         * @param max_load the maximum load factor before rehash
         */
        explicit OpenHashSet(
                size_type n = 12,
                percent_type max_load = 75) :
                m_hash_map(n, max_load) {
        }

        OpenHashSet(const hash_set &) = delete;

        OpenHashSet(hash_set &&set)
                : m_hash_map(move(set.m_hash_map)) {
        }

        /**
         * @return the current number of elements in the set
         */
        size_type size() const {
            return m_hash_map.size();
        }

        /**
         * @return the size of the backing array
         */
        size_type capacity() const {
            return m_hash_map.capacity();
        }

        /**
         * @return the maximum load factor
         */
        percent_type max_load() const {
            return m_hash_map.max_load();
        }

        /**
         * @return whether the hash map is empty
         */
        bool empty() const {
            return m_hash_map.empty();
        }

        /**
         * @return a pointer to the backing map's node allocator
         */
        const Allocator *get_node_allocator() const {
            return m_hash_map.get_node_allocator();
        }

        /**
         * @return a pointer to the backing hash map
         */
        const map_type *get_backing_hash_map() const {
            return &m_hash_map;
        }

        /**
         * An iterator instance to the beginning of the hash set.
         * The iterator order of the set is not guaranteed to
         * be in any particular order.
         * @return iterator to the first element, or @code end @endcode
         * if the set is empty
         */
        iterator begin() {
            return m_hash_map.begin();
        }

        /**
         * A pass-the-end iterator instance. This iterator means
         * that an iterator has read past the end of the set
         * and has become invalid.
         * @return a pass-the-end iterator
         */
        iterator end() {
            return m_hash_map.end();
        }

        /**
         * @return a constant iterator to the first element
         */
        const_iterator begin() const {
            return m_hash_map.begin();
        }

        /**
         * @return a constant pass-the-end iterator
         */
        const_iterator end() const {
            return m_hash_map.end();
        }

        /**
         * Empty the elements in the set, such that its
         * size is now zero.
         */
        void clear() noexcept {
            m_hash_map.clear();
        }

        /**
         * Insert an element into the set. This function
         * returns a pair consistent of the iterator to the
         * element that was inserted or to the element that
         * prevented insertion. The second boolean value indicates
         * whether insertion took place.
         * @param key the element to insert
         * @return a pair of an iterator and boolean
         */
        Pair<iterator, bool> insert(key_type key) {
            return m_hash_map.insert(key, key);
        };

        /**
         * Check if an element is in the set.
         * @param key element whose existence to check
         * @return true if the element is contained in the set
         */
        bool contains(const key_type &key) const {
            return m_hash_map.contains(key);
        }

        /**
         * Obtain an iterator to an element in the set.
         * @param key the element to find
         * @return iterator to the element or pass-the-end
         * if the element is not in the set
         */
        iterator find(const key_type &key) {
            return m_hash_map.find(key);
        }

        /**
         * Obtain a const iterator to an element in the set
         * @param key the element to find
         * @return const iterator to the element or pass-the-end
         * if the element is not in the set
         */
        const_iterator find(const key_type &key) const {
            return m_hash_map.find(key);
        }

        iterator &erase(iterator &pos) {
            return m_hash_map.erase(pos);
        }

        const_iterator &erase(const_iterator &pos) {
            return m_hash_map.erase(pos);
        }

        bool erase(key_type &key) {
            return m_hash_map.erase(key);
        }

        hash_set &operator=(const hash_set &) = delete;

        hash_set &operator=(hash_set &&set) {
            m_hash_map = move(set.m_hash_map);
            return *this;
        }
    };

}


#endif //EMBEDDEDCPLUSPLUS_OPENSET_H
