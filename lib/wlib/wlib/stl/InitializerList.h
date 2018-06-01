#ifndef EMBEDDEDCPLUSPLUS_INITIALIZERLIST_H
#define EMBEDDEDCPLUSPLUS_INITIALIZERLIST_H

#include <wlib/Types.h>

namespace wlp {

    template<typename val_t>
    class initializer_list {
    public:
        typedef val_t value_type;
        typedef const val_t &reference;
        typedef const val_t &const_reference;
        typedef wlp::size_type size_type;
        typedef const val_t *iterator;
        typedef const val_t *const_iterator;

    private:
        iterator m_arr;
        size_type m_len;

        constexpr initializer_list(const_iterator arr, size_type len)
            : m_arr(arr), m_len(len) {}

    public:
        constexpr initializer_list() noexcept
            : m_arr(0), m_len(0) {}

        constexpr size_type
        size() const noexcept {
            return m_len;
        }

        constexpr const_iterator
        begin() const noexcept {
            return m_arr;
        }

        constexpr const_iterator
        end() const noexcept {
            return begin() + size();
        }
    };

    template<class Val>
    constexpr const Val *
    begin(initializer_list<Val> il) noexcept {
        return il.begin();
    }

    template<class _Tp>
    constexpr const _Tp *
    end(initializer_list<_Tp> il) noexcept {
        return il.end();
    }

}

#endif //EMBEDDEDCPLUSPLUS_INITIALIZERLIST_H
