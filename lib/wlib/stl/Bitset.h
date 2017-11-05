/**
 * @file Bitset.h
 * @brief Bitset is a class that provides bits manipulation methods
 *
 * It provides setting, resetting, testing and flipping of bits
 *
 * @author Deep Dhillon
 * @date October 28, 2017
 * @bug No Known bugs
 */


#ifndef CORE_STL_BITSET_H
#define CORE_STL_BITSET_H

#include <string.h>
#include <stdint.h>
#include <math.h>

namespace wlp {
#define NO_OF_BITS (8 * sizeof(unsigned int))

	template<uint8_t bits>
	class Bitset {
	public:
		/**
		 * Default Constructor creates an empty bitset
		 */
		Bitset() {
			memset(m_array, 0, sizeof(m_array));
		}

		/**
		 * Constructor creates a bitset from a number that can be of max
		 * 64 bit in size
		 *
		 * @param number the number to create bitset from
		 */
		explicit Bitset(uint64_t number){
			memset(m_array, 0, sizeof(m_array));

			uint16_t size = sizeof(number) * 8;

			if (size > bits) size = bits;

			for (uint16_t index = 0; index < size; ++index) {
				uint8_t remainder = (uint8_t) (number % 2);
				number /= 2;

				if (remainder) set(index);
				else reset(index);
			}
		}


		/**
		 * Sets the bit at @code index to be true
		 *
		 * @param index the index of the bit
		 */
		void set(uint16_t index) {
			m_array[index / NO_OF_BITS] |= (1U << (index % NO_OF_BITS));
		}

		/**
		 * Sets the bit at @code index to be false
		 *
		 * @param index the index of the bit
		 */
		void reset(uint16_t index) {
			m_array[index / NO_OF_BITS] &= ~(1U << (index % NO_OF_BITS));
		}

		/**
		 * Toggles the but at @code index
		 *
		 * @param index the index of the bit
		 */
		void flip(uint16_t index) {
			m_array[index / NO_OF_BITS] ^= (1U << (index % NO_OF_BITS));
		}

		/**
		 * Returns the value of bit at @code index
		 *
		 * @param index the index of the bit
		 * @return the bit value
		 */
		bool test(uint16_t index) {
			return (m_array[index / NO_OF_BITS] & (1U << (index % NO_OF_BITS))) != 0;
		}

		/**
		 * Converts the bits into 64 bit unsigned integer
		 *
		 * @return unsigned 64 bit integer
		 */
		uint64_t to_uint64_t() {
			uint64_t number = 0;

			for (uint8_t i = 0; i < 64; ++i) {
				uint64_t powerVal = (uint64_t) ceil(pow(2, i));
				uint8_t bitVal = (uint8_t) test(i);
				number += bitVal * powerVal;
			}

			return number;
		}

		/**
		 * Converts the bits into 32 bit unsigned integer
		 *
		 * @return unsigned 32 bit integer
		 */
		uint32_t to_uint32_t() {
			uint32_t number = 0;

			for (uint8_t i = 0; i < 32; ++i) {
				uint32_t powerVal = (uint32_t) ceil(pow(2, i));
				uint8_t bitVal = (uint8_t) test(i);
				number += bitVal * powerVal;
			}

			return number;
		}

		/**
		 * Converts the bits into 16 bit unsigned integer
		 *
		 * @return unsigned 16 bit integer
		 */
		uint16_t to_uint16_t() {
			uint16_t number = 0;

			for (uint8_t i = 0; i < 16; ++i) {
				uint16_t powerVal = (uint16_t) ceil(pow(2, i));
				uint8_t bitVal = (uint8_t) test(i);
				number += bitVal * powerVal;
			}

			return number;
		}

		/**
		 * Converts the bits into 8 bit unsigned integer
		 *
		 * @return unsigned 8 bit integer
		 */
		uint8_t to_uint8_t() {
			uint8_t number = 0;

			for (uint8_t i = 0; i < 8; ++i) {
				uint8_t powerVal = (uint8_t) ceil(pow(2, i));
				uint8_t bitVal = (uint8_t) test(i);
				number += bitVal * powerVal;
			}

			return number;
		}

	private:
		int m_array[bits/ NO_OF_BITS];
	};
}

#endif //CORE_STL_BITSET_H
