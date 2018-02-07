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

#ifndef EMBEDDEDTESTS_PAIR_H
#define EMBEDDEDTESTS_PAIR_H

#include "wlib/utility/Utility.h"

namespace wlp {

    template<class First, class Second>
    struct Pair {
        typedef First first_type;
        typedef Second second_type;
        typedef Pair<First, Second> pair;

        /**
         * First element in the pair.
         */
        first_type m_first;
        /**
         * Second element in the pair.
         */
        second_type m_second;

        /**
         * Default constructor creates an empty pair.
         */
        Pair() {};

        /**
         * Copy constructor for const reference.
         *
         * @param pair pair to copy
         */
        Pair(pair const &pair)
                : m_first(pair.m_first),
                  m_second(pair.m_second) {
        }

        /**
         * Copy constructor for R-value.
         *
         * @param pair temporary pair to copy
         */
        Pair(pair &&pair)
                : m_first(move(pair.m_first)),
                  m_second(move(pair.m_second)) {
        }

        /**
         * Create a pair from two values of first type and second type.
         *
         * @param first  first type value
         * @param second second type value
         */
        template<typename FirstType, typename SecondType>
        Pair(FirstType &&first, SecondType &&second)
                : m_first(forward<FirstType>(first)),
                  m_second(forward<SecondType>(second)) {
        }

        /**
         * @return the first value in the pair
         */
        first_type &first() {
            return m_first;
        }

        /**
         * @return the second value in the pair
         */
        second_type &second() {
            return m_second;
        }

        /**
         * @return unmodifiable first value in the pair
         */
        first_type const &first() const {
            return m_first;
        }

        /**
         * @return unmodifiable second value in the pair
         */
        second_type const &second() const {
            return m_second;
        }

        /**
         * Assignment operator copies the first and second values.
         *
         * @param p pair to copy
         * @return a reference to this pair
         */
        template<typename P>
        pair &operator=(P &&p) {
            m_first = forward<first_type>(p.m_first);
            m_second = forward<second_type>(p.m_second);
            return *this;
        }

        /**
         * Equality operator. Two pairs are equal if
         * both their elements are equal.
         *
         * @param p the pair to compare
         * @return true if the pairs are equal
         */
        bool operator==(const pair &p) {
            return m_first == p.m_first && m_second == p.m_second;
        }

        /**
         * Inequality operator. Two pairs are unequal if
         * one or more of the two elements are unequal.
         *
         * @param p the pair to compare
         * @return true if the pairs are unequal
         */
        bool operator!=(const pair &p) {
            return m_first != p.m_first || m_second != p.m_second;
        }

    };

}

#endif //EMBEDDEDTESTS_PAIR_H
