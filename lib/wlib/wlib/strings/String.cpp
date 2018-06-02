/**
 * @file DynamicString.cpp
 * @brief DynamicString is a class that provides dynamic strings and functions for dynamic strings
 *
 * @author Bob Wei
 * @author Jeff Niu
 * @date November 25, 2017
 * @bug No known bugs
 */

#include <wlib/strings/String.h>

namespace wlp {

    dynamic_string::dynamic_string() : dynamic_string(nullptr, nullptr, 0, 0) {}

    dynamic_string::dynamic_string(const char *str)
            : dynamic_string(str, nullptr, static_cast<size_type>(strlen(str)), 0) {}

    dynamic_string::dynamic_string(const char *str, size_type len)
            : dynamic_string(str, nullptr, len, 0) {}

    dynamic_string::dynamic_string(size_type len, char *str)
            : m_buffer(str),
              m_len(len) {}

    dynamic_string::dynamic_string(const dynamic_string &str) : dynamic_string(str.c_str()) {}

    dynamic_string::dynamic_string(dynamic_string &&str) noexcept
            : m_buffer(str.m_buffer),
              m_len(str.m_len) {
        str.m_len = 0;
        str.m_buffer = malloc<char[]>(1);
        str.m_buffer[0] = '\0';
    }

    dynamic_string::dynamic_string(const char *str1, const char *str2, size_type len1, size_type len2) {
        m_len = len1 + len2;
        m_buffer = malloc<char[]>(static_cast<size_type>(m_len + 1));
        memcpy(m_buffer, str1, len1);
        memcpy(m_buffer + len1, str2, len2);
        m_buffer[m_len] = '\0';
    }

    dynamic_string::~dynamic_string() {
        if (m_buffer) {
            free<char>(m_buffer);
        }
    }

    void dynamic_string::set_value(const char *str, size_type len) {
        if (len > m_len) {
            free(m_buffer);
            m_buffer = malloc<char[]>(static_cast<size_type>(len + 1));
        }
        m_len = len;
        memcpy(m_buffer, str, len);
        m_buffer[len] = '\0';
    }

    dynamic_string &dynamic_string::operator=(const dynamic_string &str) {
        set_value(str.c_str(), str.length());
        return *this;
    }

    dynamic_string &dynamic_string::operator=(const char *str) {
        set_value(str, static_cast<size_type>(strlen(str)));
        return *this;
    }

    dynamic_string &dynamic_string::operator=(dynamic_string &&str) noexcept {
        free<char>(m_buffer);
        m_buffer = str.m_buffer;
        m_len = str.m_len;
        str.m_len = 0;
        str.m_buffer = malloc<char[]>(1);
        str.m_buffer[0] = '\0';
        return *this;
    }

    dynamic_string &dynamic_string::operator=(const char c) {
        free<char>(m_buffer);
        m_buffer = malloc<char[]>(2);
        reinterpret_cast<uint16_t *>(m_buffer)[0] = static_cast<uint16_t>(c);
        return *this;
    }

    size_type dynamic_string::length() const {
        return m_len;
    }

    size_type dynamic_string::capacity() const {
        return static_cast<size_type>(-1);
    }

    void dynamic_string::clear() noexcept {
        m_buffer[0] = '\0';
        m_len = 0;
    }

    char &dynamic_string::operator[](size_type pos) {
        return m_buffer[pos];
    }

    const char &dynamic_string::operator[](size_type pos) const {
        return m_buffer[pos];
    }

    char &dynamic_string::at(size_type pos) {
        return pos < m_len ? m_buffer[pos] : m_buffer[m_len];
    }

    const char &dynamic_string::at(size_type pos) const {
        return pos < m_len ? m_buffer[pos] : m_buffer[m_len];
    }

    bool dynamic_string::empty() const {
        return m_len == 0;
    }

    char &dynamic_string::front() {
        return m_buffer[0];
    }

    const char &dynamic_string::front() const {
        return m_buffer[0];
    }

    char &dynamic_string::back() {
        return empty() ? m_buffer[0] : m_buffer[m_len - 1];
    }

    const char &dynamic_string::back() const {
        return empty() ? m_buffer[0] : m_buffer[m_len - 1];
    }

    dynamic_string &dynamic_string::operator+=(char c) {
        const char array[2] = {c, '\0'};
        return append(array, 1);
    }

    dynamic_string &dynamic_string::operator+=(const char *val) {
        return append(val, static_cast<size_type>(strlen(val)));
    }

    dynamic_string &dynamic_string::operator+=(const dynamic_string &other) {
        return append(other.c_str(), other.length());
    }

    dynamic_string &dynamic_string::append(const char *c_str, size_type len) {
        auto newLength = static_cast<size_type>(m_len + len);
        m_buffer = realloc<char>(m_buffer, static_cast<size_type>(newLength + 1));

        memcpy(m_buffer + m_len, c_str, newLength);

        m_buffer[newLength] = '\0';
        m_len = newLength;

        return *this;
    }

    dynamic_string &dynamic_string::append(const char *str) {
        return append(str, static_cast<size_type>(strlen(str)));
    }

    dynamic_string &dynamic_string::append(const dynamic_string &str) {
        return append(str.c_str(), str.length());
    }

    void dynamic_string::push_back(const char c) {
        const char array[2] = {c, '\0'};
        append(array, 1);
    }

    void dynamic_string::erase(size_type pos) {
        if (m_len == 0 || pos >= m_len) { return; }
        m_len--;
        memmove(m_buffer + pos, m_buffer + pos + 1, m_len - pos);
        m_buffer[m_len] = '\0';
    }

    void dynamic_string::pop_back() {
        if (m_len != 0) {
            m_buffer[m_len - 1] = '\0';
            m_len--;
        }
    }

    const char *dynamic_string::c_str() const {
        return m_buffer;
    }

    dynamic_string dynamic_string::substr(size_type pos, size_type length) const {
        char *newBuffer = malloc<char[]>(static_cast<size_type>(length + 1));
        memcpy(newBuffer, m_buffer + pos, length);
        newBuffer[length] = '\0';
        return {length, newBuffer};
    }

    diff_type dynamic_string::compare(const dynamic_string &str) const {
        return compare(str.c_str());
    }

    diff_type dynamic_string::compare(const char *str) const {
        return static_cast<diff_type>(strcmp(c_str(), str));
    }

    diff_type dynamic_string::compare(char c) const {
        const char array[2] = {c, '\0'};
        return static_cast<diff_type>(strcmp(c_str(), array));
    }

    bool operator==(const dynamic_string &lhs, const dynamic_string &rhs) {
        return lhs.compare(rhs) == 0;
    }

    bool operator!=(const dynamic_string &lhs, const dynamic_string &rhs) {
        return lhs.compare(rhs) != 0;
    }

    bool operator>(const dynamic_string &lhs, const dynamic_string &rhs) {
        return lhs.compare(rhs) > 0;
    }

    bool operator>=(const dynamic_string &lhs, const dynamic_string &rhs) {
        return lhs.compare(rhs) >= 0;
    }

    bool operator<(const dynamic_string &lhs, const dynamic_string &rhs) {
        return lhs.compare(rhs) < 0;
    }

    bool operator<=(const dynamic_string &lhs, const dynamic_string &rhs) {
        return lhs.compare(rhs) <= 0;
    }

    bool operator==(const char *lhs, const dynamic_string &rhs) {
        return rhs.compare(lhs) == 0;
    }

    bool operator!=(const char *lhs, const dynamic_string &rhs) {
        return rhs.compare(lhs) != 0;
    }

    bool operator>(const char *lhs, const dynamic_string &rhs) {
        return rhs.compare(lhs) <= 0;
    }

    bool operator>=(const char *lhs, const dynamic_string &rhs) {
        return rhs.compare(lhs) < 0;
    }

    bool operator<(const char *lhs, const dynamic_string &rhs) {
        return rhs.compare(lhs) >= 0;
    }

    bool operator<=(const char *lhs, const dynamic_string &rhs) {
        return rhs.compare(lhs) > 0;
    }

    bool operator==(const dynamic_string &lhs, const char *rhs) {
        return lhs.compare(rhs) == 0;
    }

    bool operator!=(const dynamic_string &lhs, const char *rhs) {
        return lhs.compare(rhs) != 0;
    }

    bool operator>(const dynamic_string &lhs, const char *rhs) {
        return lhs.compare(rhs) > 0;
    }

    bool operator>=(const dynamic_string &lhs, const char *rhs) {
        return lhs.compare(rhs) >= 0;
    }

    bool operator<(const dynamic_string &lhs, const char *rhs) {
        return lhs.compare(rhs) < 0;
    }

    bool operator<=(const dynamic_string &lhs, const char *rhs) {
        return lhs.compare(rhs) <= 0;
    }

    bool operator==(char lhs, const dynamic_string &rhs) {
        return rhs.compare(lhs) == 0;
    }

    bool operator==(const dynamic_string &lhs, char rhs) {
        return lhs.compare(rhs) == 0;
    }

    dynamic_string operator+(const dynamic_string &lhs, const dynamic_string &rhs) {
        return {lhs.c_str(), rhs.c_str(), lhs.length(), rhs.length()};
    }

    dynamic_string operator+(const char *lhs, const dynamic_string &rhs) {
        return {lhs, rhs.c_str(), static_cast<size_type>(strlen(lhs)), rhs.length()};
    }

    dynamic_string operator+(const dynamic_string &lhs, const char *rhs) {
        return {lhs.c_str(), rhs, lhs.length(), static_cast<size_type>(strlen(rhs))};
    }

    dynamic_string operator+(char lhs, const dynamic_string &rhs) {
        const char temp[2] = {lhs, '\0'};
        return {temp, rhs.c_str(), 1, rhs.length()};
    }

    dynamic_string operator+(const dynamic_string &lhs, char rhs) {
        const char temp[2] = {rhs, '\0'};
        return {lhs.c_str(), temp, lhs.length(), 1};
    }
}

