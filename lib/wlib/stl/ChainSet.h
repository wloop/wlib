#ifndef EMBEDDEDCPLUSPLUS_CHAINSET_H
#define EMBEDDEDCPLUSPLUS_CHAINSET_H

#include "Hash.h"
#include "Equal.h"
#include "ChainMap.h"

namespace wlp {

    template<class Key,
            class Hash = hash<Key, uint16_t>,
            class Equal = equals<Key>>
    class ChainHashSet {
    public:
        typedef ChainHashSet<Key, Hash, Equal> hash_set;
        typedef ChainHashMap<Key, Key, Hash, Equal> hash_map;
        typedef ChainHashMapIterator<Key, Key, Hash, Equal> iterator;
        typedef ChainHashMapConstIterator<Key, Key, Hash, Equal> const_iterator;
        typedef hash_map::size_type size_type;
        typedef hash_map::percent_type percent_type;
        typedef hash_map::key_type key_type;

    private:
        hash_map m_hash_map;

    public:
        explicit ChainHashSet(
                size_type n = 12,
                percent_type max_load = 75) :
                m_hash_map(n, max_load) {}

        size_type size() const {
            return m_hash_map.size();
        }

        size_type max_size() const {
            return m_hash_map.max_size();
        }

        percent_type max_load() const {
            return m_hash_map.max_load();
        }

        bool empty() const {
            return m_hash_map.empty();
        }

        const Allocator* get_node_allocator() const {
            return m_hash_map.get_node_allocator();
        }

        const hash_map* get_backing_hash_map() const {
            return &m_hash_map;
        }

        iterator begin() {
            return m_hash_map.begin();
        }

        iterator end() {
            return m_hash_map.end();
        }

        const_iterator begin() const {
            return m_hash_map.begin();
        }

        const_iterator end() const {
            return m_hash_map.end();
        }

        void clear() noexcept {
            m_hash_map.clear();
        }

        Pair<iterator, bool> insert(key_type key) {
            return m_hash_map.insert(key, key);
        };

        iterator erase(iterator& pos) {
            return m_hash_map.erase(pos);
        }

        const_iterator erase(const_iterator& pos) {
            return m_hash_map.erase(pos);
        }

        bool erase(key_type& key) {
            return m_hash_map.erase(key);
        }

        bool contains(const key_type& key) const {
            return m_hash_map.contains(key);
        }

        iterator find(const key_type& key) {
            return m_hash_map.find(key);
        }

        const_iterator find(const key_type& key) const {
            return m_hash_map.find(key);
        }
    };

}

#endif //EMBEDDEDCPLUSPLUS_CHAINSET_H
