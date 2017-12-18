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

#include "../Types.h"
#include "RedBlackTree.h"
#include "Tuple.h"

namespace wlp {

    template<typename Key, typename Val>
    struct TreeMapGetKey {
        typedef Tuple<Key, Val> element_type;
        typedef Key key_type;

        template<typename E>
        const key_type &operator()(E &&element) const {
            return get<0>(forward<E>(element));
        }
    };

    template<typename Key, typename Val>
    struct TreeMapGetVal {
        typedef Tuple<Key, Val> element_type;
        typedef Val val_type;

        template<typename E>
        val_type &operator()(E &&element) const {
            return get<1>(forward<E>(element));
        }
    };

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
        typedef RedBlackTree<Key, Val, Cmp, TreeMapGetKey<Key, Val>, TreeMapGetVal<Key, Val>> table_type;
        typedef typename table_type::node_type node_type;
        typedef typename table_type::iterator iterator;
        typedef typename table_type::const_iterator const_iterator;
        typedef typename table_type::size_type size_type;

        typedef Key key_type;
        typedef Val val_type;

    private:
        table_type m_tree;

    public:
        explicit TreeMap()
                : m_tree() {
        }

        TreeMap(const map_type &) = delete;

        TreeMap(map_type &&map)
                : m_tree(move(map.m_tree)) {
        }

        size_type size() const {
            return m_tree.size();
        }

        size_type capacity() const {
            return m_tree.capacity();
        }

        bool empty() const {
            return m_tree.empty();
        }

        const table_type *get_backing_table() const {
            return &m_tree;
        }

        iterator begin() {
            return m_tree.begin();
        }

        const_iterator begin() const {
            return m_tree.begin();
        }

        iterator end() {
            return m_tree.end();
        }

        const_iterator end() const {
            return m_tree.end();
        }

        void clear() noexcept {
            m_tree.clear();
        }

        template<typename K, typename V>
        Pair<iterator, bool> insert(K &&key, V &&val) {
            return m_tree.insert_unique(make_tuple(forward<K>(key), forward<V>(val)));
        };

        template<typename K, typename V>
        Pair<iterator, bool> insert_or_assign(K &&key, V &&val) {
            Pair<iterator, bool> result = m_tree.insert_unique(make_tuple(forward<K>(key), forward<V>(val)));
            if (!result.m_second) {
                *result.m_first = forward<V>(val);
            }
            return result;
        };

        iterator erase(const iterator &pos) {
            iterator tmp = pos;
            ++tmp;
            m_tree.erase(pos);
            return tmp;
        }

        bool erase(const key_type &key) {
            return m_tree.erase(key) > 0;
        }

        val_type &at(const key_type &key) {
            return *m_tree.find(key);
        }

        const val_type &at(const key_type &key) const {
            return *m_tree.find(key);
        }

        bool contains(const key_type &key) const {
            return m_tree.find(key) != m_tree.end();
        }

        iterator find(const key_type &key) {
            return m_tree.find(key);
        }

        const_iterator find(const key_type &key) const {
            return m_tree.find(key);
        }

        template<typename K>
        val_type &operator[](K &&key) {
            Pair<iterator, bool> result = m_tree.insert_unique(make_tuple(forward<K>(key), val_type()));
            return *result.m_first;
        }

        map_type &operator=(const map_type &) = delete;

        map_type &operator=(map_type &&map) {
            m_tree = move(map.m_tree);
            return *this;
        }
    };

}

#endif //EMBEDDEDCPLUSPLUS_TREEMAP_H
