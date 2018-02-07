/**
 * @file TreeMap.h
 * @brief Map implementation using red black tree.
 *
 * @author Jeff Niu
 * @date November 14, 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDCPLUSPLUS_TREEMAP_H
#define EMBEDDEDCPLUSPLUS_TREEMAP_H

#include "RedBlackTree.h"
#include "Table.h"
#include "Tuple.h"
#include "wlib/Types.h"

namespace wlp {

    /**
     * Map implementation using @code RedBlackTree @endcode as the
     * backing data structure.
     *
     * @see wlp::ChainHashMap
     * @see wlp::RedBlackTree
     *
     * @tparam Key key type
     * @tparam Val value type
     * @tparam Cmp key comparator type, which uses the default comparator
     */
    template<typename Key, typename Val, typename Cmp = Comparator<Key>>
    class TreeMap {
    public:
        typedef TreeMap<Key, Val, Cmp> map_type;
        typedef RedBlackTree<Tuple<Key, Val>,
                Key, Val,
                MapGetKey<Key, Val>, MapGetVal<Key, Val>,
                Cmp
        > table_type;
        typedef typename table_type::iterator iterator;
        typedef typename table_type::const_iterator const_iterator;
        typedef typename table_type::size_type size_type;

        typedef Key key_type;
        typedef Val val_type;

    private:
        table_type m_table;

    public:
        explicit TreeMap()
                : m_table() {
        }

        TreeMap(const map_type &) = delete;

        TreeMap(map_type &&map)
                : m_table(move(map.m_table)) {
        }

        size_type size() const {
            return m_table.size();
        }

        size_type capacity() const {
            return m_table.capacity();
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
        };

        template<typename K, typename V>
        Pair<iterator, bool> insert_or_assign(K &&key, V &&val) {
            Pair<iterator, bool> result = m_table.insert_unique(make_tuple(forward<K>(key), forward<V>(val)));
            if (!result.m_second) {
                *result.m_first = forward<V>(val);
            }
            return result;
        };

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
            Pair<iterator, bool> result = m_table.insert_unique(make_tuple(forward<K>(key), val_type()));
            return *result.m_first;
        }

        map_type &operator=(const map_type &) = delete;

        map_type &operator=(map_type &&map) {
            m_table = move(map.m_table);
            return *this;
        }
    };

}

#endif //EMBEDDEDCPLUSPLUS_TREEMAP_H
