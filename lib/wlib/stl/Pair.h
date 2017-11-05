/**
 * @file Pair.h
 * @brief Implementation of a 2-element tuple.
 *
 * Created in the spirit of std::pair
 *
 * @author Jeff Niu
 * @date November 2, 2017
 * @bug No known bugs
 */

/*
 * TODO: create and define more boolean operators
 */

#ifndef EMBEDDEDTESTS_PAIR_H
#define EMBEDDEDTESTS_PAIR_H

namespace wlp {

    template<class First, class Second>
    struct Pair {
        typedef First first_type;
        typedef Second second_type;
        typedef Pair<First, Second> pair;

        first_type m_first;
        second_type m_second;

        Pair() {};
        Pair(first_type first, second_type second) : m_first(first), m_second(second) {}

        first_type first() {
            return m_first;
        }
        second_type second() {
            return m_second;
        }

        pair& operator=(pair const& p) {
            m_first = p.m_first;
            m_second = p.m_second;
            return *this;
        }

        bool operator==(pair const& p) {
            return m_first == p.m_first && m_second == p.m_second;
        }

    };

}

#endif //EMBEDDEDTESTS_PAIR_H
