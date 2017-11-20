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

#include <string.h>
#include <math.h>

namespace wlp {
    template<uint16_t tSize>
    class StaticString {
    public:
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
        StaticString<tSize>(const StaticString<tSize> &str) : StaticString{str.c_str()} {}

        /**
         * Constructor creates string using character array
         *
         * @param str char string
         */
        explicit StaticString<tSize>(const char *str) {
            m_len = (uint16_t) strlen(str);
            if (m_len > capacity()) m_len = capacity();

            strncpy(m_buffer, str, m_len);
            m_buffer[m_len] = '\0';
        }

        /**
         * Assign operator assigns current object to given object
         *
         * @param str @code StaticString @endcode object
         * @return current object
         */
        StaticString<tSize> &operator=(const StaticString<tSize> &str) {
            return operator=(str.c_str());
        }

        /**
         * Assign operator assigns current object to a given character string
         *
         * @param str character string
         * @return current object
         */
        StaticString<tSize> &operator=(const char *str) {
            m_len = (uint16_t) ceil(fmin((uint16_t) strlen(str), capacity()));
            strncpy(m_buffer, str, m_len);
            m_buffer[m_len] = '\0';

            return *this;
        }

        /**
         * Assign operator assigns current object to given character
         *
         * @param c given character
         * @return current object
         */
        StaticString<tSize> &operator=(const char c) {
            char string[] = {c, '\0'};
            return operator=(string);
        }

        /**
         * Provides current length of string
         *
         * @return string length
         */
        uint16_t length() const {
            return m_len;
        }

        /**
         * Provides the maximum capacity of string
         *
         * @return string capacity
         */
        uint16_t capacity() {
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
        void clear() {
            m_buffer[0] = '\0';
            m_len = 0;
        }

        /**
         * Element access operator gives access to character at @p pos
         *
         * @param pos the position of the character
         * @return character at @p pos
         */
        char &operator[](uint16_t pos) {
            return at(pos);
        }

        /**
         * Element access operator gives access to character at @p pos.
         * Character is constant
         *
         * @param pos the position of the character
         * @return character at @p pos
         */
        const char &operator[](uint16_t pos) const {
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
        char &at(uint16_t pos) {
            if (pos >= m_len) return back();

            return m_buffer[pos];
        }

        /**
         * Provides access to character at @p pos. If the @p pos
         * is out of bounds, last element is returned
         *
         * @param pos the position of the character
         * @return character at @p pos
         */
        const char &at(uint16_t pos) const {
            if (pos >= m_len) return back();

            return m_buffer[pos];
        }

        /**
         * Provides access to the last character in the string
         *
         * @return the last character
         */
        char &back() {
            if (empty()) return m_buffer[0];
            return m_buffer[m_len - 1];
        }

        /**
         * Provides access to the last character in the string. Character is constant
         *
         * @return the last character
         */
        const char &back() const {
            if (empty()) return m_buffer[0];
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
            return append(val);
        }

        /**
         * Modifier operator adds character to the current string. If String cannot
         * hold the character, it does not add it
         *
         * @param c character to add
         * @return the current string
         */
        StaticString<tSize> &operator+=(char c) {
            return push_back(c);
        }

        /**
         * Appends a @code StaticString @endcode string to the current string. If String cannot
         * hold the given string, it does not add it
         *
         * @param str @code StaticString @endcode string to add
         * @return the current string
         */
        StaticString<tSize> &append(const StaticString<tSize> &str) {
            return append(str.c_str());
        }

        /**
         * Appends a character string to the current string. If String cannot
         * hold the given character string, it does not add it
         *
         * @param str character string to add
         * @return the current string
         */
        StaticString<tSize> &append(const char *str) {
            uint16_t bufferLength = m_len;
            uint16_t otherLength = (uint16_t) strlen(str);

            for (uint16_t i = bufferLength; i < bufferLength + otherLength && i < capacity(); i++) {
                m_buffer[i] = str[i - bufferLength];
            }

            m_len = (uint16_t) ceil(fmin(capacity(), (bufferLength + otherLength)));

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
        StaticString<tSize> &push_back(const char c) {
            char str[2] = {c, '\0'};
            return append(str);
        }

        /**
         * Deletes the element @p pos from the String
         *
         * @param pos position of the element to be deleted
         * @return the modified String
         */
        StaticString<tSize> &erase(uint16_t pos = 0) {
            if (m_len == 0 || pos >= m_len) return *this;

            --m_len;

            for (uint16_t i = pos; i < m_len; ++i) {
                m_buffer[i] = m_buffer[i + 1];
            }

            m_buffer[m_len] = '\0';
            return *this;
        }

        /**
         * Deletes the last character in the String
         */
        void pop_back() {
            if (m_len == 0) return;

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
        StaticString<tSize> substr(uint16_t pos, uint16_t length) const {
            if (pos >= m_len)
                return *this;

            if (pos + length >= m_len)
                length = (uint16_t) (m_len - pos);

            char newBuffer[length + 1];

            for (uint16_t i = pos; i < pos + length; i++) {
                newBuffer[i - pos] = m_buffer[i];
            }

            newBuffer[length] = '\0';

            StaticString<tSize> s{newBuffer};

            return s;
        }

        /**
         * Compares two strings and return 0 if they are equal, less than 0 if
         * given string is less than current string and greater than 0 if
         * given string is greater than current string
         *
         * @param str @code StaticString @endcode string to compare against current string
         * @return a signed number based on how strings compare
         */
        int16_t compare(const StaticString<tSize> &str) const {
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
        int16_t compare(const char *str) const {
            return (int16_t) strcmp(this->c_str(), str);
        }

        /**
         * Compares a string and a char and return 0 if they are equal, less than 0 if
         * given char is less than current string and greater than 0 if
         * given char is greater than current string
         *
         * @param c character to compare against current string
         * @return a signed number based on how strings compare
         */
        int16_t compare(char c) const {
            char array[] = {c, '\0'};
            return (int16_t) strcmp(this->c_str(), array);
        }

    private:
        char m_buffer[tSize + 1];
        uint16_t m_len;
    };

    /**
     * Comparative operator compares if two strings are equal or not
     *
     * @tparam tSize size of template string
     * @param lhs @code StaticString @endcode string as left hand side string
     * @param rhs @code StaticString @endcode string as right hand side string
     * @return true or false based on if two strings are equal
     */
    template<uint16_t tSize>
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
    template<uint16_t tSize>
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
    template<uint16_t tSize>
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
    template<uint16_t tSize>
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
    template<uint16_t tSize>
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
    template<uint16_t tSize>
    StaticString<tSize> operator+(const StaticString<tSize> &lhs, const StaticString<tSize> &rhs) {
        StaticString<tSize> newStr;
        newStr.append(lhs).append(rhs);
        return newStr;
    }

    /**
     * Additive operator adds two given strings
     *
     * @tparam tSize size of template string
     * @param lhs character string as left hand side string
     * @param rhs @code StaticString @endcode string as right hand side string
     * @return a @code StaticString @endcode that is build from left hind string and right hand string
     */
    template<uint16_t tSize>
    StaticString<tSize> operator+(const char *lhs, const StaticString<tSize> &rhs) {
        StaticString<tSize> newStr;
        newStr.append(lhs).append(rhs);
        return newStr;
    }

    /**
     * Additive operator adds two given strings
     *
     * @tparam tSize size of template string
     * @param lhs @code StaticString @endcode string as left hand side string
     * @param rhs character string as right hand side string
     * @return a @code StaticString @endcode that is build from left hind string and right hand string
     */
    template<uint16_t tSize>
    StaticString<tSize> operator+(const StaticString<tSize> &lhs, const char *rhs) {
        StaticString<tSize> newStr;
        newStr.append(lhs).append(rhs);
        return newStr;
    }

    /**
     * Additive operator adds a string with character
     *
     * @tparam tSize size of template string
     * @param lhs @code StaticString @endcode string as left hand side string
     * @param rhs character
     * @return a @code StaticString @endcode that is build from left hind string and right hand character
     */
    template<uint16_t tSize>
    StaticString<tSize> operator+(const StaticString<tSize> &lhs, const char rhs) {
        StaticString<tSize> newStr;
        newStr.append(lhs).push_back(rhs);
        return newStr;
    }

    /**
     * Additive operator adds a string with character
     *
     * @tparam tSize size of template string
     * @param lhs character
     * @param rhs @code StaticString @endcode string as right hand side string
     * @return a @code StaticString @endcode that is build from right hind string and left hand character
     */
    template<uint16_t tSize>
    StaticString<tSize> operator+(const char lhs, const StaticString<tSize> &rhs) {
        StaticString<tSize> newStr;
        newStr.push_back(lhs).append(rhs);
        return newStr;
    }
}

#endif //WLIB_STATICSTRING_H