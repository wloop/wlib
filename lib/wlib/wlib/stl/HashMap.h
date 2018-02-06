/**
 * @file ChainMap.h
 * @brief Hasher map implementation.
 *
 * Hasher map is implemented using separate chaining
 * because someone wanted remove operations.
 *
 * @author Jeff Niu
 * @date November 3, 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDTESTS_CHAINMAP_H
#define EMBEDDEDTESTS_CHAINMAP_H

#include "Equal.h"
#include "Hash.h"
#include "HashTable.h"
#include "Pair.h"
#include "Table.h"
#include "Tuple.h"

#include "wlib/utility/Utility.h"

namespace wlp {

    /**
     * Hash map implemented using separate chaining,
     * in the spirit of std::unordered_map.
     *
     * @tparam Key    key type
     * @tparam Val    value type
     * @tparam Hasher hash function
     * @tparam Equals key equality function
     */
    template<typename Key,
            typename Val,
            typename Hasher = Hash<Key, uint16_t>,
            typename Equals = Equal<Key>>
    class HashMap {
    public:
        typedef HashMap<Key, Val, Hasher, Equals> map_type;
        typedef HashTable<Tuple<Key, Val>,
                Key, Val,
                MapGetKey<Key, Val>, MapGetVal<Key, Val>,
                Hasher, Equals
        > table_type;
        typedef typename table_type::iterator iterator;
        typedef typename table_type::const_iterator const_iterator;
        typedef typename table_type::size_type size_type;
        typedef typename table_type::percent_type percent_type;

        typedef Key key_type;
        typedef Val val_type;

    private:
        table_type m_table;

    public:
        explicit HashMap(size_type n = 12, percent_type max_load = 75)
                : m_table(n, max_load) {
        }

        HashMap(const map_type &) = delete;

        HashMap(map_type &&map)
                : m_table(move(map.m_table)) {
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

        iterator begin() {
            return m_table.begin();
        }

        const_iterator begin() const {
            return m_table.begin();
        }

        iterator end() {
            return m_table.end();
        }

        const_iterator end() const {
            return m_table.end();
        }

        void clear() noexcept {
            m_table.clear();
        }

        template<typename K, typename V>
        Pair<iterator, bool> insert(K &&key, V &&val) {
            return m_table.insert_unique(make_tuple(forward<K>(key), forward<V>(val)));
        }

        template<typename K, typename V>
        Pair<iterator, bool> insert_or_assign(K &&key, V &&val) {
            Pair<iterator, bool> result = m_table.insert_unique(make_tuple(forward<K>(key), forward<V>(val)));
            if (!result.m_second) {
                *result.m_first = forward<V>(val);
            }
            return result;
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

        val_type &at(const key_type &key) {
            return *m_table.find(key);
        }

        const val_type &at(const key_type &key) const {
            return *m_table.find(key);
        }

        bool contains(const key_type &key) const {
            return m_table.find(key) != m_table.end();
        }

        iterator find(const key_type &key) {
            return m_table.find(key);
        }

        const_iterator find(const key_type &key) const {
            return m_table.find(key);
        }

        template<typename K>
        val_type &operator[](K &&key) {
            return get<1>(m_table.find_or_insert(make_tuple(forward<K>(key), val_type())));
        }

        map_type &operator=(const map_type &) = delete;

        map_type &operator=(map_type &&map) {
            m_table = move(map.m_table);
            return *this;
        }
    };

}

#endif //EMBEDDEDTESTS_CHAINMAP_H
