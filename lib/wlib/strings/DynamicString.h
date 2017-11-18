#ifndef WLIB_DYNAMICSTRING_H
#define WLIB_DYNAMICSTRING_H

#include <stdint.h>
#include <string.h>
#include <math.h>
#include "memory/Memory.h"
#include "Types.h"

class DynamicString {
private:
	char * m_buffer;
public:
	/**
	 * Default constructor creates string with no characters
	 */
	DynamicString(){
		m_buffer = (char *)memory_alloc(sizeof(char));
		m_buffer[0] = '\0';
	}

	/**
	 * Constructor creates string using character array
	 *
	 * @param str char string
	 */
	DynamicString(const char* str){
		size_type size = (size_type) strlen(str);
		m_buffer = (char *)memory_alloc((size + 1) * sizeof(char));
		strncpy(m_buffer, str, size);
		m_buffer[size] = '\0';
	}

	/**
	 * Constructor creates string using DynamicString object
	 *
	 * @param str @code DynamicString object
	 */
	DynamicString(const DynamicString &str){
		DynamicString(str.c_str());
	}

	/**
	 * Assign operator assigns current object to given object
	 *
	 * @param str @code DynamicString object
	 * @return current object
	 */
	DynamicString operator= (const DynamicString &str){
		return operator=(str.c_str());
	}

	/**
	 * Assign operator assigns current object to given character string
	 * @param str
	 * @return current object
	 */
	DynamicString operator= (const char* str){
		size_type size = (size_type) strlen(str);
		m_buffer = (char *)memory_realloc(m_buffer, (size + 1) * sizeof(char));
		strncpy(m_buffer, str, size);
		m_buffer[size] = '\0';

		return *this;
	}

	/**
	 * Provides current length of string
	 *
	 * @return string length
	 */
	size_type length() const{
		size_type i = 0;
		while (m_buffer[i] != '\0') i++;
		return i;
	}

	/**
	 * Clears the string such that there are no characters left in it
	 */
	void clear(){
		m_buffer[0] = '\0';
	}

	/**
	 * Element access operator gives access to character at @code pos
	 *
	 * @param pos the position of the character
	 * @return character at @code position
	 */
	char &operator[](size_type pos){
		return m_buffer[pos];
	}

	/**
	 * Element access operator gives access to character at @code pos.
	 * Character is constant
	 *
	 * @param pos the position of the character
	 * @return character at @code position
	 */
	const char &operator[](size_type pos) const{
		return m_buffer[pos];
	}

	/**
	 * Provides access to character at @code pos
	 *
	 * @param pos the position of the character
	 * @return character at @code position
	 */
	char &at(size_type pos){
		return m_buffer[pos];
	}

	/**
	 * Provides access to character at @code pos. Character is constant
	 *
	 * @param pos the position of the character
	 * @return character at @code position
	 */
	const char &at(size_type pos) const{
		return m_buffer[pos];
	}

	/**
	 * Checks if string is empty or not
	 *
	 * @return if string is empty or not
	 */
	bool empty() const {
		return length() == 0;
	}

	/**
	 * Provides access to the first character in the string
	 *
	 * @return the first character
	 */
	char &front(){
		return m_buffer[0];
	}

	/**
	 * Provides access to the first character in the string. Character is constant
	 *
	 * @return the first character
	 */
	const char &front() const{
		return m_buffer[0];
	}

	/**
	 * Provides access to the last character in the string
	 *
	 * @return the last character
	 */
	char &end(){
		if (empty()) return m_buffer[0];

		return m_buffer[length() - 1];
	}

	/**
	 * Provides access to the last character in the string. Character is constant
	 *
	 * @return the last character
	 */
	const char &end() const{
		if (empty()) return m_buffer[0];

		return m_buffer[length() - 1];
	}

	/**
	 * Modifier operator adds character to the current string.
	 *
	 * @param c character to add
	 * @return the current string
	 */
	DynamicString &operator+=(char c){
		char array[1] = {c};
		return append(array);
	}

	/**
	 * Modifier operator adds char string to the current string.
	 *
	 * @param val char string to add
	 * @return the current string
	 */
	DynamicString &operator+= (const char* val){
		return append(val);
	}

	/**
	 * Modifier operator adds @code DynamicString object to the current string.
	 *
	 * @param other @code DynamicString string to add
	 * @return the current string
	 */
	DynamicString &operator+= (DynamicString &other){
		return append(other.c_str());
	}

	/**
	 * Appends a character to the current string.
	 *
	 * @param c character to add
	 * @return the current string
	 */
	DynamicString &append(const char c){
		char array[2] = {c, '\0'};
		return append(array);
	}

	/**
	 * Appends a character string to the current string.
	 *
	 * @param str character string to add
	 * @return the current string
	 */
	DynamicString &append(const char *str){
		size_type bufferLength = this->length();
		size_type newLength = (size_type)(bufferLength + strlen(str));
		m_buffer = (char *)memory_realloc(m_buffer, (newLength + 1) * sizeof(char));
		
		for (int i = bufferLength; i < newLength; i++) {
			m_buffer[i] = str[i-bufferLength];
		}
		
		m_buffer[newLength] = '\0';

		return *this;
	}

	/**
	 * Appends a DynamicString string to the current string.
	 *
	 * @param str DynamicString string to add
	 * @return the current string
	 */
	DynamicString &append(const DynamicString &str){
		return append(str.c_str());
	}

	/**
	 * Appends a character to the current string.
	 *
	 * @param c character to add
	 * @return the current string
	 */
	DynamicString &push_back(const char c){
		char array[2] = {c, '\0'};
		return append(array);
	}

	/**
     * Deletes the element @p pos from the String
     *
     * @param pos position of the element to be deleted
     * @return the modified String
     */
    DynamicString &erase(size_type pos = 0){
		size_type len = this->length();
        if (len == 0 || pos >= len) return *this;

		len--;

        for (size_type i = pos; i < len; ++i) {
            m_buffer[i] = m_buffer[i+1];
        }

        m_buffer[len] = '\0';
        return *this;
    }

    /**
     * Deletes the last character in the String
     */
    void pop_back(){
		size_type len = this->length();
        if (len == 0) return;

        m_buffer[len - 1] = '\0';
    }

	/**
	 * Provides access to character array that string uses behind the screen
	 *
	 * @return character array
	 */
	const char* c_str() const{
		return m_buffer;
	}

	/**
	 * Makes substring of the current string
	 *
	 * @param pos starting position
	 * @param length length of the new string
	 * @return new string which is a substring of current string
	 */
	DynamicString substr(size_type pos, size_type length) const{
		char newBuffer[length + 1];

		for(size_type i = pos; i < pos + length ;i++){
			newBuffer[i-pos] = m_buffer[i];
		}

		newBuffer[length] = '\0';

		DynamicString s{newBuffer};

		return s;
	}

	/**
	 * Compares two strings and return 0 if they are equal, less than 0 if
	 * given string is less than current string and greater than 0 if
	 * given string is greater than current string
	 *
	 * @param str @code DynamicString string to compare against current string
	 * @return a signed number based on how strings compare
	 */
	int16_t compare(const DynamicString &str) const{
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
	int16_t compare(const char *str) const{
		return (int16_t) strcmp(this->c_str(), str);
	}

	/**
	 * Compares a string and character and return 0 if they are equal, less than 0 if
	 * given string is less than current string and greater than 0 if
	 * given string is greater than current string
	 *
	 * @param c character to compare against current string
	 * @return a signed number based on how strings compare
	 */
	int16_t compare(const char c) const{
		char array[2] = {c, '\0'};
		return (int16_t) strcmp(this->c_str(), array);
	}
};

/**
 * Comparative operator compares if two strings are equal or not
 *
 * @param lhs @code DynamicString string as left hand side string
 * @param rhs @code DynamicString string as right hand side string
 * @return true or false based on if two strings are equal
 */

bool operator== (const DynamicString &lhs, const DynamicString &rhs){
	return lhs.compare(rhs) == 0;
}

/**
 * Comparative operator compares if two strings are equal or not
 *
 * @param lhs character string as left hand side string
 * @param rhs @code DynamicString string as right hand side string
 * @return true or false based on if two strings are equal
 */
bool operator== (const char *lhs, const DynamicString &rhs){
	return rhs.compare(lhs) == 0;
}

/**
 * Comparative operator compares if two strings are equal or not
 *
 * @param lhs @code DynamicString string as left hand side string
 * @param rhs character string as right hand side string
 * @return true or false based on if two strings are equal
 */
bool operator== (const DynamicString &lhs, const char *rhs){
	return lhs.compare(rhs) == 0;
}

/**
 * Comparative operator compares if string and single character are equal or not
 *
 * @param lhs character as left hand side
 * @param rhs @code DynamicString string as right hand side string
 * @return true or false based on if two strings are equal
 */
bool operator== (const char lhs, const DynamicString &rhs){
	return rhs.compare(lhs) == 0;
}

/**
 * Comparative operator compares if string and single character are equal or not
 *
 * @param lhs @code DynamicString string as left hand side string
 * @param rhs character as right hand side
 * @return true or false based on if two strings are equal
 */
bool operator== (const DynamicString &lhs, const char rhs){
	return lhs.compare(rhs) == 0;
}

/**
 * Additive operator adds two given strings
 *
 * @param lhs @code DynamicString string as left hand side string
 * @param rhs @code DynamicString string as right hand side string
 * @return a @code DynamicString that is build from left hand string and right hand string
 */
DynamicString operator+(const DynamicString &lhs, const DynamicString &rhs){
	DynamicString newStr;
	newStr.append(lhs).append(rhs);
	return newStr;
}

/**
 * Additive operator adds two given strings
 *
 * @param lhs character string as left hand side string
 * @param rhs @code DynamicString string as right hand side string
 * @return a @code DynamicString that is build from left hand string and right hand string
 */
DynamicString operator+(const char *lhs, const DynamicString &rhs){
	DynamicString newStr;
	newStr.append(lhs).append(rhs);
	return newStr;
}

/**
 * Additive operator adds two given strings
 *
 * @param lhs @code DynamicString string as left hand side string
 * @param rhs character string as right hand side string
 * @return a @code DynamicString that is build from left hand string and right hand string
 */
DynamicString operator+(const DynamicString &lhs, const char *rhs){
	DynamicString newStr;
	newStr.append(lhs).append(rhs);
	return newStr;
}

/**
 * Additive operator single character to string
 *
 * @param lhs character as left hand side
 * @param rhs @code DynamicString string as right hand side string
 * @return a @code DynamicString that is build from left hand string and right hand string
 */
DynamicString operator+(const char lhs, const DynamicString &rhs){
	DynamicString newStr;
	newStr.append(lhs).append(rhs);
	return newStr;
}

/**
 * Additive operator adds string to single character
 *
 * @param lhs @code DynamicString string as left hand side string
 * @param rhs character as right hand side
 * @return a @code DynamicString that is build from left hand string and right hand string
 */
DynamicString operator+(const DynamicString &lhs, const char rhs){
	DynamicString newStr;
	newStr.append(lhs).append(rhs);
	return newStr;
}

/**
 * Destructor function for dynamic string
 */
~DynamicString() {
	memory_free(m_buffer);
}

#endif //WLIB_DYNAMICSTRING_H