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

	private:
		/**
		 * First element in the pair.
		 */
		first_type m_first;
		/**
		 * Second element in the pair.
		 */
		second_type m_second;

	public:
		/**
		 * Default constructor creates an empty pair.
		 */
		Pair() {};
		/**
		 * Create a pair from two values of first type and second type.
		 * @param first  first type value
		 * @param second second type value
		 */
		Pair(first_type first, second_type second) : m_first(first), m_second(second) {}

		/**
		 * @return the first value in the pair
		 */
		first_type first() {
			return m_first;
		}
		/**
		 * @return the second value in the pair
		 */
		second_type second() {
			return m_second;
		}

		/**
		 * Assignment operator copies the first and second values.
		 * @param p pair to copy
		 * @return a reference to this pair
		 */
		pair& operator=(const pair& p) {
			m_first = p.m_first;
			m_second = p.m_second;
			return *this;
		}

		/**
		 * Assignment operator copies the first and second values.
		 * @param p pair to copy
		 * @return a reference to this pair
		 */
		pair& operator=(pair& p) {
			m_first = p.m_first;
			m_second = p.m_second;
			return *this;
		}

		/**
		 * Equality operator. Two pairs are equal if
		 * both their elements are equal.
		 * @param p the pair to compare
		 * @return true if the pairs are equal
		 */
		bool operator==(const pair& p) {
			return m_first == p.m_first && m_second == p.m_second;
		}

		/**
		 * Equality operator. Two pairs are equal if
		 * both their elements are equal.
		 * @param p the pair to compare
		 * @return true if the pairs are equal
		 */
		bool operator==(pair& p) {
			return m_first == p.m_first && m_second == p.m_second;
		}

	};

}

#endif //EMBEDDEDTESTS_PAIR_H
