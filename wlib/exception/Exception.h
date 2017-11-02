/**
 * @file Exception.h
 * @brief Custom exception types for WLib.
 *
 * @author Jeff Niu
 * @date November 2, 2017
 * @bug No known bugs
 */

/*
 * TODO: Use dynamic string type once implemented
 * TODO: Define RangeException
 */

#ifndef EMBEDDEDTESTS_EXCEPTION_H
#define EMBEDDEDTESTS_EXCEPTION_H

#include <stdint.h>
#include <strings/StaticString.h>

namespace wlp {

    /**
     * Base exception class from which other exceptions derive.
     * @tparam tSize size of the static string message
     */
    class Exception {
    public:
        typedef const char* msg_type;

        Exception() throw() = default;
        Exception(const Exception& cause) throw() = default;
        Exception& operator=(const Exception& e) throw() {}
        virtual ~Exception() throw() = default;
        virtual msg_type what() const throw() {}
    };

    class RangeException : Exception {
    };

    /**
     * A KeyException is thrown in certain situations when a key-type value
     * fails to map to a value in a Map data structure.
     * @tparam Key key type
     */
    template<class Key>
    class KeyException : Exception {
    public:
        typedef Key key_type;
        typedef const char* msg_type;
        key_type m_err_key;

        explicit KeyException(const key_type& key) throw() : m_err_key(key) {}
        msg_type what() const throw() override {
            return "Bad key";
        }
        const key_type& key() const throw() {
            return m_err_key;
        }
    };

}

#endif //EMBEDDEDTESTS_EXCEPTION_H
