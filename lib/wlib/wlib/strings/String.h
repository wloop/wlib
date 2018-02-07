/**
 * @file Types.h
 * @brief type definition of all the string types we will be using
 *
 * @author Deep Dhillon
 * @author Jeff Niu
 * @author Bob Wei
 * @date December 2, 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDCPLUSPLUS_STRINGTYPES_H
#define EMBEDDEDCPLUSPLUS_STRINGTYPES_H

#include <string.h> // strlen, strncpy, strcmp

#include "StringIterator.h"

#include "wlib/Types.h"
#include "wlib/utility/Math.h"
#include "wlib/memory/Memory.h"

namespace wlp {

    template<size_type tSize>
    class StaticString;

    class DynamicString;

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
        explicit StaticString<tSize>(const char *str)
                : StaticString(str, static_cast<size_type>(strlen(str))) {}

        /**
         * Construct a Static String from a Dynamic String.
         *
         * @param str dynamic string
         */
        explicit StaticString<tSize>(const DynamicString &str);

        /**
         * Construct a Static String from a character array and a known length.
         *
         * @param str character array
         * @param len length of the array
         */
        StaticString<tSize>(const char *str, size_type len) {
            m_len = MIN(len, tSize);
            memcpy(m_buffer, str, m_len);
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
         * Assignment operator to copy a Dynamic String
         *
         * @param str dynamic string to copy
         * @return reference to this string
         */
        StaticString<tSize> &operator=(const DynamicString &str);

        /**
         * The move assignment operator for a StaticString
         * must do a copy of the static array. This function
         * exists for string concept.
         *
         * @param str string to move
         * @return reference to this string
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
         * Append the contents of a Dynamic String. Excess
         * characters will be truncated.
         *
         * @param str dynamic string to append
         * @return reference to this string
         */
        StaticString<tSize> &operator+=(const DynamicString &str) {
            return append(str);
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
         * Add a static string to a dynamic string.
         *
         * @param str dynamic string to add
         * @return a new static string containing the most contents of both strings
         */
        StaticString<tSize> operator+(const DynamicString &str) const;

        StaticString<tSize> operator+(const StaticString<tSize> &str) const {
            return {m_buffer, str.m_buffer, m_len, str.m_len};
        }

        /**
         * Add a static string to a static string.
         *
         * @param str static string to add
         * @return a new static string containing the most contents of both strings
         */
        StaticString<tSize> operator=(const StaticString<tSize> &str) const {
            return {m_buffer, str.m_buffer, m_len, str.m_len};
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

        /**
         * Append the contents of a Dynamic String to this string.
         * Function truncates excess elements.
         *
         * @param str dynamic string to append
         * @return reference to this string
         */
        StaticString<tSize> &append(const DynamicString &str);

        /**
         * Append a character array with unknown length.
         *
         * @param str character array
         * @return reference to this string
         */
        StaticString<tSize> &append(const char *str) {
            return append(str, static_cast<size_type>(strlen(str)));
        }

        /**
         * Appends a character string to the current string. The function
         * truncates excess elements.
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
         * Appends a character to the current string. The function
         * truncates excess elements.
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
         * Compare with a Dynamic String. Returns 0 if equal, negative if
         * this string is less, and positive if this string is greater.
         *
         * @param str dynamic string with which to compare
         * @return signed difference number
         */
        diff_type compare(const DynamicString &str) const;

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
            if (!m_len) {
                return -1;
            }
            diff_type first = static_cast<diff_type>(m_buffer[0] - c);
            if (!first) {
                return m_len > 1;
            }
            return first;
        }

        /**
         * @return iterator to the first character in the string
         */
        iterator begin() {
            return iterator(0, this);
        }

        /**
         * @return pass-the-end iterator
         */
        iterator end() {
            return iterator(m_len, this);
        }

        /**
         * @return const iterator to the first character in the string
         */
        const_iterator begin() const {
            return const_iterator(0, this);
        }

        /**
         * @return pass-the-end iterator
         */
        const_iterator end() const {
            return const_iterator(m_len, this);
        }

        // Comparison operators with static string and dynamic string
        bool operator==(const StaticString<tSize> &str) const {
            return compare(str) == 0;
        }

        bool operator!=(const StaticString<tSize> &str) const {
            return compare(str) != 0;
        }

        bool operator>(const StaticString<tSize> &str) const {
            return compare(str) > 0;
        }

        bool operator>=(const StaticString<tSize> &str) const {
            return compare(str) >= 0;
        }

        bool operator<(const StaticString<tSize> &str) const {
            return compare(str) < 0;
        }

        bool operator<=(const StaticString<tSize> &str) const {
            return compare(str) <= 0;
        }

        bool operator==(const DynamicString &str) const {
            return compare(str) == 0;
        }

        bool operator!=(const DynamicString &str) const {
            return compare(str) != 0;
        }

        bool operator>(const DynamicString &str) const {
            return compare(str) > 0;
        }

        bool operator>=(const DynamicString &str) const {
            return compare(str) >= 0;
        }

        bool operator<(const DynamicString &str) const {
            return compare(str) < 0;
        }

        bool operator<=(const DynamicString &str) const {
            return compare(str) <= 0;
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

        // Asymmetrical addition operators
        template<size_type size>
        friend StaticString<size> operator+(const StaticString<size> &, const char *);

        template<size_type size>
        friend StaticString<size> operator+(const char *, const StaticString<size> &);

        template<size_type size>
        friend StaticString<size> operator+(const StaticString<size> &, char);

        template<size_type size>
        friend StaticString<size> operator+(char, const StaticString<size> &);
    };

    // Comparison with LHS character array
    template<size_type tSize>
    bool operator==(const char *lhs, const StaticString<tSize> &rhs) {
        return rhs.compare(lhs) == 0;
    }

    template<size_type tSize>
    bool operator!=(const char *lhs, const StaticString<tSize> &rhs) {
        return rhs.compare(lhs) != 0;
    }

    template<size_type tSize>
    bool operator>(const char *lhs, const StaticString<tSize> &rhs) {
        return rhs.compare(lhs) <= 0;
    }

    template<size_type tSize>
    bool operator>=(const char *lhs, const StaticString<tSize> &rhs) {
        return rhs.compare(lhs) < 0;
    }

    template<size_type tSize>
    bool operator<(const char *lhs, const StaticString<tSize> &rhs) {
        return rhs.compare(lhs) >= 0;
    }

    template<size_type tSize>
    bool operator<=(const char *lhs, const StaticString<tSize> &rhs) {
        return rhs.compare(lhs) > 0;
    }

    // Comparison with RHS character array
    template<size_type tSize>
    bool operator==(const StaticString<tSize> &lhs, const char *rhs) {
        return lhs.compare(rhs) == 0;
    }

    template<size_type tSize>
    bool operator!=(const StaticString<tSize> &lhs, const char *rhs) {
        return lhs.compare(rhs) != 0;
    }

    template<size_type tSize>
    bool operator>(const StaticString<tSize> &lhs, const char *rhs) {
        return lhs.compare(rhs) > 0;
    }

    template<size_type tSize>
    bool operator>=(const StaticString<tSize> &lhs, const char *rhs) {
        return lhs.compare(rhs) >= 0;
    }

    template<size_type tSize>
    bool operator<(const StaticString<tSize> &lhs, const char *rhs) {
        return lhs.compare(rhs) < 0;
    }

    template<size_type tSize>
    bool operator<=(const StaticString<tSize> &lhs, const char *rhs) {
        return lhs.compare(rhs) <= 0;
    }

    // Comparison with RHS character
    template<size_type tSize>
    bool operator==(const StaticString<tSize> &lhs, const char rhs) {
        return lhs.length() == 1 && lhs.at(0) == rhs;
    }

    template<size_type tSize>
    bool operator!=(const StaticString<tSize> &lhs, const char rhs) {
        return lhs.length() != 1 || lhs.at(0) != rhs;
    }

    template<size_type tSize>
    bool operator>(const StaticString<tSize> &lhs, const char rhs) {
        return lhs.compare(rhs) > 0;
    }

    template<size_type tSize>
    bool operator>=(const StaticString<tSize> &lhs, const char rhs) {
        return lhs.compare(rhs) >= 0;
    }

    template<size_type tSize>
    bool operator<(const StaticString<tSize> &lhs, const char rhs) {
        return lhs.compare(rhs) < 0;
    }

    template<size_type tSize>
    bool operator<=(const StaticString<tSize> &lhs, const char rhs) {
        return lhs.compare(rhs) <= 0;
    }

    // Comparison with LHS character
    template<size_type tSize>
    bool operator==(const char lhs, const StaticString<tSize> &rhs) {
        return rhs == lhs;
    }

    template<size_type tSize>
    bool operator!=(const char lhs, const StaticString<tSize> &rhs) {
        return rhs != lhs;
    }

    template<size_type tSize>
    bool operator>(const char lhs, const StaticString<tSize> &rhs) {
        return rhs <= lhs;
    }

    template<size_type tSize>
    bool operator>=(const char lhs, const StaticString<tSize> &rhs) {
        return rhs < lhs;
    }

    template<size_type tSize>
    bool operator<(const char lhs, const StaticString<tSize> &rhs) {
        return rhs >= lhs;
    }

    template<size_type tSize>
    bool operator<=(const char lhs, const StaticString<tSize> &rhs) {
        return rhs > lhs;
    }

    template<size_type tSize>
    StaticString<tSize> operator+(const char *lhs, const StaticString<tSize> &rhs) {
        return {lhs, rhs.c_str(), static_cast<size_type>(strlen(lhs)), rhs.length()};
    }

    template<size_type tSize>
    StaticString<tSize> operator+(const StaticString<tSize> &lhs, const char *rhs) {
        return {lhs.c_str(), rhs, lhs.length(), static_cast<size_type>(strlen(rhs))};
    }

    template<size_type tSize>
    StaticString<tSize> operator+(const StaticString<tSize> &lhs, const char rhs) {
        return {lhs.c_str(), &rhs, lhs.length(), 1};
    }

    template<size_type tSize>
    StaticString<tSize> operator+(const char lhs, const StaticString<tSize> &rhs) {
        return {&lhs, rhs.c_str(), 1, rhs.length()};
    }

    class DynamicString {
    public:
        // Required types for concept check
        typedef wlp::size_type size_type;
        typedef wlp::diff_type diff_type;

        // Iterator types
        typedef StringIterator<DynamicString, char &, char *> iterator;
        typedef StringIterator<const DynamicString, const char &, const char *> const_iterator;

        /**
         * Default constructor creates string with no characters.
         */
        DynamicString();

        /**
         * Constructor creates string using character array.
         *
         * @param str char string
         */
        explicit DynamicString(const char *str);

        /**
         * Construct a dynamic string from a static string.
         *
         * @param str static string to copy
         */
        template<size_type tSize>
        explicit DynamicString(const StaticString<tSize> &str)
                : DynamicString(str.c_str(), str.length()) {}

        /**
         * Construct a dynamic string from a character array and
         * a known length.
         *
         * @param str character array
         * @param len length of the array
         */
        DynamicString(const char *str, size_type len);

        /**
         * Constructor creates string using DynamicString object.
         *
         * @param str @code DynamicString @endcode object
         */
        DynamicString(const DynamicString &str);

        /**
         * Move constructor will transfer the underlying string.
         *
         * @param str the @code DynamicString @endcode to move
         */
        DynamicString(DynamicString &&str) noexcept;

        /**
          * Destructor for DynamicString object.
          */
        ~DynamicString();

        void set_value(const char *str, size_type len);

        /**
         * Assign operator assigns current object to given object.
         *
         * @param str @code DynamicString @endcode object
         * @return current object
         */
        DynamicString &operator=(const DynamicString &str);

        template<size_type tSize>
        DynamicString &operator=(const StaticString<tSize> &str) {
            set_value(str.c_str(), str.length());
            return *this;
        }

        /**
         * Assign operator assigns current object to given character string.
         *
         * @param str
         * @return current object
         */
        DynamicString &operator=(const char *str);

        /**
         * Move assignment operator transfers the underlying
         * character array.
         *
         * @param str the @code DynamicString @endcode to move
         */
        DynamicString &operator=(DynamicString &&str) noexcept;

        /**
         * Assignment operator for a single character.
         *
         * @param c character to assign
         * @return reference to this string
         */
        DynamicString &operator=(char c);

        /**
         * Provides current length of string.
         *
         * @return string length
         */
        size_type length() const;

        /**
         * The DynamicString has capacity equal to the maximum
         * possible value of @code size_type @endcode.
         *
         * @return the maximum dynamic string capcacity
         */
        size_type capacity() const;

        /**
         * Clears the string such that there are no characters left in it.
         */
        void clear() noexcept;

        /**
         * Element access operator gives access to character at @code pos.
         *
         * @param pos the position of the character
         * @return character at @code position @endcode
         */
        char &operator[](size_type pos);

        /**
         * Element access operator gives access to character at @code pos.
         * Character is constant.
         *
         * @param pos the position of the character
         * @return character at @code position @endcode
         */
        const char &operator[](size_type pos) const;

        /**
         * Provides access to character at @code pos with bounds checking.
         *
         * @param pos the position of the character
         * @return character at @code position @endcode
         */
        char &at(size_type pos);

        /**
         * Provides access to character at @code pos @endcode with bounds
         * checking. Character is constant.
         *
         * @param pos the position of the character
         * @return character at @code position @endcode
         */
        const char &at(size_type pos) const;

        /**
         * Checks if string is empty or not.
         *
         * @return if string is empty or not
         */
        bool empty() const;

        /**
         * Provides access to the first character in the string.
         *
         * @return the first character
         */
        char &front();

        /**
         * Provides access to the first character in the string. Character is constant.
         *
         * @return the first character
         */
        const char &front() const;

        /**
         * Provides access to the last character in the string.
         *
         * @return the last character
         */
        char &back();

        /**
         * Provides access to the last character in the string. Character is constant.
         *
         * @return the last character
         */
        const char &back() const;

        /**
         * Modifier operator adds character to the current string.
         *
         * @param c character to add
         * @return the current string
         */
        DynamicString &operator+=(char c);

        /**
         * Modifier operator adds char string to the current string.
         *
         * @param val char string to add
         * @return the current string
         */
        DynamicString &operator+=(const char *val);

        /**
         * Modifier operator adds @code DynamicString @endcode object to the current string.
         *
         * @param other @code DynamicString @endcode string to add
         * @return the current string
         */
        DynamicString &operator+=(const DynamicString &other);

        template<size_type tSize>
        DynamicString &operator+=(const StaticString<tSize> &str) {
            return append(str.c_str(), str.length());
        }

        /**
         * Appends a character string to the current string.
         *
         * @param str character string to add
         * @return the current string
         */
        DynamicString &append(const char *str);

        /**
         * Appends a DynamicString string to the current string.
         *
         * @param str DynamicString string to add
         * @return the current string
         */
        DynamicString &append(const DynamicString &str);

        /**
         * Appends a character to the current string.
         *
         * @param c character to add
         * @return the current string
         */
        void push_back(char c);

        /**
         * Deletes the element @p pos from the String.
         *
         * @param pos position of the element to be deleted
         * @return the modified String
         */
        void erase(size_type pos = 0);

        /**
         * Deletes the last character in the String.
         */
        void pop_back();

        /**
         * Provides access to character array that string uses behind the screen.
         *
         * @return character array
         */
        const char *c_str() const;

        /**
         * Makes substring of the current string.
         *
         * @param pos starting position
         * @param length length of the new string
         * @return new string which is a substring of current string
         */
        DynamicString substr(size_type pos, size_type length) const;

        /**
         * Compares two strings and return 0 if they are equal, less than 0 if
         * given string is less than current string and greater than 0 if
         * given string is greater than current string.
         *
         * @param str @code DynamicString string to compare against current string
         * @return a signed number based on how strings compare
         */
        diff_type compare(const DynamicString &str) const;

        template<size_type tSize>
        diff_type compare(const StaticString<tSize> &str) const {
            return compare(str.c_str());
        }

        /**
         * Compares two strings and return 0 if they are equal, less than 0 if
         * given string is less than current string and greater than 0 if
         * given string is greater than current string.
         *
         * @param str character string to compare against current string
         * @return a signed number based on how strings compare
         */
        diff_type compare(const char *str) const;

        /**
         * Compares a string and character and return 0 if they are equal, less than 0 if
         * given string is less than current string and greater than 0 if
         * given string is greater than current string.
         *
         * @param c character to compare against current string
         * @return a signed number based on how strings compare
         */
        diff_type compare(char c) const;

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

        template<size_type tSize>
        DynamicString operator+(const StaticString<tSize> &str) {
            return {m_buffer, str.c_str(), m_len, str.length()};
        }

    private:
        char *m_buffer;
        size_type m_len;

        /**
         * Constructor used by other String constructors to create @code DynamicString @endcode.
         *
         * @param str1 first string to use in making
         * @param str2 second string to use in making
         * @param len1 length of first string
         * @param len2 length of second string
         */
        DynamicString(const char *str1, const char *str2, size_type len1, size_type len2);

        /**
         * Constructor for populating a DynamicString with a dynamically allocated
         * character array which the string takes ownership of and its length.
         *
         * @param str dynamically allocated character array filled with characters
         * @param len length of the string
         */
        DynamicString(size_type len, char *str);

        /**
         * Append method used by other public append methods.
         *
         * @param c_str c style string to append
         * @param len length of @p c_str
         * @return the @code DynamicString @endcode with @p c_str append to it
         */
        DynamicString &append(const char *c_str, size_type len);

        friend DynamicString operator+(const DynamicString &lhs, const DynamicString &rhs);

        friend DynamicString operator+(const char *lhs, const DynamicString &rhs);

        friend DynamicString operator+(const DynamicString &lhs, const char *rhs);

        friend DynamicString operator+(char lhs, const DynamicString &rhs);

        friend DynamicString operator+(const DynamicString &lhs, char rhs);
    };

    bool operator==(const DynamicString &lhs, const DynamicString &rhs);

    bool operator!=(const DynamicString &lhs, const DynamicString &rhs);

    bool operator>(const DynamicString &lhs, const DynamicString &rhs);

    bool operator>=(const DynamicString &lhs, const DynamicString &rhs);

    bool operator<(const DynamicString &lhs, const DynamicString &rhs);

    bool operator<=(const DynamicString &lhs, const DynamicString &rhs);

    bool operator==(const char *lhs, const DynamicString &rhs);

    bool operator!=(const char *lhs, const DynamicString &rhs);

    bool operator>(const char *lhs, const DynamicString &rhs);

    bool operator>=(const char *lhs, const DynamicString &rhs);

    bool operator<(const char *lhs, const DynamicString &rhs);

    bool operator<=(const char *lhs, const DynamicString &rhs);

    bool operator==(const DynamicString &lhs, const char *rhs);

    bool operator!=(const DynamicString &lhs, const char *rhs);

    bool operator>(const DynamicString &lhs, const char *rhs);

    bool operator>=(const DynamicString &lhs, const char *rhs);

    bool operator<(const DynamicString &lhs, const char *rhs);

    bool operator<=(const DynamicString &lhs, const char *rhs);

    bool operator==(char lhs, const DynamicString &rhs);

    bool operator==(const DynamicString &lhs, char rhs);

    template<size_type tSize>
    StaticString<tSize>::StaticString(const DynamicString &str)
            : StaticString(str.c_str(), str.length()) {
    }

    template<size_type tSize>
    StaticString<tSize> &StaticString<tSize>::operator=(const DynamicString &str) {
        m_len = MIN(str.length(), tSize);
        memcpy(m_buffer, str.c_str(), m_len);
        m_buffer[m_len] = '\0';
        return *this;
    }

    template<size_type tSize>
    StaticString<tSize> StaticString<tSize>::operator+(const DynamicString &str) const {
        return {m_buffer, str.c_str(), m_len, str.length()};
    }

    template<size_type tSize>
    StaticString<tSize> &StaticString<tSize>::append(const DynamicString &str) {
        return append(str.c_str(), str.length());
    }

    template<size_type tSize>
    diff_type StaticString<tSize>::compare(const DynamicString &str) const {
        return compare(str.c_str());
    }

    // Static Strings
    typedef StaticString<8u> String8;
    typedef StaticString<16u> String16;
    typedef StaticString<32u> String32;
    typedef StaticString<64u> String64;
    typedef StaticString<128u> String128;
    typedef StaticString<256u> String256;

    // Dynamic String
    typedef wlp::DynamicString String;

}

#endif //EMBEDDEDCPLUSPLUS_STRINGTYPES_H
