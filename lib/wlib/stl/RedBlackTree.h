#ifndef EMBEDDEDCPLUSPLUS_REDBLACKTREE_H
#define EMBEDDEDCPLUSPLUS_REDBLACKTREE_H

#include "Utility.h"
#include "Comparator.h"

#include "../Types.h"

#include "../memory/Allocator.h"
#include "Pair.h"

namespace wlp {

    struct RedBlackTreeColor {
        typedef bool type;
        static constexpr type RED = false;
        static constexpr type BLACK = true;
    };

    template<typename Key, typename Val>
    struct RedBlackTreeNode {
        typedef RedBlackTreeNode<Key, Val> node_type;
        typedef Key key_type;
        typedef Val val_type;

    private:
        typedef RedBlackTreeColor::type color;


    public:
        color m_color;

        node_type *m_parent;
        node_type *m_left;
        node_type *m_right;

        key_type m_key;
        val_type m_val;

        static node_type *find_minimum(node_type *node) {
            while (node->m_left) {
                node = node->m_left;
            }
            return node;
        }

        static node_type *find_maximum(node_type *node) {
            while (node->m_right) {
                node = node->m_right;
            }
            return node;
        }
    };

    template<typename Key, typename Val>
    struct RedBlackTreeIterator {
        typedef RedBlackTreeNode<Key, Val> node_type;
        typedef RedBlackTreeIterator<Key, Val> iterator;
        typedef RedBlackTreeColor color;
        typedef Key key_type;
        typedef Val val_type;

        node_type *m_node;

    public:
        RedBlackTreeIterator(node_type *node)
                : m_node(node) {
        }

        RedBlackTreeIterator(const iterator &it)
                : m_node(it.m_node) {
        }

        RedBlackTreeIterator(iterator &&it)
                : m_node(move(it.m_node)) {
            it.m_node = nullptr;
        }

        void increment() {
            if (m_node->m_right) {
                m_node = m_node->m_right;
                while (m_node->m_left) {
                    m_node = m_node->m_left;
                }
            } else {
                node_type *parent = m_node->m_parent;
                while (m_node == parent->m_right) {
                    m_node = parent;
                    parent = parent->m_parent;
                }
                if (m_node->m_right != parent) {
                    m_node = parent;
                }
            }
        }

        void decrement() {
            if (m_node->m_color == color::RED && m_node->m_parent->m_parent == m_node) {
                m_node = m_node->m_right;
            } else if (m_node->m_left) {
                node_type *child = m_node->m_left;
                while (child->m_right) {
                    child = child->m_right;
                }
                m_node = child;
            } else {
                node_type *parent = m_node->m_parent;
                while (m_node == parent->m_left) {
                    m_node = parent;
                    parent = parent->m_parent;
                }
                m_node = parent;
            }
        }

        bool operator==(const iterator &it) const {
            return m_node == it.m_node;
        }

        bool operator==(iterator &&it) const {
            return m_node == it.m_node;
        }

        bool operator!=(const iterator &it) const {
            return m_node != it.m_node;
        }

        bool operator!=(iterator &&it) const {
            return m_node != it.m_node;
        }

        val_type &operator*() const {
            return m_node->m_val;
        }

        val_type *operator->() const {
            return &m_node->m_val;
        }

        iterator &operator++() {
            increment();
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            increment();
            return tmp;
        }

        iterator &operator--() {
            decrement();
            return *this;
        }

        iterator operator--(int) {
            iterator tmp = *this;
            decrement();
            return tmp;
        }

        key_type &key() const {
            return m_node->m_key;
        }
    };

    template<typename Key, typename Val, typename Cmp = Comparator<Key>>
    class RedBlackTree {
    public:
        typedef Key key_type;
        typedef Val val_type;
        typedef Cmp comparator;
        typedef wlp::size_type size_type;
        typedef RedBlackTreeNode<Key, Val> node_type;
        typedef RedBlackTree<Key, Val, Cmp> tree_type;
        typedef RedBlackTreeIterator<Key, Val> iterator;

    protected:
        typedef RedBlackTreeColor color;

        Allocator m_node_allocator;
        node_type *m_header;
        size_type m_size;
        comparator m_cmp;

        node_type *create_node() {
            return static_cast<node_type *>(m_node_allocator.Allocate());
        }

        node_type *copy_node(node_type *node) {
            node_type *copy = create_node();
            copy->m_key = node->m_key;
            copy->m_val = node->m_val;
            copy->m_color = node->m_color;
            copy->m_left = node->m_left;
            copy->m_right = node->m_right;
            return copy;
        }

        void destroy_node(node_type *node) {
            m_node_allocator.Deallocate(node);
        }

        void rotateLeft(node_type *node, node_type *&root);

        void rotateRight(node_type *node, node_type *&root);

        void rebalance(node_type *node, node_type *&root);

        node_type *eraseRebalance(
                node_type *node,
                node_type *&root,
                node_type *&leftmost,
                node_type *&rightmost
        );

        iterator insert(node_type *node, node_type *piv, const key_type &key, const val_type &val);

        void erase(node_type *node);

        void empty_initialize() {
            m_header->m_color = color::RED;
            m_header->m_parent = nullptr;
            m_header->m_left = m_header;
            m_header->m_right = m_header;
        }

    public:
        RedBlackTree(size_type n = 12)
                : m_node_allocator(sizeof(node_type), n * sizeof(node_type)),
                  m_header(nullptr),
                  m_size(0),
                  m_cmp() {
            m_header = create_node();
            empty_initialize();
        }

        ~RedBlackTree() {
            clear();
            destroy_node(m_header);
        }

        iterator begin() {
            return iterator(m_header->m_left);
        }

        iterator end() {
            return iterator(m_header);
        }

        bool empty() const {
            return m_size == 0;
        }

        size_type size() const {
            return m_size;
        }

        size_type capacity() const {
            return static_cast<size_type>(-1);
        }

        void clear() {
            if (m_size > 0) {
                erase(m_header->m_parent);
                m_header->m_parent = nullptr;
                m_header->m_left = m_header;
                m_header->m_right = m_header;
                m_size = 0;
            }
        }

        Pair<iterator, bool> insert_unique(const key_type &key, const val_type &val);

        iterator insert_equal(const key_type &key, const val_type &val);

        void erase(const iterator &pos);

        size_type erase(const key_type &key);

        size_type erase(const iterator &first, const iterator &last);

        iterator find(const key_type &key);

        size_type count(const key_type &key) const;

        iterator lower_bound(const key_type &key) const;

        iterator upper_bound(const key_type &key) const;

        Pair<iterator, iterator> equal_range(const key_type &val) const;

    };

    template<typename Key, typename Val, typename Cmp>
    inline void RedBlackTree<Key, Val, Cmp>::rotateLeft(node_type *node, node_type *&root) {
        node_type *carry = node->m_right;
        node->m_right = carry->m_left;
        if (carry->m_left) {
            carry->m_left->m_parent = node;
        }
        carry->m_parent = node->m_parent;
        if (node == root) {
            root = carry;
        } else if (node == node->m_parent->m_left) {
            node->m_parent->m_left = carry;
        } else {
            node->m_parent->m_right = carry;
        }
        carry->m_left = node;
        node->m_parent = carry;
    }

    template<typename Key, typename Val, typename Cmp>
    inline void RedBlackTree<Key, Val, Cmp>::rotateRight(node_type *node, node_type *&root) {
        node_type *carry = node->m_left;
        node->m_left = carry->m_right;
        if (carry->m_right) {
            carry->m_right->m_parent = node;
        }
        carry->m_parent = node->m_parent;
        if (node == root) {
            root = carry;
        } else if (node == node->m_parent->m_right) {
            node->m_parent->m_right = carry;
        } else {
            node->m_parent->m_left = carry;
        }
        carry->m_right = node;
        node->m_parent = carry;
    }

    template<typename Key, typename Val, typename Cmp>
    inline void RedBlackTree<Key, Val, Cmp>::rebalance(node_type *node, node_type *&root) {
        node->m_color = color::RED;
        while (node != root && node->m_parent->m_color == color::RED) {
            if (node->m_parent == node->m_parent->m_parent->m_left) {
                node_type *carry = node->m_parent->m_parent->m_right;
                if (carry && carry->m_color == color::RED) {
                    node->m_parent->m_color = color::BLACK;
                    carry->m_color = color::BLACK;
                    node->m_parent->m_parent->m_color = color::RED;
                    node = node->m_parent->m_parent;
                } else {
                    if (node == node->m_parent->m_right) {
                        node = node->m_parent;
                        rotateLeft(node, root);
                    }
                    node->m_parent->m_color = color::BLACK;
                    node->m_parent->m_parent->m_color = color::RED;
                    rotateRight(node->m_parent->m_parent, root);
                }
            } else {
                node_type *carry = node->m_parent->m_parent->m_left;
                if (carry && carry->m_color == color::RED) {
                    node->m_parent->m_color = color::BLACK;
                    carry->m_color = color::BLACK;
                    node->m_parent->m_parent->m_color = color::RED;
                    node = node->m_parent->m_parent;
                } else {
                    if (node == node->m_parent->m_left) {
                        node = node->m_parent;
                        rotateRight(node, root);
                    }
                    node->m_parent->m_color = color::BLACK;
                    node->m_parent->m_parent->m_color = color::RED;
                    rotateLeft(node->m_parent->m_parent, root);
                }
            }
        }
        root->m_color = color::BLACK;
    }

    template<typename Key, typename Val, typename Cmp>
    inline typename RedBlackTree<Key, Val, Cmp>::node_type *
    RedBlackTree<Key, Val, Cmp>::eraseRebalance(
            node_type *node,
            node_type *&root,
            node_type *&leftmost,
            node_type *&rightmost) {
        node_type *carry = node;
        node_type *cur = 0;
        node_type *cur_parent = 0;
        if (!carry->m_left) {
            // node has at most one non-null child
            // carry == node and cur might be null
            cur = carry->m_right;
        } else if (!carry->m_right) {
            // node has exactly one non-null child
            // carry == node and cur is not null
            cur = carry->m_left;
        } else {
            // node has two non-null children
            // set cur to node's successor and cur might be null
            carry = carry->m_right;
            while (carry->m_left) {
                carry = carry->m_left;
            }
            cur = carry->m_right;
        }
        if (carry != node) {
            // Relink cur in place of node
            // cur is node's successor
            node->m_left->m_parent = carry;
            carry->m_left = node->m_left;
            if (carry != node->m_right) {
                cur_parent = carry->m_parent;
                if (cur) {
                    // carry must be a child of m_left
                    cur->m_parent = carry->m_parent;
                }
                carry->m_parent->m_left = cur;
                carry->m_right = node->m_right;
                node->m_right->m_parent = carry;
            } else {
                cur_parent = carry;
            }
            if (root == node) {
                root = carry;
            } else if (node->m_parent->m_left == node) {
                node->m_parent->m_left = carry;
            } else {
                node->m_parent->m_right = carry;
            }
            carry->m_parent = node->m_parent;
            swap(carry->m_color, node->m_color);
            // carry now points to node that is deleted
            carry = node;
        } else {
            // here carry == node
            cur_parent = carry->m_parent;
            if (cur) {
                cur->m_parent = carry->m_parent;
            }
            if (root == node) {
                root = cur;
            } else if (node->m_parent->m_left == node) {
                node->m_parent->m_left = cur;
            } else {
                node->m_parent->m_right = cur;
            }
            if (leftmost == node) {
                // node->m_left might also be null
                if (!node->m_right) {
                    // makes leftmost == header if node == root
                    leftmost = node->m_parent;
                } else {
                    leftmost = node_type::find_minimum(cur);
                }
            }
            if (rightmost == node) {
                // node->m_rught might also be null
                if (!node->m_left) {
                    // makes rightmost == header if node == root
                    rightmost = node->m_parent;
                } else {
                    // cur == node->m_left
                    rightmost = node_type::find_maximum(cur);
                }
            }
        }
        if (carry->m_color != color::RED) {
            while (cur != root && (!cur || cur->m_color == color::BLACK)) {
                if (cur == cur_parent->m_left) {
                    node_type *aux = cur_parent->m_right;
                    if (aux->m_color == color::RED) {
                        aux->m_color = color::BLACK;
                        cur_parent->m_color = color::RED;
                        rotateLeft(cur_parent, root);
                        aux = cur_parent->m_right;
                    }
                    if ((!aux->m_left || aux->m_left->m_color == color::BLACK) &&
                        (!aux->m_right || aux->m_right->m_color == color::BLACK)) {
                        aux->m_color = color::RED;
                        cur = cur_parent;
                        cur_parent = cur_parent->m_parent;
                    } else {
                        if (!aux->m_right || aux->m_right->m_color == color::BLACK) {
                            if (aux->m_left) {
                                aux->m_left->m_color = color::BLACK;
                            }
                            aux->m_color = color::RED;
                            rotateRight(aux, root);
                            aux = cur_parent->m_right;
                        }
                        aux->m_color = cur_parent->m_color;
                        cur_parent->m_color = color::BLACK;
                        if (aux->m_right) {
                            aux->m_right->m_color = color::BLACK;
                        }
                        rotateLeft(cur_parent, root);
                        break;
                    }
                } else {
                    // same as above but with left and right switched
                    node_type *aux = cur_parent->m_left;
                    if (aux->m_color == color::RED) {
                        aux->m_color = color::BLACK;
                        cur_parent->m_color = color::RED;
                        rotateRight(cur_parent, root);
                        aux = cur_parent->m_left;
                    }
                    if ((!aux->m_right || aux->m_right->m_color == color::BLACK) &&
                        (!aux->m_left || aux->m_left->m_color == color::BLACK)) {
                        aux->m_color = color::RED;
                        cur = cur_parent;
                        cur_parent = cur_parent->m_parent;
                    } else {
                        if (!aux->m_left || aux->m_left->m_color == color::BLACK) {
                            if (aux->m_right) {
                                aux->m_right->m_color = color::BLACK;
                            }
                            aux->m_color = color::RED;
                            rotateLeft(aux, root);
                            aux = cur_parent->m_left;
                        }
                        aux->m_color = cur_parent->m_color;
                        cur_parent->m_color = color::BLACK;
                        if (aux->m_left) {
                            aux->m_left->m_color = color::BLACK;
                        }
                        rotateRight(cur_parent, root);
                        break;
                    }
                }
            }
            if (cur) {
                cur->m_color = color::BLACK;
            }
        }
        return carry;
    }

    template<typename Key, typename Val, typename Cmp>
    typename RedBlackTree<Key, Val, Cmp>::iterator
    RedBlackTree<Key, Val, Cmp>::insert(node_type *cur, node_type *carry, const key_type &key, const val_type &val) {
        node_type *node = create_node();
        node->m_key = key;
        node->m_val = val;
        if (carry == m_header || cur || m_cmp.__lt__(key, carry->m_key)) {
            carry->m_left = node;
            if (carry == m_header) {
                m_header->m_parent = node;
                m_header->m_right = node;
            } else if (carry == m_header->m_left) {
                m_header->m_left = node;
            }
        } else {
            carry->m_right = node;
            if (carry == m_header->m_right) {
                m_header->m_right = node;
            }
        }
        node->m_parent = carry;
        node->m_left = nullptr;
        node->m_right = nullptr;
        rebalance(node, m_header->m_parent);
        ++m_size;
        return iterator(node);
    }

    template<typename Key, typename Val, typename Cmp>
    Pair<typename RedBlackTree<Key, Val, Cmp>::iterator, bool>
    RedBlackTree<Key, Val, Cmp>::insert_unique(const key_type &key, const val_type &val) {
        node_type *carry = m_header;
        node_type *cur = m_header->m_parent;
        bool compare = true;
        while (cur) {
            carry = cur;
            compare = m_cmp.__lt__(key, cur->m_key);
            cur = compare ? cur->m_left : cur->m_right;
        }
        iterator tmp = iterator(carry);
        if (compare) {
            if (tmp == begin()) {
                return Pair<iterator, bool>(insert(cur, carry, key, val), true);
            } else {
                --tmp;
            }
        }
        if (m_cmp.__lt__(tmp.m_node->m_key, key)) {
            return Pair<iterator, bool>(insert(cur, carry, key, val), true);
        }
        return Pair<iterator, bool>(tmp, false);
    }

    template<typename Key, typename Val, typename Cmp>
    typename RedBlackTree<Key, Val, Cmp>::iterator
    RedBlackTree<Key, Val, Cmp>::insert_equal(const key_type &key, const val_type &val) {
        node_type *carry = m_header;
        node_type *cur = m_header->m_parent;
        while (cur) {
            carry = cur;
            cur = m_cmp.__lt__(key, cur->m_key) ? cur->m_left : cur->m_right;
        }
        return insert(cur, carry, key, val);
    }

    template<typename Key, typename Val, typename Cmp>
    inline void RedBlackTree<Key, Val, Cmp>::erase(node_type *node) {
        while (node) {
            erase(node->m_right);
            node_type *carry = node->m_left;
            destroy_node(node);
            node = carry;
        }
    }

    template<typename Key, typename Val, typename Cmp>
    inline void RedBlackTree<Key, Val, Cmp>::erase(const iterator &pos) {
        node_type *carry = eraseRebalance(pos.m_node, m_header->m_parent, m_header->m_left, m_header->m_right);
        destroy_node(carry);
        --m_size;
    }

    template<typename Key, typename Val, typename Cmp>
    inline typename RedBlackTree<Key, Val, Cmp>::size_type
    RedBlackTree<Key, Val, Cmp>::erase(const key_type &cur) {
        Pair<iterator, iterator> res = equal_range(cur);
        return erase(res.m_first, res.m_second);
    }

    template<typename Key, typename Val, typename Cmp>
    inline size_type RedBlackTree<Key, Val, Cmp>::erase(const iterator &first, const iterator &last) {
        size_type count;
        if (first == begin() && last == end()) {
            count = m_size;
            clear();
        } else {
            count = 0;
            iterator tmp = first;
            while (tmp != last) {
                erase(tmp++);
                ++count;
            }
        }
        return count;
    }

    template<typename Key, typename Val, typename Cmp>
    typename RedBlackTree<Key, Val, Cmp>::iterator
    RedBlackTree<Key, Val, Cmp>::find(const key_type &key) {
        node_type *carry = m_header;
        node_type *cur = m_header->m_parent;
        while (cur) {
            if (!m_cmp.__lt__(cur->m_key, key)) {
                carry = cur, cur = cur->m_left;
            } else {
                cur = cur->m_right;
            }
        }
        iterator tmp = iterator(carry);
        return (tmp == end() || m_cmp.__lt__(key, tmp.m_node->m_key)) ? end() : tmp;
    }

    template<typename Key, typename Val, typename Cmp>
    typename RedBlackTree<Key, Val, Cmp>::size_type
    RedBlackTree<Key, Val, Cmp>::count(const key_type &key) const {
        Pair<iterator, iterator> res = equal_range(key);
        size_type count = 0;
        while (res.m_first != res.m_second) {
            ++res.m_first;
            ++count;
        }
        return count;
    }

    template<typename Key, typename Val, typename Cmp>
    typename RedBlackTree<Key, Val, Cmp>::iterator
    RedBlackTree<Key, Val, Cmp>::lower_bound(const key_type &key) const {
        node_type *carry = m_header;
        node_type *cur = m_header->m_parent;
        while (cur) {
            if (!m_cmp.__lt__(cur->m_key, key)) {
                carry = cur;
                cur = cur->m_left;
            } else {
                cur = cur->m_right;
            }
        }
        return iterator(carry);
    }

    template<typename Key, typename Val, typename Cmp>
    typename RedBlackTree<Key, Val, Cmp>::iterator
    RedBlackTree<Key, Val, Cmp>::upper_bound(const key_type &key) const {
        node_type *carry = m_header;
        node_type *cur = m_header->m_parent;
        while (cur) {
            if (m_cmp.__lt__(key, cur->m_key)) {
                carry = cur;
                cur = cur->m_left;
            } else {
                cur = cur->m_right;
            }
        }

        return iterator(carry);
    }

    template<typename Key, typename Val, typename Cmp>
    inline Pair<
            typename RedBlackTree<Key, Val, Cmp>::iterator,
            typename RedBlackTree<Key, Val, Cmp>::iterator
    >
    RedBlackTree<Key, Val, Cmp>::equal_range(const key_type &key) const {
        return Pair<iterator, iterator>(lower_bound(key), upper_bound(key));
    }

}

#endif //EMBEDDEDCPLUSPLUS_REDBLACKTREE_H
