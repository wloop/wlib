#ifndef EMBEDDEDCPLUSPLUS_OPENSET_H
#define EMBEDDEDCPLUSPLUS_OPENSET_H

#include "Hash.h"
#include "Equal.h"
#include "OpenMap.h"

namespace wlp {

    template<class Key,
            class Hash = hash<Key, uint16_t>,
            class Equal = equals<Key>>
    class OpenHashSet {
    public:
        typedef OpenHashSet<Key, Hash, Equal> hash_set;
        typedef OpenHashMap<Key, Key, Hash, Equal> hash_map;
        typedef OpenHashMapIterator<Key, Key, Hash, Equal> iterator;
        typedef OpenHashMapConstIterator<Key, Key, Hash, Equal> const_iterator;
        typedef hash_map::size_type size_type;
        typedef hash_map::percent_type percent_type;
        typedef hash_map::key_type key_type;

    private:
        hash_map m_hash_map;

    public:
        explicit OpenHashSet(
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


#endif //EMBEDDEDCPLUSPLUS_OPENSET_H
