/**
 * @file ChainSet.h
 * @brief Hash set implementation.
 *
 * Set implementation using a separately chained
 * hash map as the backing structure.
 *
 * @author Jeff Niu
 * @date November 4, 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDCPLUSPLUS_CHAINSET_H
#define EMBEDDEDCPLUSPLUS_CHAINSET_H

#include "Equal.h"
#include "Hash.h"
#include "HashTable.h"
#include "Table.h"

namespace wlp {

    /**
     * Hash set implementation using separate chaining. This implementation
     * supports removal operations, unlike the open addressed set.
     *
     * @tparam Key   the element type
     * @tparam Hash  the hash function
     * @tparam Equal the equality function
     */
    template<class Key,
            class Hasher = Hash <Key, uint16_t>,
            class Equals = Equal <Key>>
    class HashSet {
    public:
        typedef HashSet<Key, Hasher, Equals> set_type;
        typedef HashTable<Key, Key, Key, SetGetKey<Key>, SetGetVal<Key>, Hasher, Equals> table_type;

        typedef typename table_type::iterator iterator;
        typedef typename table_type::const_iterator const_iterator;
        typedef typename table_type::size_type size_type;
        typedef typename table_type::percent_type percent_type;

        typedef Key key_type;

    private:
        table_type m_table;

    public:
        /**
         * Construct a chained hash set, which creates
         * the backing chain hash map instance.
         *
         * @param n        the initial size of the backing array
         * @param max_load the maximum load factor before rehash
         */
        explicit HashSet(size_type n = 12, percent_type max_load = 75)
                : m_table(n, max_load) {
        }

        HashSet(const set_type &) = delete;

        HashSet(set_type &&set)
                : m_table(move(set.m_table)) {
        }

        size_type size() const {
            return m_table.size();
        }

        size_type capacity() const {
            return m_table.capacity();
        }

        percent_type max_load() const {
            return m_table.max_load();
        }

        bool empty() const {
            return m_table.empty();
        }

        const table_type *get_backing_table() const {
            return &m_table;
        }

        iterator begin() {
            return m_table.begin();
        }

        iterator end() {
            return m_table.end();
        }

        const_iterator begin() const {
            return m_table.begin();
        }

        const_iterator end() const {
            return m_table.end();
        }

        void clear() noexcept {
            m_table.clear();
        }

        template<typename K>
        Pair<iterator, bool> insert(K &&key) {
            return m_table.insert_unique(key);
        };

        bool contains(const key_type &key) const {
            return m_table.find(key) != m_table.end();
        }

        iterator find(const key_type &key) {
            return m_table.find(key);
        }

        const_iterator find(const key_type &key) const {
            return m_table.find(key);
        }

        iterator erase(const iterator &pos) {
            iterator tmp = pos;
            ++tmp;
            m_table.erase(pos);
            return tmp;
        }

        bool erase(const key_type &key) {
            return m_table.erase(key) > 0;
        }

        set_type &operator=(const set_type &) = delete;

        set_type &operator=(set_type &&set) {
            m_table = move(set.m_table);
            return *this;
        }
    };

}

#endif //EMBEDDEDCPLUSPLUS_CHAINSET_H
