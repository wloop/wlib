/**
 * @file RedBlackTree.h
 * @brief Red black tree implementation.
 *
 * Implements a red black tree structure for use in other
 * data structures using non-recursive methods.
 *
 * @author Jeff Niu
 * @date November 12, 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDCPLUSPLUS_REDBLACKTREE_H
#define EMBEDDEDCPLUSPLUS_REDBLACKTREE_H

#include "Comparator.h"
#include "Pair.h"

#include "wlib/Types.h"
#include "wlib/memory/Memory.h"
#include "wlib/utility/Utility.h"
#include "wlib/exceptions/Exceptions.h"

namespace wlp {

    /**
     * Tree node color, either red or black.
     */
    struct RedBlackTreeColor {
        /**
         * Color type.
         */
        typedef bool type;
        /**
         * Red node.
         */
        static constexpr type RED = false;
        /**
         * Black node.
         */
        static constexpr type BLACK = true;
    };

    /**
     * Tree node contains the node key and value.
     *
     * @tparam Element element type contained by the node, which must
     * provide the functions @code get_key() @endcode and @code get_val() @endcode.
     */
    template<typename Element>
    struct RedBlackTreeNode {
        typedef RedBlackTreeNode<Element> node_type;
        typedef Element element_type;

    private:
        typedef RedBlackTreeColor::type color;


    public:
        /**
         * The node color.
         */
        color m_color;

        /**
         * Node parent.
         */
        node_type *m_parent;
        /**
         * Left child node.
         */
        node_type *m_left;
        /**
         * Right child node.
         */
        node_type *m_right;

        /**
         * Element of the node, which contains the key
         * use to compare nodes and the value, if one
         * is mapped to by the key.
         */
        element_type m_element;

        /**
         * Obtain the minimum key node starting from the given node.
         *
         * @param node from which to find the minimum
         * @return pointer to the node with the smallest key
         */
        static node_type *find_minimum(node_type *node) {
            while (node->m_left) {
                node = node->m_left;
            }
            return node;
        }

        /**
         * Obtain the maximum key node starting from the given node.
         *
         * @param node from which to find the maximum
         * @return pointer to the node with the largest key
         */
        static node_type *find_maximum(node_type *node) {
            while (node->m_right) {
                node = node->m_right;
            }
            return node;
        }
    };

    /**
     * Tree iterator class, templated to enable constant and non-constant
     * derived types. This class should not be used directly.
     *
     * @tparam Element element type which contains the key and value
     * @tparam Key     the node key type
     * @tparam Ref     reference to value type, which may be a constant reference
     * @tparam Ptr     pointer to value type, which may be a constant pointer
     * @tparam GetVal  struct which returns the value of an element
     */
    template<typename Element, typename Val, typename Ref, typename Ptr, typename GetVal>
    struct RedBlackTreeIterator {
        typedef RedBlackTreeNode<Element> node_type;
        typedef RedBlackTreeIterator<Element, Val, Val &, Val *, GetVal> iterator;
        typedef RedBlackTreeIterator<Element, Val, const Val &, const Val *, GetVal> const_iterator;
        typedef RedBlackTreeColor color;
        typedef Ref reference;
        typedef Ptr pointer;
        typedef Element element_type;
        typedef GetVal get_value;

    private:
        typedef RedBlackTreeIterator<Element, Val, Ref, Ptr, GetVal> self_type;

    public:
        /**
         * The tree node pointed to by this iterator.
         */
        node_type *m_node;

        /**
         * Functor used to obtain element value.
         */
        get_value m_get_value{};

        /**
         * Constructor from node.
         *
         * @param node to point to
         */
        RedBlackTreeIterator(node_type *node)
                : m_node(node) {
        }

        /**
         * Constructor from constant reference.
         *
         * @param it iterator to copy
         */
        RedBlackTreeIterator(const self_type &it)
                : m_node(it.m_node) {
        }

        /**
         * Move the iterator to the next ordered node in the tree.
         */
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

        /**
         * Move the iterator to the previous ordered node in the tree.
         */
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

        /**
         * Iterator equality operator.
         *
         * @param it iterator to compare
         * @return true if they point to the same node
         */
        bool operator==(const self_type &it) const {
            return m_node == it.m_node;
        }

        /**
         * Iterator inequality operator.
         *
         * @param it iterator to compare
         * @return true if they point to different nodes
         */
        bool operator!=(const self_type &it) const {
            return m_node != it.m_node;
        }

        /**
         * Deference operator.
         *
         * @return reference to the value of the node
         * pointed to by the iterator
         */
        reference operator*() const {
            if (m_node == nullptr) {
                THROW(KEY_EXCEPTION("Accessing invalid iterator"))
            }
            return m_get_value(m_node->m_element);
        }

        /**
         * Pointer operator.
         *
         * @return pointer to the value of the node
         * pointer to by the iterator
         */
        pointer operator->() const {
            if (m_node == nullptr) {
                return nullptr;
            }
            return &m_get_value(m_node->m_element);
        }

        /**
         * Prefix increment operator moves the iterator to the next
         * ordered node in the tree, or pass-the-end if the
         * iterator has reached the rightmost end of the tree.
         *
         * @return reference to this iterator
         */
        self_type &operator++() {
            increment();
            return *this;
        }

        /**
         * Postfix increment operator moves the iterator to the next
         * ordered node in the tree, or pass-the-end if the
         * iterator has reached the rightmost end of the tree.
         *
         * @return a copy of the iterator before the increment
         */
        self_type operator++(int) {
            iterator tmp = *this;
            increment();
            return tmp;
        }

        /**
         * Prefix decrement operator moves the iterator to the previous
         * ordered node in the tree. Iterator does not move past the begin.
         *
         * @return reference to this iterator
         */
        self_type &operator--() {
            decrement();
            return *this;
        }

        /**
         * Postfix decrement operator moves the iterator to the previous
         * ordered node in the tree. Iterator does not move past the begin.
         *
         * @return a copy of the iterator before the increment
         */
        self_type operator--(int) {
            iterator tmp = *this;
            decrement();
            return tmp;
        }

        /**
         * Copy assignment operator.
         *
         * @param it iterator to assign
         * @return reference to this iterator
         */
        self_type &operator=(const self_type &it) {
            m_node = it.m_node;
            return *this;
        }

    };

    /**
     * Red black tree implementation, designed for use in associative
     * container implementation, e.g. @code Map @endcode and @code Set @endcode.
     * The insertion and deletion operations are based on those in CLRS.
     *
     * The provided @code Element @endcode type must provide functions
     * @code get_key() @endcode which returns a type of @code Key @endcode
     * and a function @code get_val() @endcode which returns a type of @code Val @endcode.
     * These details are dealt by implementing data structures such as @code Map @endcode
     * and @code Set @endcode.
     *
     * @tparam Element element type which contains the key and val
     * @tparam Key     node key type
     * @tparam Val     node value type
     * @tparam Cmp     key comparator type, which uses the default comparator
     * @tparam GetKey  functor type used to get element key
     * @tparam GetVal  functor type used to get element value
     */
    template<typename Element,
            typename Key,
            typename Val,
            typename GetKey,
            typename GetVal,
            typename Cmp = Comparator <Key>>
    class RedBlackTree {
    public:
        typedef Key key_type;
        typedef Val val_type;
        typedef Cmp comparator;
        typedef wlp::size_type size_type;
        typedef RedBlackTreeNode<Element> node_type;
        typedef RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp> tree_type;
        typedef RedBlackTreeIterator<Element, Val, Val &, Val *, GetVal> iterator;
        typedef RedBlackTreeIterator<Element, Val, const Val &, const Val *, GetVal> const_iterator;
        typedef GetKey get_key;
        typedef GetVal get_val;

    protected:
        typedef RedBlackTreeColor color;

        /**
         * Header node, which maintains reference to the leftmost node,
         * the rightmost node, and the root node.
         */
        node_type *m_header;
        /**
         * The number of nodes currently in the tree.
         */
        size_type m_size;
        /**
         * Class comparator instance.
         */
        comparator m_cmp;
        /**
         * Functor used to obtain element key.
         */
        get_key m_get_key{};

        /**
         * Allocate a new node.
         *
         * @return pointer to the new node
         */
        node_type *create_node() {
            return malloc<node_type>();
        }

        /**
         * Copy the contents of a node except for its paremt.
         *
         * @param node the tree node to copy
         * @return a copy of the node
         */
        node_type *copy_node(node_type *node) {
            node_type *copy = create_node();
            copy->m_element = node->m_element;
            copy->m_color = node->m_color;
            copy->m_left = node->m_left;
            copy->m_right = node->m_right;
            return copy;
        }

        /**
         * Deallocate the given node.
         *
         * @param node node to deallocate
         */
        void destroy_node(node_type *node) {
            free<node_type>(node);
        }

        /**
         * Perform red-black tree left rotation of the specified
         * node about the specified root.
         *
         * @param node the node to rotate
         * @param root the rotate root node
         */
        void rotateLeft(node_type *node, node_type *&root);

        /**
         * Perform red-black tree right rotation of the specified
         * node about the specified root.
         *
         * @param node the node to rotate
         * @param root the rotate root node
         */
        void rotateRight(node_type *node, node_type *&root);

        /**
         * Perform red-black tree rebalance of a potentially
         * erroneous node starting from the given root.
         *
         * @param node the node to rebalance
         * @param root the rebalance root node
         */
        void rebalance(node_type *node, node_type *&root);

        /**
         * Rebalance for erasure the given node. The node is eliminated
         * from the tree during the rebalance and is prepared for deletion.
         *
         * @param node      the node to delete
         * @param root      the root node in the tree
         * @param leftmost  the leftmost node in the tree
         * @param rightmost the right most node in the tree
         * @return a pointer to the node that can be deleted
         */
        node_type *erase_rebalance(
                node_type *node,
                node_type *&root,
                node_type *&leftmost,
                node_type *&rightmost
        );

        /**
         * Insert a given node at the pivot position, which will become
         * the parent node of the inserted node.
         *
         * @param node the node to insert
         * @param piv  the insertion pivot node
         * @param key  the key to insert
         * @param val  the value to insert
         * @return iterator to the inserted node
         */
        template<typename E>
        iterator insert(node_type *node, node_type *piv, E &&element);

        /**
         * Delete the supplied node from the tree and all
         * nodes beneath it.
         *
         * @param root the node to delete
         */
        void erase(node_type *root);

        /**
         * Initialize the tree as empty, where the header
         * node children are itself and the parent is null.
         */
        void empty_initialize() {
            m_header->m_color = color::RED;
            m_header->m_parent = nullptr;
            m_header->m_left = m_header;
            m_header->m_right = m_header;
        }

    public:
        /**
         * Create an empty red black tree.
         *
         * @param n reserved space for nodes
         */
        explicit RedBlackTree()
                : m_header(nullptr),
                  m_size(0),
                  m_cmp() {
            m_header = create_node();
            empty_initialize();
        }

        /**
         * Disable copy construction.
         */
        RedBlackTree(const tree_type &) = delete;

        /**
         * Move constructor.
         *
         * @param tree tree to move
         */
        RedBlackTree(tree_type &&tree)
                : m_header(move(tree.m_header)),
                  m_size(move(tree.m_size)),
                  m_cmp() {
            tree.m_header = nullptr;
            tree.m_size = 0;
        }

        /**
         * Destructor.
         */
        ~RedBlackTree() {
            clear();
            if (m_header) {
                destroy_node(m_header);
            }
        }

        /**
         * @return an iterator to the leftmost node in the tree
         */
        iterator begin() {
            return iterator(m_header->m_left);
        }

        /**
         * @return a const iterator to the leftmost node in the tree
         */
        const_iterator begin() const {
            return const_iterator(m_header->m_left);
        }

        /**
         * @return an iterator to the header node, which is the
         * pass-the-end node after the rightmost node
         */
        iterator end() {
            return iterator(m_header);
        }

        /**
         * @return a const iterator to the header node, which is the
         * pass-the-end node after the rightmost node
         */
        const_iterator end() const {
            return const_iterator(m_header);
        }

        /**
         * @return whether the tree has no nodes
         */
        bool empty() const {
            return m_size == 0;
        }

        /**
         * @return the number of nodes in the tree
         */
        size_type size() const {
            return m_size;
        }

        /**
         * The maximum number of nodes that can be held in the
         * tree. The tree is not intrinsically limited by any limit
         * except that its size cannot exceed the defined size type.
         *
         * @return the maximum value of size_type
         */
        size_type capacity() const {
            return static_cast<size_type>(-1);
        }

        /**
         * Delete all the nodes in the tree such that it is now empty.
         */
        void clear() noexcept {
            if (m_size > 0) {
                erase(m_header->m_parent);
                m_header->m_parent = nullptr;
                m_header->m_left = m_header;
                m_header->m_right = m_header;
                m_size = 0;
            }
        }

        /**
         * Insert a value with a given key into the tree. This function
         * will insert the value if its key does not exist and fail
         * if the key already exists.
         *
         * @param key key to insert
         * @param val value to insert
         * @return a pair consisting of an iterator to the inserted node
         * or the node that prevented insertion and a flag indicating
         * whether insertion occurred
         */
        template<typename E>
        Pair<iterator, bool> insert_unique(E &&element);

        /**
         * Insert a value with a given key into the tree. This function
         * will always insert the value, allowing duplicate keys.
         *
         * @param key key to insert
         * @param val value to insert
         * @return iterator to the inserted node
         */
        template<typename E>
        iterator insert_equal(E &&element);

        /**
         * Delete the node pointed to by the provided iterator.
         *
         * @param pos iterator whose node to delete
         */
        void erase(const iterator &pos);

        /**
         * Erase all nodes in the tree that have the provided key.
         * For containers not allowing multiple keys, the returned value
         * is never greater than 1.
         *
         * @param key the key all of whose associated nodes are to be deleted
         * @return the number of deleted nodes
         */
        size_type erase(const key_type &key);

        /**
         * Erase all nodes starting from the first iterator to before the
         * last iterator, such that the nodes @code [first, last) @endcode
         * are deleted and the size of the tree is reduced by
         * @code last - first @endcode.
         *
         * @param first iterator to the first nodes to delete
         * @param last  iterator to the last nodes to delete
         * @return the number of deleted nodes
         */
        size_type erase(const iterator &first, const iterator &last);

        /**
         * Obtain an iterator to the first node in the tree
         * whose key matches the provided key. Returns pass-the-end
         * if no node in the tree has the provided key.
         *
         * @param key the key for which to obtain the first node
         * @return iterator to the first node with the key or pass-the-end
         */
        iterator find(const key_type &key);

        /**
         * Obtain a const iterator to the first node in the tree
         * whose key matches the provided key. Returns pass-the-end
         * if no node in the tree has the provided key.
         *
         * @param key the key for which to obtain the first node
         * @return const iterator to the first node with the key or pass-the-end
         */
        const_iterator find(const key_type &key) const;

        /**
         * Obtain the number of nodes in the tree that have
         * the provided key as their keys.
         *
         * @param key the key whose nodes to count
         * @return the number of nodes with the key
         */
        size_type count(const key_type &key) const;

        /**
         * Obtain an iterator to the first node in the tree by natural
         * order that has the provided key. Returns pass-the-end if
         * there are no nodes with the provided key.
         *
         * @pre The returned value is such that all nodes with the provided
         *      key are in the range @code [lower_bound(key), upper_bound(key)) @endcode.
         *
         * @param key the key whose first node to find
         * @return iterator to the first node with the key
         */
        iterator lower_bound(const key_type &key);

        /**
         * Obtain an iterator to the node right after the last node in the
         * tree by natural order that has the provided key. May return pass-the-end
         * if the key is the largest key in the tree, or if the key is not found.
         *
         * @pre The returned value is such that all nodes with the provided
         *      key are in the range @code [lower_bound(key), upper_bound(key)) @endcode.
         *
         * @param key the key whose last node to find
         * @return iterator to the node right after the last node
         */
        iterator upper_bound(const key_type &key);

        /**
         * Obtain a const iterator to the first node in the tree by natural
         * order that has the provided key. Returns pass-the-end if
         * there are no nodes with the provided key.
         *
         * @pre The returned value is such that all nodes with the provided
         *      key are in the range @code [lower_bound(key), upper_bound(key)) @endcode.
         *
         * @param key the key whose first node to find
         * @return const iterator to the first node with the key
         */
        const_iterator lower_bound(const key_type &key) const;

        /**
         * Obtain a const iterator to the node right after the last node in the
         * tree by natural order that has the provided key. May return pass-the-end
         * if the key is the largest key in the tree, or if the key is not found.
         *
         * @pre The returned value is such that all nodes with the provided
         *      key are in the range @code [lower_bound(key), upper_bound(key)) @endcode.
         *
         * @param key the key whose last node to find
         * @return const iterator to the node right after the last node
         */
        const_iterator upper_bound(const key_type &key) const;

        /**
         * Obtain the range of nodes whose keys are equal to the provided key
         * as per @code lower_bound @endcode and @code upper_bound @endcode.
         *
         * @param key the key whose range to get
         * @return a pair of iterators to the lower and upper bounds
         */
        Pair <iterator, iterator> equal_range(const key_type &key);

        /**
         * Obtain the range of nodes whose keys are equal to the provided key
         * as per @code lower_bound @endcode and @code upper_bound @endcode.
         *
         * @param key the key whose range to get
         * @return a pair of const iterators to the lower and upper bounds
         */
        Pair <const_iterator, const_iterator> equal_range(const key_type &val) const;

        /**
         * Disable copy assignemnt.
         *
         * @return reference to this tree
         */
        tree_type &operator=(const tree_type &) = delete;

        /**
         * Move assignment operator. Resources in this tree
         * are freed and replaced by moving those from the
         * assigned tree. The assigned tree is prepared for
         * destruction.
         *
         * @param tree the tree to move
         * @return reference to this tree
         */
        tree_type &operator=(tree_type &&tree) {
            clear();
            destroy_node(m_header);
            m_size = move(tree.m_size);
            m_header = move(tree.m_header);
            tree.m_size = 0;
            tree.m_header = 0;
            return *this;
        }

    };

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal, typename Cmp>
    inline void RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::rotateLeft(node_type *node, node_type *&root) {
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

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal, typename Cmp>
    inline void RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::rotateRight(node_type *node, node_type *&root) {
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

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal, typename Cmp>
    inline void RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::rebalance(node_type *node, node_type *&root) {
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

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal, typename Cmp>
    inline typename RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>::node_type *
    RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::erase_rebalance(
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

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal, typename Cmp>
    template<typename E>
    typename RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>::iterator
    RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::insert(node_type *cur, node_type *carry, E &&element) {
        node_type *node = create_node();
        node->m_element = forward<E>(element);
        if (carry == m_header || cur || m_cmp.__lt__(m_get_key(element), m_get_key(carry->m_element))) {
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

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal, typename Cmp>
    template<typename E>
    Pair<typename RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>::iterator, bool>
    RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::insert_unique(E &&element) {
        node_type *carry = m_header;
        node_type *cur = m_header->m_parent;
        bool compare = true;
        while (cur) {
            carry = cur;
            compare = m_cmp.__lt__(m_get_key(element), m_get_key(cur->m_element));
            cur = compare ? cur->m_left : cur->m_right;
        }
        iterator tmp = iterator(carry);
        if (compare) {
            if (tmp == begin()) {
                return Pair<iterator, bool>(insert(cur, carry, forward<E>(element)), true);
            } else {
                --tmp;
            }
        }
        if (m_cmp.__lt__(m_get_key(tmp.m_node->m_element), m_get_key(element))) {
            return Pair<iterator, bool>(insert(cur, carry, forward<E>(element)), true);
        }
        return Pair<iterator, bool>(tmp, false);
    }

    template<typename Element, typename Key, typename Val, 
            typename GetKey, typename GetVal, typename Cmp>
    template<typename E>
    typename RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>::iterator
    RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::insert_equal(E &&element) {
        node_type *carry = m_header;
        node_type *cur = m_header->m_parent;
        while (cur) {
            carry = cur;
            cur = m_cmp.__lt__(m_get_key(element), m_get_key(cur->m_element)) ? cur->m_left : cur->m_right;
        }
        return insert(cur, carry, forward<E>(element));
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal, typename Cmp>
    inline void RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::erase(node_type *root) {
        node_type *current;
        node_type *pre;
        node_type *tmp;
        if (!root) {
            return;
        }
        current = root;
        while (current) {
            if (!current->m_left) {
                tmp = current;
                current = current->m_right;
                destroy_node(tmp);
            } else {
                pre = current->m_left;
                while (pre->m_right && pre->m_right != current) {
                    pre = pre->m_right;
                }
                if (!pre->m_right) {
                    pre->m_right = current;
                    current = current->m_left;
                } else {
                    pre->m_right = nullptr;
                    tmp = current;
                    current = current->m_right;
                    destroy_node(tmp);
                }
            }
        }
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal, typename Cmp>
    inline void RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::erase(const iterator &pos) {
        node_type *carry = erase_rebalance(pos.m_node, m_header->m_parent, m_header->m_left, m_header->m_right);
        destroy_node(carry);
        --m_size;
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal, typename Cmp>
    inline typename RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>::size_type
    RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::erase(const key_type &cur) {
        Pair<iterator, iterator> res = equal_range(cur);
        return erase(res.m_first, res.m_second);
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal, typename Cmp>
    inline size_type
    RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::erase(const iterator &first, const iterator &last) {
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

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal, typename Cmp>
    typename RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>::iterator
    RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::find(const key_type &key) {
        node_type *carry = m_header;
        node_type *cur = m_header->m_parent;
        while (cur) {
            if (!m_cmp.__lt__(m_get_key(cur->m_element), key)) {
                carry = cur;
                cur = cur->m_left;
            } else {
                cur = cur->m_right;
            }
        }
        iterator tmp = iterator(carry);
        return (tmp == end() || m_cmp.__lt__(key, m_get_key(tmp.m_node->m_element))) ? end() : tmp;
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal, typename Cmp>
    typename RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>::const_iterator
    RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::find(const key_type &key) const {
        node_type *carry = m_header;
        node_type *cur = m_header->m_parent;
        while (cur) {
            if (!m_cmp.__lt__(m_get_key(cur->m_element), key)) {
                carry = cur;
                cur = cur->m_left;
            } else {
                cur = cur->m_right;
            }
        }
        const_iterator tmp = const_iterator(carry);
        return (tmp == end() || m_cmp.__lt__(key, m_get_key(tmp.m_node->m_element))) ? end() : tmp;
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal, typename Cmp>
    typename RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>::size_type
    RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::count(const key_type &key) const {
        Pair<const_iterator, const_iterator> res = equal_range(key);
        size_type count = 0;
        while (res.m_first != res.m_second) {
            ++res.m_first;
            ++count;
        }
        return count;
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal, typename Cmp>
    typename RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>::iterator
    RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::lower_bound(const key_type &key) {
        node_type *carry = m_header;
        node_type *cur = m_header->m_parent;
        while (cur) {
            if (!m_cmp.__lt__(m_get_key(cur->m_element), key)) {
                carry = cur;
                cur = cur->m_left;
            } else {
                cur = cur->m_right;
            }
        }
        return iterator(carry);
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal, typename Cmp>
    typename RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>::iterator
    RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::upper_bound(const key_type &key) {
        node_type *carry = m_header;
        node_type *cur = m_header->m_parent;
        while (cur) {
            if (m_cmp.__lt__(key, m_get_key(cur->m_element))) {
                carry = cur;
                cur = cur->m_left;
            } else {
                cur = cur->m_right;
            }
        }

        return iterator(carry);
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal, typename Cmp>
    typename RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>::const_iterator
    RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::lower_bound(const key_type &key) const {
        node_type *carry = m_header;
        node_type *cur = m_header->m_parent;
        while (cur) {
            if (!m_cmp.__lt__(m_get_key(cur->m_element), key)) {
                carry = cur;
                cur = cur->m_left;
            } else {
                cur = cur->m_right;
            }
        }
        return const_iterator(carry);
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal, typename Cmp>
    typename RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>::const_iterator
    RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::upper_bound(const key_type &key) const {
        node_type *carry = m_header;
        node_type *cur = m_header->m_parent;
        while (cur) {
            if (m_cmp.__lt__(key, m_get_key(cur->m_element))) {
                carry = cur;
                cur = cur->m_left;
            } else {
                cur = cur->m_right;
            }
        }

        return const_iterator(carry);
    }

    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal, typename Cmp>
    inline Pair<
            typename RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>::iterator,
            typename RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>::iterator
    >
    RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::equal_range(const key_type &key) {
        return Pair<iterator, iterator>(lower_bound(key), upper_bound(key));
    }


    template<typename Element, typename Key, typename Val,
            typename GetKey, typename GetVal, typename Cmp>
    inline Pair<
            typename RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>::const_iterator,
            typename RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>::const_iterator
    >
    RedBlackTree<Element, Key, Val, GetKey, GetVal, Cmp>
    ::equal_range(const key_type &key) const {
        return Pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key));
    }

}

#endif //EMBEDDEDCPLUSPLUS_REDBLACKTREE_H
