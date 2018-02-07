#ifndef EMBEDDEDCPLUSPLUS_STRINGITERATOR_H
#define EMBEDDEDCPLUSPLUS_STRINGITERATOR_H

#include "wlib/Types.h"
#include "wlib/stl/Concept.h"

namespace wlp {

    template<typename String, typename Ref, typename Ptr/*, typename = typename enable_if<is_string<String>()>::type*/>
    class StringIterator {
    public:
        typedef char val_type;
        typedef wlp::size_type size_type;
        typedef wlp::diff_type diff_type;
        typedef String string_type;
        typedef Ref reference;
        typedef Ptr pointer;

    private:
        typedef StringIterator<String, Ref, Ptr> self_type;

        size_type m_i;
        string_type *m_string;

        void check_bounds() {
            if (m_i > m_string->length()) {
                m_i = m_string->length();
            }
        }

    public:
        StringIterator(size_type i, string_type *string)
                : m_i(i),
                  m_string(string) {
            check_bounds();
        }

        StringIterator(const self_type &it)
                : m_i(it.m_i),
                  m_string(it.m_string) {
            check_bounds();
        }

        reference operator*() const {
            return m_string->at(m_i);
        }

        pointer operator->() const {
            return m_string->c_str() + m_i;
        }

        self_type &operator++() {
            if (m_i < m_string->length()) {
                ++m_i;
            }
            return *this;
        }

        self_type operator++(int) {
            self_type it = *this;
            ++*this;
            return it;
        }

        self_type &operator--() {
            if (m_i > 0) {
                --m_i;
            }
            return *this;
        }

        self_type operator--(int) {
            self_type it = *this;
            --*this;
            return it;
        }

        bool operator==(const self_type &it) const {
            return m_i == it.m_i;
        }

        bool operator!=(const self_type &it) const {
            return m_i != it.m_i;
        }

        self_type &operator=(const self_type &it) {
            m_i = it.m_i;
            return *this;
        }

        self_type operator+(const size_type &d) const {
            return self_type(static_cast<size_type>(m_i + d), m_string);
        }

        self_type operator-(const size_type &d) const {
            return self_type(static_cast<size_type>(m_i - d), m_string);
        }

        diff_type operator-(const self_type &it) const {
            return static_cast<diff_type>(m_i - it.m_i);
        }

        self_type &operator+=(const size_type &d) {
            m_i = static_cast<size_type>(m_i + d);
            check_bounds();
            return *this;
        }

        self_type &operator-=(const size_type &d) {
            if (d >= m_i) {
                m_i = 0;
            } else {
                m_i = static_cast<size_type>(m_i - d);
            }
            return *this;
        }
    };

}

#endif //EMBEDDEDCPLUSPLUS_STRINGITERATOR_H
