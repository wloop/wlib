/**
 * @file UniquePtr.h
 * @brief Unique pointer implementation
 *
 * Implementation of a unique pointer class, a smart pointer which
 * manages a single pointer and cannot share with another unique pointer.
 *
 * @author Jeff Niu
 * @date November 22, 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDCPLUSPLUS_UNIQUEPTR_H
#define EMBEDDEDCPLUSPLUS_UNIQUEPTR_H

#include <wlib/stl/Helper.h>
#include <wlib/tmp/Convertible.h>
#include <wlib/type_traits>
#include <wlib/memory>

namespace wlp {

    /**
     * A unique pointer is a smart pointer that handles exactly one pointer
     * that cannot and should not at any moment be shared with another class.
     *
     * This class is an object that wraps a usual pointer. This means that
     * the object cannot escape local variable scopes. To escape a pointer from
     * the local variable scope, @code release @endcode can be called to
     * remove ownership of the pointer.
     *
     * The unique pointer automatically handles the deletion of the pointer
     * and associated resources in its destructor when the class reaches the
     * end of scope.
     *
     * Supplying a non-dynamically allocated pointer or sharing a provided
     * pointer with different unique pointer leads to undefined behaviour, and
     * with Memory, segmentation faults.
     *
     * Construction is recommended with @code make_unique @endcode.
     * 
     * Unique pointer class does not currently support multiple deleter
     * types, and uses only functions in Memory. Adding deleters for other
     * Memory solutions can be a future feature, but makes the class more complex.
     *
     * @tparam T the type pointed to by this unique pointer
     * @tparam T_Deleter deleter type used to free the pointer
     */
    template<typename T>
    class unique_ptr {
        typedef T *unique_ptr::* unspecified_bool_type;
        typedef T *unique_ptr::* unspecified_pointer_type;
        typedef unique_ptr<T> unique_ptr_t;

    public:
        typedef T *pointer;
        typedef T val_type;

    private:
        val_type *m_ptr;

    public:

        unique_ptr()
                : m_ptr(pointer()) {
        }

        explicit
        unique_ptr(pointer ptr)
                : m_ptr(ptr) {
        }


        unique_ptr(unique_ptr_t &&ptr)
                : m_ptr(ptr.release()) {
        }

        template<typename U>
        unique_ptr(unique_ptr<U> &&ptr)
                : m_ptr(ptr.release()) {
        };

        ~unique_ptr() {
            reset();
        }

        unique_ptr_t &operator=(unique_ptr_t &&ptr) {
            reset(ptr.release());
            return *this;
        }

        template<typename U>
        unique_ptr_t &operator=(unique_ptr<U> &&ptr) {
            reset(ptr.release());
            return *this;
        };

        unique_ptr_t &operator=(unspecified_pointer_type) {
            reset();
            return *this;
        }

        typename add_lvalue_reference<val_type>::type operator*() const {
            return *m_ptr;
        };

        pointer operator->() const {
            return m_ptr;
        }

        pointer get() const {
            return m_ptr;
        }


        operator unspecified_bool_type() const {
            return m_ptr == nullptr ? 0 : &unique_ptr::m_ptr;
        }

        pointer release() {
            pointer ptr = m_ptr;
            m_ptr = nullptr;
            return ptr;
        }

        void reset(pointer ptr = pointer()) {
            if (ptr != m_ptr) {
                destroy<val_type>(m_ptr);
                m_ptr = ptr;
            }
        }

        void swap(unique_ptr_t &&ptr) {
            wlp::swap(m_ptr, ptr.m_ptr);
        }

    private:
        unique_ptr(const unique_ptr_t &) = delete;

        template<typename U>
        unique_ptr(const unique_ptr<U> &) = delete;

        unique_ptr_t &operator=(const unique_ptr_t &) = delete;

        template<typename U>
        unique_ptr_t &operator=(const unique_ptr<U> &) = delete;

    };

    template<typename T>
    class unique_ptr<T[]> {
        typedef T *unique_ptr::* unspecified_bool_type;
        typedef T *unique_ptr::* unspecified_pointer_type;
        typedef unique_ptr<T> unique_ptr_t;

    public:
        typedef T *pointer;
        typedef T val_type;

    private:
        val_type *m_ptr;

    public:
        unique_ptr()
                : m_ptr(pointer()) {
        }

        explicit
        unique_ptr(pointer ptr)
                : m_ptr(ptr) {
        }

        ~unique_ptr() {
            reset();
        }

        unique_ptr_t &operator=(unique_ptr_t &&ptr) {
            reset(ptr.release());
            return *this;
        }

        template<typename U>
        unique_ptr_t &operator=(unique_ptr<U> &&ptr) {
            reset(ptr.release());
            return *this;
        };

        unique_ptr_t &operator=(unspecified_pointer_type) {
            reset();
            return *this;
        }

        typename add_lvalue_reference<val_type>::type operator[](size_t i) const {
            return m_ptr[i];
        }

        pointer get() const {
            return m_ptr;
        }

        operator unspecified_bool_type() const {
            return m_ptr == nullptr ? 0 : &unique_ptr::m_ptr;
        };

        pointer release() {
            pointer ptr = m_ptr;
            m_ptr = nullptr;
            return ptr;
        }

        void reset(pointer ptr = pointer()) {
            if (ptr != m_ptr) {
                destroy<val_type[]>(m_ptr);
                m_ptr = ptr;
            }
        }

        template<typename U>
        void reset(U) = delete;

        void swap(unique_ptr_t &&u) {
            wlp::swap(m_ptr, u.m_ptr);
        }

    private:
        unique_ptr(const unique_ptr_t &) = delete;

        template<typename U>
        explicit
        unique_ptr(U *, typename enable_if<
            is_convertible<U *, pointer>::value
        >::type * = 0) = delete;

        unique_ptr_t &operator=(const unique_ptr_t &) = delete;

        template<typename U, typename U_Deleter>
        unique_ptr_t &operator=(const unique_ptr<U> &) = delete;
    };

    template<typename T>
    inline void swap(unique_ptr<T> &x, unique_ptr<T> &y) {
        x.swap(y);
    };

    template<typename T>
    inline void swap(unique_ptr<T> &&x, unique_ptr<T> &y) {
        x.swap(y);
    };

    template<typename T>
    inline void swap(unique_ptr<T> &x, unique_ptr<T> &&y) {
        x.swap(y);
    }

    template<typename T, typename U>
    inline bool operator==(const unique_ptr<T> &x, const unique_ptr<U> &y) {
        return x.get() == y.get();
    }

    template<typename T, typename U>
    inline bool operator!=(const unique_ptr<T> &x, const unique_ptr<U> &y) {
        return !(x.get() == y.get());
    }

    template<typename T, typename U>
    inline bool operator<(const unique_ptr<T> &x, const unique_ptr<U> &y) {
        return x.get() < y.get();
    }

    template<typename T, typename U>
    inline bool operator<=(const unique_ptr<T> &x, const unique_ptr<U> &y) {
        return !(y.get() < x.get());
    }

    template<typename T, typename U>
    inline bool operator>(const unique_ptr<T> &x, const unique_ptr<U> &y) {
        return y.get() < x.get();
    }

    template<typename T, typename U>
    inline bool operator>=(const unique_ptr<T> &x, const unique_ptr<U> &y) {
        return !(x.get() < y.get());
    }

    template<typename T, typename... Args>
    unique_ptr<T> make_unique(Args &&... args) {
        return unique_ptr<T>(create<T>(forward<Args>(args)...));
    };

};

#endif //EMBEDDEDCPLUSPLUS_UNIQUEPTR_H
