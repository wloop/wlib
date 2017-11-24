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
        typedef RedBlackTree<Key, Key, Cmp> table_type;
        typedef typename RedBlackTree<Key, Key, Cmp>::iterator iterator;
        typedef typename RedBlackTree<Key, Key, Cmp>::const_iterator const_iterator;
        typedef typename RedBlackTree<Key, Key, Cmp>::size_type size_type;

        typedef Key key_type;

    private:
        table_type m_tree;

    public:
        explicit TreeSet()
                : m_tree() {
        }

        TreeSet(const set_type &) = delete;

        TreeSet(set_type &&set)
                : m_tree(move(set.m_tree)) {
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

        template<typename K>
        Pair<iterator, bool> insert(K &&key) {
            return m_tree.insert_unique(forward<K>(key), forward<K>(key));
        };

        bool contains(const key_type &key) const {
            return m_tree.find(key) != m_tree.end();
        }

        iterator find(const key_type &key) {
            return m_tree.find(key);
        }

        const_iterator find(const key_type &key) const {
            return m_tree.find(key);
        }

        iterator erase(const iterator &pos) {
            iterator tmp = pos;
            ++tmp;
            m_tree.erase(pos);
            return tmp;
        }

        bool erase(const key_type &key) {
            return m_tree.erase(key) > 0;
        }

        set_type &operator=(const set_type &) = delete;

        set_type &operator=(set_type &&set) {
            m_tree = move(set.m_tree);
            return *this;
        }

    };

}

#endif //EMBEDDEDCPLUSPLUS_TREESET_H
