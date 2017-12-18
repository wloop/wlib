/**
 * @file TreeSet.h
 * @brief Set implementation using red black tree.
 *
 * @author Jeff Niu
 * @date November 14, 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDCPLUSPLUS_TREESET_H
#define EMBEDDEDCPLUSPLUS_TREESET_H

#include "Table.h"
#include "RedBlackTree.h"

namespace wlp {

    /**
     * Set implementation using @code RedBlackTree @endcode as the
     * backing data structure.
     *
     * @see wlp::ChainHashSet
     * @see wlp::RedBlackTree
     *
     * @tparam Key stored value type
     * @tparam Cmp comparator for stored value, which uses the default comparator
     */
    template<typename Key, typename Cmp = Comparator<Key>>
    class TreeSet {
    public:
        typedef TreeSet<Key, Cmp> set_type;
        typedef RedBlackTree<Key,
            Key, Key,
            SetGetKey<Key>, SetGetVal<Key>,
            Cmp
        > table_type;
        typedef typename table_type::iterator iterator;
        typedef typename table_type::const_iterator const_iterator;
        typedef typename table_type::size_type size_type;

        typedef Key key_type;

    private:
        table_type m_table;

    public:
        explicit TreeSet()
                : m_table() {
        }

        TreeSet(const set_type &) = delete;

        TreeSet(set_type &&set)
                : m_table(move(set.m_table)) {
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

#endif //EMBEDDEDCPLUSPLUS_TREESET_H
