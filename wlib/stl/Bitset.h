/**
 * @file Bitset.h
 * @brief Bitset is a class that provides bits manipulation methods
 *
 * It provides setting, resetting, testing and flipping of bits
 *
 * @author Deep Dhillon
 * @author Jeff Niu
 * @date October 28, 2017
 * @bug No Known bugs
 */


#ifndef CORE_STL_BITSET_H
#define CORE_STL_BITSET_H

#include <math.h>
#include <string.h>

#include "wlib/Types.h"
#include "wlib/Wlib.h"
#include "wlib/strings/String.h"

namespace wlp {

    /**
     * Computes the mask corresponding to the number of bits
     * or the exponent in the from 2^n - 1.
     * @tparam exp the number of bits, 32 or less
     */
    template<uint8_t exp>
    struct pow_mask {
        static const uint32_t value = static_cast<uint32_t>((1 << exp) - 1);
    };

    /**
     * Template specialization for 16 bits to
     * prevent overflow.
     */
    template<>
    struct pow_mask<16> {
        static const uint32_t value = 0xffff;
    };

    /**
     * Template specialization for 32 bits to
     * prevent overflow.
     */
    template<>
    struct pow_mask<32> {
        static const uint32_t value = 0xffffffff;
    };

    /**
     * Compute the minimum number of integers
     * needed to store a certain number of bits.
     * @tparam nBits the bits to store
     */
    template<uint8_t nBits>
    struct ceil_bits {
        static const uint32_t value = static_cast<uint32_t>((nBits + INT32_SIZE - 1) / INT32_SIZE);
    };

    template<uint8_t nBits>
    struct next_byte {
        static constexpr uint16_t value = static_cast<const uint16_t>((nBits + BYTE_SIZE - 1) / BYTE_SIZE);
    };

    template<uint8_t nBits>
    class Bitset {
    public:
        /**
         * Default Constructor creates an empty bitset.
         */
        Bitset() {
            memset(m_array, 0, sizeof(m_array));
        }

        /**
         * Constructor creates a bitset from a number that
         * can be of max 64 bit in size.
         *
         * @param n the number to create bitset from
         */
        explicit Bitset(uint64_t n) {
            setFromNumber(n);
        }

        /**
         * Copy constructor for const.
         * @param b Bitset to copy
         */
        Bitset(const Bitset<nBits> &b) {
            uint32_t end = ceil_bits<nBits>::value;
            for (size_type i = 0; i < end; i++) {
                m_array[i] = (m_array[i] & 0) | b.m_array[i];
            }
        }

        /**
         * Set the value of the Bitset from a number
         * of maximum 64 bit size.
         * @param n the number to set from
         */
        void setFromNumber(uint64_t n) {
            memset(m_array, 0, sizeof(m_array));
            constexpr uint32_t end = static_cast<uint32_t>(nBits / INT32_SIZE);
            constexpr uint32_t extra = static_cast<uint32_t>(nBits - end * INT32_SIZE);
            for (size_type i = 0; i < end; ++i) {
                m_array[i] = static_cast<uint32_t>(n);
                n >>= INT32_SIZE;
            }
            if (extra) {
                m_array[end] = (static_cast<uint32_t>(n)) & pow_mask<extra>::value;
            }
        }


        /**
         * Sets the bit at @code index to be true.
         *
         * @param index the index of the bit
         */
        void set(uint16_t index) {
            m_array[index / INT32_SIZE] |= (1U << (index % INT32_SIZE));
        }

        /**
         * Sets the bit at @code index to be false.
         *
         * @param index the index of the bit
         */
        void reset(uint16_t index) {
            m_array[index / INT32_SIZE] &= ~(1U << (index % INT32_SIZE));
        }

        /**
         * Toggles the but at @code index.
         *
         * @param index the index of the bit
         */
        void flip(uint16_t index) {
            m_array[index / INT32_SIZE] ^= (1U << (index % INT32_SIZE));
        }

        /**
         * Returns the value of bit at @code index.
         *
         * @param index the index of the bit
         * @return the bit value
         */
        bool test(uint16_t index) const {
            return (m_array[index / INT32_SIZE] & (1U << (index % INT32_SIZE))) != 0;
        }

        /**
         * Converts the bits into 64 bit unsigned integer
         *
         * @return unsigned 64 bit integer
         */
        uint64_t to_uint64() const {
            if (nBits <= 32) {
                return to_uint32();
            }
            return (((uint64_t) m_array[1]) << INT32_SIZE) | ((uint32_t) m_array[0]);
        }

        /**
         * Converts the bits into 32 bit unsigned integer
         *
         * @return unsigned 32 bit integer
         */
        uint32_t to_uint32() const {
            return (uint32_t) m_array[0];
        }

        /**
         * Converts the bits into 16 bit unsigned integer
         *
         * @return unsigned 16 bit integer
         */
        uint16_t to_uint16() const {
            return (uint16_t) (m_array[0] & pow_mask<16>::value);
        }

        /**
         * Converts the bits into 8 bit unsigned integer
         *
         * @return unsigned 8 bit integer
         */
        uint8_t to_uint8() const {
            return (uint8_t) (m_array[0] & pow_mask<8>::value);
        }

        /**
         * Access operator returns the bit at the given position.
         * @param i the position of the bit to test
         * @return the value of the bit
         */
        bool operator[](const uint16_t i) const {
            return test(i);
        }

        /**
         * Assignment operator copies the contents of the bitset.
         * @param b Bitset to assign
         */
        Bitset<nBits> &operator=(const Bitset<nBits> &b) {
            uint32_t end = ceil_bits<nBits>::value;
            for (size_type i = 0; i < end; i++) {
                m_array[i] = (m_array[i] & 0) | b.m_array[i];
            }
            return *this;
        }

        /**
         * @return a reference to mutable elements of the bits
         */
        uint32_t *data() {
            return m_array;
        }

        /**
         * @return a reference to const elements of the bits
         */
        const uint32_t *data() const {
            return m_array;
        }

        StaticString<next_byte<nBits>::value> to_static_string() const {
            uint16_t num_bytes = static_cast<uint16_t>(ceil_bits<nBits>::value * INT32_SIZE / BYTE_SIZE);
            return {reinterpret_cast<const char *>(m_array), num_bytes};
        }

        DynamicString to_dynamic_string() const {
            uint16_t num_bytes = static_cast<uint16_t>(ceil_bits<nBits>::value * INT32_SIZE / BYTE_SIZE);
            return {reinterpret_cast<const char *>(m_array), num_bytes};
        }

    private:
        /**
         * Backing array of integers that contain the bites.
         * Integer type arrays generally have the fastest access
         * times in C++.
         */
        uint32_t m_array[ceil_bits<nBits>::value];
    };
}

#endif //CORE_STL_BITSET_H
