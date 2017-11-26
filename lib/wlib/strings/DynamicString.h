/**
 * @file DynamicString.h
 * @brief DynamicString is a class that provides dynamic strings and functions for dynamic strings
 *
 * @author Bob Wei
 * @date November , 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDCPLUSPLUS_DYNAMICSTRING_H
#define EMBEDDEDCPLUSPLUS_DYNAMICSTRING_H

#include <string.h>

#include "../memory/Memory.h"
#include "../Types.h"

namespace wlp {

    class DynamicString {
    public:
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
        DynamicString(DynamicString &&str);

        /**
          * Destructor for DynamicString object.
          */
        ~DynamicString();

        /**
         * Assign operator assigns current object to given object.
         *
         * @param str @code DynamicString @endcode object
         * @return current object
         */
        DynamicString &operator=(const DynamicString &str);

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
        DynamicString &operator=(DynamicString &&str);

        /**
         * Provides current length of string.
         *
         * @return string length
         */
        size_type length() const;

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
        char &end();

        /**
         * Provides access to the last character in the string. Character is constant.
         *
         * @return the last character
         */
        const char &end() const;

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
        DynamicString &operator+=(DynamicString &other);

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
        DynamicString &push_back(const char c);

        /**
         * Deletes the element @p pos from the String.
         *
         * @param pos position of the element to be deleted
         * @return the modified String
         */
        DynamicString &erase(size_type pos = 0);

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
        diff_type compare(const char c) const;

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
         * Append method used by other public append methods.
         *
         * @param cstr c style string to append
         * @param len length of @p cstr
         * @return the @code DynamicString @endcode with @p cstr append to it
         */
        DynamicString &append(const char *cstr, size_type len);

        friend DynamicString operator+(const DynamicString &lhs, const DynamicString &rhs);

        friend DynamicString operator+(const char *lhs, const DynamicString &rhs);

        friend DynamicString operator+(const DynamicString &lhs, const char *rhs);

        friend DynamicString operator+(const DynamicString &lhs, const DynamicString &rhs);

        friend DynamicString operator+(char lhs, const DynamicString &rhs);

        friend DynamicString operator+(const DynamicString &lhs, char rhs);
    };

    /**
    * Comparative operator compares if two strings are equal or not.
    *
    * @param lhs @code DynamicString @endcode string as left hand side string
    * @param rhs @code DynamicString @endcode string as right hand side string
    * @return true or false based on if two strings are equal
    */

    bool operator==(const DynamicString &lhs, const DynamicString &rhs);

    /**
    * Comparative operator compares if two strings are equal or not.
    *
    * @param lhs character string as left hand side string
    * @param rhs @code DynamicString @endcode string as right hand side string
    * @return true or false based on if two strings are equal
    */
    bool operator==(const char *lhs, const DynamicString &rhs);

    /**
    * Comparative operator compares if two strings are equal or not.
    *
    * @param lhs @code DynamicString @endcode string as left hand side string
    * @param rhs character string as right hand side string
    * @return true or false based on if two strings are equal
    */
    bool operator==(const DynamicString &lhs, const char *rhs);

    /**
    * Comparative operator compares if string and single character are equal or not.
    *
    * @param lhs character as left hand side
    * @param rhs @code DynamicString @endcode string as right hand side string
    * @return true or false based on if two strings are equal
    */
    bool operator==(char lhs, const DynamicString &rhs);

    /**
    * Comparative operator compares if string and single character are equal or not.
    *
    * @param lhs @code DynamicString @endcode string as left hand side string
    * @param rhs character as right hand side
    * @return true or false based on if two strings are equal
    */
    bool operator==(const DynamicString &lhs, char rhs);

    /**
    * Additive operator adds two given strings.
    *
    * @param lhs @code DynamicString @endcode string as left hand side string
    * @param rhs @code DynamicString @endcode string as right hand side string
    * @return a @code DynamicString @endcode that is build from left hand string and right hand string
    */
    DynamicString operator+(const DynamicString &lhs, const DynamicString &rhs);

    /**
    * Additive operator adds two given strings.
    *
    * @param lhs character string as left hand side string
    * @param rhs @code DynamicString @endcode string as right hand side string
    * @return a @code DynamicString @endcode that is build from left hand string and right hand string
    */
    DynamicString operator+(const char *lhs, const DynamicString &rhs);

    /**
    * Additive operator adds two given strings.
    *
    * @param lhs @code DynamicString @endcode string as left hand side string
    * @param rhs character string as right hand side string
    * @return a @code DynamicString @endcode that is build from left hand string and right hand string
    */
    DynamicString operator+(const DynamicString &lhs, const char *rhs);

    /**
    * Additive operator single character to string.
    *
    * @param lhs character as left hand side
    * @param rhs @code DynamicString @endcode string as right hand side string
    * @return a @code DynamicString @endcode that is build from left hand string and right hand string
    */
    DynamicString operator+(char lhs, const DynamicString &rhs);

    /**
    * Additive operator adds string to single character.
    *
    * @param lhs @code DynamicString @endcode string as left hand side string
    * @param rhs character as right hand side
    * @return a @code DynamicString @endcode that is build from left hand string and right hand string
    */
    DynamicString operator+(const DynamicString &lhs, char rhs);
}

#endif //EMBEDDEDCPLUSPLUS_DYNAMICSTRING_H

