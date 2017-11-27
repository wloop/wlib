/**
 * @file StaticString.h
 * @brief StaticString provides a String that is on stack and is of fixed size
 *
 * @author Aditya Arora
 * @author Deep Dhillon
 * @date November 11, 2017
 * @bug No known bugs
 */

#ifndef WLIB_STATICSTRING_H
#define WLIB_STATICSTRING_H

#include <string.h> // strlen, strncpy, strcmp

#include "StringIterator.h"

#include "../Types.h"
#include "../utility/Math.h"

namespace wlp {

    template<size_type tSize>
    class StaticString {
    public:
        // Iterator types
        typedef StringIterator<StaticString<tSize>, char &, char *> iterator;
        typedef StringIterator<const StaticString<tSize>, const char &, const char *> const_iterator;

        // Required types for concept check
        typedef wlp::size_type size_type;
        typedef wlp::diff_type diff_type;

        /**
         * Default constructor creates string with no character
         */
        StaticString<tSize>() {
            clear();
        }

        /**
         * Constructor creates string using static string object
         *
         * @param str @code StaticString @endcode object
         */
        StaticString<tSize>(const StaticString<tSize> &str)
                : StaticString{str.c_str()} {}

        /**
         * Constructor creates string using character array
         *
         * @param str char string
         */
        explicit StaticString<tSize>(const char *str) {
            m_len = MIN(static_cast<size_type>(strlen(str)), tSize);
            memcpy(m_buffer, str, m_len + 1);
            m_buffer[m_len] = '\0';
        }

        /**
         * Assign operator assigns current object to given object
         *
         * @param str @code StaticString @endcode object
         * @return current object
         */
        StaticString<tSize> &operator=(const StaticString<tSize> &str) {
            m_len = str.m_len;
            memcpy(m_buffer, str.m_buffer, m_len + 1);
            return *this;
        }

        /**
         * The move assignment operator for a StaticString
         * must do a copy of the static array. This function
         * exists for string concept.
         *
         * @param str
         * @return
         */
        StaticString<tSize> &operator=(StaticString<tSize> &&str) noexcept {
            m_len = str.m_len;
            memcpy(m_buffer, str.m_buffer, m_len + 1);
            return *this;
        }

        /**
         * Assign operator assigns current object to a given character string
         *
         * @param str character string
         * @return current object
         */
        StaticString<tSize> &operator=(const char *str) {
            m_len = MIN(static_cast<size_type>(strlen(str)), tSize);
            memcpy(m_buffer, str, m_len + 1);
            return *this;
        }

        /**
         * Assign operator assigns current object to given character
         *
         * @param c given character
         * @return current object
         */
        StaticString<tSize> &operator=(const char c) {
            if (tSize == 0) {
                return *this;
            }
            m_len = 1;
            reinterpret_cast<uint16_t *>(m_buffer)[0] = static_cast<uint16_t>(c);
            return *this;
        }

        /**
         * Provides current length of string
         *
         * @return string length
         */
        size_type length() const {
            return m_len;
        }

        /**
         * Provides the maximum capacity of string
         *
         * @return string capacity
         */
        size_type capacity() const {
            return tSize;
        }

        /**
         * Checks if string is empty or not
         *
         * @return if string is empty or not
         */
        bool empty() const {
            return m_len == 0;
        }

        /**
         * Clears the string such that there are no characters left in it
         */
        void clear() noexcept {
            m_buffer[0] = '\0';
            m_len = 0;
        }

        /**
         * Element access operator gives access to character at @p pos
         *
         * @param pos the position of the character
         * @return character at @p pos
         */
        char &operator[](size_type pos) {
            return at(pos);
        }

        /**
         * Element access operator gives access to character at @p pos.
         * Character is constant
         *
         * @param pos the position of the character
         * @return character at @p pos
         */
        const char &operator[](size_type pos) const {
            return at(pos);
        }

        /**
         * Provides access to character at @p pos.
         * If the @p pos is out of bounds, last element
         * is returned
         *
         * @param pos the position of the character
         * @return character at @p pos
         */
        char &at(size_type pos) {
            if (pos >= m_len) { return back(); }

            return m_buffer[pos];
        }

        /**
         * Provides access to character at @p pos. If the @p pos
         * is out of bounds, last element is returned
         *
         * @param pos the position of the character
         * @return character at @p pos
         */
        const char &at(size_type pos) const {
            if (pos >= m_len) { return back(); }

            return m_buffer[pos];
        }

        /**
         * Provides access to the last character in the string
         *
         * @return the last character
         */
        char &back() {
            if (empty()) { return m_buffer[0]; }
            return m_buffer[m_len - 1];
        }

        /**
         * Provides access to the last character in the string. Character is constant
         *
         * @return the last character
         */
        const char &back() const {
            if (empty()) { return m_buffer[0]; }
            return m_buffer[m_len - 1];
        }

        /**
         * Provides access to the first character in the string
         *
         * @return the first character
         */
        char &front() {
            return m_buffer[0];
        }

        /**
         * Provides access to the first character in the string. Character is constant
         *
         * @return the first character
         */
        const char &front() const {
            return m_buffer[0];
        }

        /**
         * Modifier operator adds @code StaticString @endcode object to the current string. If String cannot
         * hold the given object string, it does not add it
         *
         * @param other @code StaticString @endcode string to add
         * @return the current string
         */
        StaticString<tSize> &operator+=(const StaticString<tSize> &other) {
            return append(other);
        }

        /**
         * Modifier operator adds char string to the current string. If String cannot
         * hold the given string, it does not add it
         *
         * @param val char string to add
         * @return the current string
         */
        StaticString<tSize> &operator+=(const char *val) {
            return append(val, static_cast<size_type>(strlen(val)));
        }

        /**
         * Modifier operator adds character to the current string. If String cannot
         * hold the character, it does not add it
         *
         * @param c character to add
         * @return the current string
         */
        StaticString<tSize> &operator+=(char c) {
            push_back(c);
            return *this;
        }

        /**
         * Appends a @code StaticString @endcode string to the current string. If String cannot
         * hold the given string, it does not add it
         *
         * @param str @code StaticString @endcode string to add
         * @return the current string
         */
        StaticString<tSize> &append(const StaticString<tSize> &str) {
            return append(str.c_str(), str.length());
        }

        StaticString<tSize> &append(const char *str) {
            return append(str, static_cast<size_type>(strlen(str)));
        }

        /**
         * Appends a character string to the current string. If String cannot
         * hold the given character string, it does not add it
         *
         * @param str character string to add
         * @return the current string
         */
        StaticString<tSize> &append(const char *str, size_type len) {
            char *start = m_buffer + m_len;
            size_type new_len = MIN(tSize, static_cast<size_type>(m_len + len));
            memcpy(start, str, new_len - m_len);
            m_len = new_len;
            m_buffer[m_len] = '\0';
            return *this;
        }

        /**
         * Appends a character to the current string. If String cannot
         * hold the given character, it does not add it
         *
         * @param c character to add
         * @return the current string
         */
        void push_back(char c) {
            if (m_len == tSize) {
                return;
            }
            reinterpret_cast<uint16_t *>(m_buffer + m_len)[0] = static_cast<uint16_t>(c);
            ++m_len;
        }

        /**
         * Deletes the element @p pos from the String
         *
         * @param pos position of the element to be deleted
         * @return the modified String
         */
        void erase(size_type pos = 0) {
            if (m_len == 0 || pos >= m_len) { return; }
            --m_len;
            memmove(m_buffer + pos, m_buffer + pos + 1, m_len - pos);
            m_buffer[m_len] = '\0';
        }

        /**
         * Deletes the last character in the String
         */
        void pop_back() {
            if (m_len == 0) { return; }
            --m_len;
            m_buffer[m_len] = '\0';
        }

        /**
         * Provides access to character array that string uses behind the screen
         *
         * @return character array
         */
        const char *c_str() const {
            return m_buffer;
        }

        /**
         * Makes substring of the current string. If the @p pos is out
         * of bounds, same String is returned. If the length of substring
         * is too long, then a substring from @p pos to the end is returned;
         *
         * @param pos starting position
         * @param length length of the new string
         * @return new string which is a substring of current string
         */
        StaticString<tSize> substr(size_type pos, size_type length) const {
            if (pos >= m_len) {
                return *this;
            }
            if (pos + length >= m_len) {
                length = static_cast<size_type>(m_len - pos);
            }
            StaticString<tSize> sub;
            memcpy(sub.m_buffer, m_buffer + pos, length);
            sub.m_buffer[length] = '\0';
            return sub;
        }

        /**
         * Compares two strings and return 0 if they are equal, less than 0 if
         * given string is less than current string and greater than 0 if
         * given string is greater than current string
         *
         * @param str @code StaticString @endcode string to compare against current string
         * @return a signed number based on how strings compare
         */
        diff_type compare(const StaticString<tSize> &str) const {
            return compare(str.c_str());
        }

        /**
         * Compares two strings and return 0 if they are equal, less than 0 if
         * given string is less than current string and greater than 0 if
         * given string is greater than current string
         *
         * @param str character string to compare against current string
         * @return a signed number based on how strings compare
         */
        diff_type compare(const char *str) const {
            return static_cast<diff_type>(strcmp(this->c_str(), str));
        }

        /**
         * Compares a string and a char and return 0 if they are equal, less than 0 if
         * given char is less than current string and greater than 0 if
         * given char is greater than current string
         *
         * @param c character to compare against current string
         * @return a signed number based on how strings compare
         */
        diff_type compare(char c) const {
            char array[] = {c, '\0'};
            return static_cast<diff_type>(strcmp(this->c_str(), array));
        }

        iterator begin() {
            return iterator(0, this);
        }

        iterator end() {
            return iterator(m_len, this);
        }

        const_iterator begin() const {
            return const_iterator(0, this);
        }

        const_iterator end() const {
            return const_iterator(m_len, this);
        }

    private:
        StaticString(const char *str1, const char *str2, size_type len1, size_type len2) {
            m_len = MIN(static_cast<size_type>(len1 + len2), tSize);
            size_type min_len = MIN(m_len, len1);
            memcpy(m_buffer, str1, min_len);
            memcpy(m_buffer + min_len, str2, m_len - min_len);
            m_buffer[m_len] = '\0';
        }

        char m_buffer[tSize + 1];
        size_type m_len;

        template<size_type size>
        friend StaticString<size> operator+(const StaticString<size> &, const StaticString<size> &);

        template<size_type size>
        friend StaticString<size> operator+(const StaticString<size> &, const char *);

        template<size_type size>
        friend StaticString<size> operator+(const char *, const StaticString<size> &);

        template<size_type size>
        friend StaticString<size> operator+(const StaticString<size> &, char);

        template<size_type size>
        friend StaticString<size> operator+(char, const StaticString<size> &);
    };

    /**
     * Comparative operator compares if two strings are equal or not
     *
     * @tparam tSize size of template string
     * @param lhs @code StaticString @endcode string as left hand side string
     * @param rhs @code StaticString @endcode string as right hand side string
     * @return true or false based on if two strings are equal
     */
    template<size_type tSize>
    bool operator==(const StaticString<tSize> &lhs, const StaticString<tSize> &rhs) {
        return lhs.compare(rhs) == 0;
    }

    /**
     * Comparative operator compares if two strings are equal or not
     *
     * @tparam tSize size of template string
     * @param lhs character string as left hand side string
     * @param rhs @code StaticString @endcode string as right hand side string
     * @return true or false based on if two strings are equal
     */
    template<size_type tSize>
    bool operator==(const char *lhs, const StaticString<tSize> &rhs) {
        return rhs.compare(lhs) == 0;
    }

    /**
     * Comparative operator compares if two strings are equal or not
     *
     * @tparam tSize size of template string
     * @param lhs @code StaticString @endcode string as left hand side string
     * @param rhs character string as right hand side string
     * @return true or false based on if two strings are equal
     */
    template<size_type tSize>
    bool operator==(const StaticString<tSize> &lhs, const char *rhs) {
        return lhs.compare(rhs) == 0;
    }

    /**
     * Comparative operator compares if a strings and a char are equal
     *
     * @tparam tSize size of template string
     * @param lhs @code StaticString @endcode string as left hand side string
     * @param rhs a character
     * @return true or false based on the string is equal to the character
     */
    template<size_type tSize>
    bool operator==(const StaticString<tSize> &lhs, const char rhs) {
        return lhs.compare(rhs) == 0;
    }

    /**
     * Comparative operator compares if a strings and a char are equal
     *
     * @tparam tSize size of template string
     * @param lhs a Character
     * @param rhs @code StaticString @endcode string as right hand side string
     * @return true or false based on the string is equal to the character
     */
    template<size_type tSize>
    bool operator==(const char lhs, const StaticString<tSize> &rhs) {
        return rhs.compare(lhs) == 0;
    }

    /**
     * Additive operator adds two given strings
     *
     * @tparam tSize size of template string
     * @param lhs @code StaticString @endcode string as left hand side string
     * @param rhs @code StaticString @endcode string as right hand side string
     * @return a @code StaticString @endcode that is build from left hind string and right hand string
     */
    template<size_type tSize>
    StaticString<tSize> operator+(const StaticString<tSize> &lhs, const StaticString<tSize> &rhs) {
        return {lhs.c_str(), rhs.c_str(), lhs.length(), rhs.length()};
    }

    /**
     * Additive operator adds two given strings
     *
     * @tparam tSize size of template string
     * @param lhs character string as left hand side string
     * @param rhs @code StaticString @endcode string as right hand side string
     * @return a @code StaticString @endcode that is build from left hind string and right hand string
     */
    template<size_type tSize>
    StaticString<tSize> operator+(const char *lhs, const StaticString<tSize> &rhs) {
        return {lhs, rhs.c_str(), static_cast<size_type>(strlen(lhs)), rhs.length()};
    }

    /**
     * Additive operator adds two given strings
     *
     * @tparam tSize size of template string
     * @param lhs @code StaticString @endcode string as left hand side string
     * @param rhs character string as right hand side string
     * @return a @code StaticString @endcode that is build from left hind string and right hand string
     */
    template<size_type tSize>
    StaticString<tSize> operator+(const StaticString<tSize> &lhs, const char *rhs) {
        return {lhs.c_str(), rhs, lhs.length(), static_cast<size_type>(strlen(rhs))};
    }

    /**
     * Additive operator adds a string with character
     *
     * @tparam tSize size of template string
     * @param lhs @code StaticString @endcode string as left hand side string
     * @param rhs character
     * @return a @code StaticString @endcode that is build from left hind string and right hand character
     */
    template<size_type tSize>
    StaticString<tSize> operator+(const StaticString<tSize> &lhs, const char rhs) {
        return {lhs.c_str(), &rhs, lhs.length(), 1};
    }

    /**
     * Additive operator adds a string with character
     *
     * @tparam tSize size of template string
     * @param lhs character
     * @param rhs @code StaticString @endcode string as right hand side string
     * @return a @code StaticString @endcode that is build from right hind string and left hand character
     */
    template<size_type tSize>
    StaticString<tSize> operator+(const char lhs, const StaticString<tSize> &rhs) {
        return {&lhs, rhs.c_str(), 1, rhs.length()};
    }
}

#endif //WLIB_STATICSTRING_H