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

#include "../memory/Memory.h"
#include "../exceptions/Exceptions.h"

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
    class UniquePtr {
        typedef T *UniquePtr::* unspecified_bool_type;
        typedef T *UniquePtr::* unspecified_pointer_type;
        typedef UniquePtr<T> unique_ptr;

    public:
        typedef T *pointer;
        typedef T val_type;

    private:
        val_type *m_ptr;

    public:

        UniquePtr()
                : m_ptr(pointer()) {
        }

        explicit
        UniquePtr(pointer ptr)
                : m_ptr(ptr) {
        }


        UniquePtr(unique_ptr &&ptr)
                : m_ptr(ptr.release()) {
        }

        template<typename U>
        UniquePtr(UniquePtr<U> &&ptr)
                : m_ptr(ptr.release()) {
        };

        ~UniquePtr() {
            reset();
        }

        unique_ptr &operator=(unique_ptr &&ptr) {
            reset(ptr.release());
            return *this;
        }

        template<typename U>
        unique_ptr &operator=(UniquePtr<U> &&ptr) {
            reset(ptr.release());
            return *this;
        };

        unique_ptr &operator=(unspecified_pointer_type) {
            reset();
            return *this;
        }

        typename add_lvalue_reference<val_type>::type operator*() const {
            if (m_ptr == nullptr) {
                THROW(NULLPTR_EXCEPTION("Unique pointer is null"))
            }
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
                free<val_type>(m_ptr);
                m_ptr = ptr;
            }
        }

        void swap(unique_ptr &&ptr) {
            wlp::swap(m_ptr, ptr.m_ptr);
        }

    private:
        UniquePtr(const unique_ptr &) = delete;

        template<typename U>
        UniquePtr(const UniquePtr<U> &) = delete;

        unique_ptr &operator=(const unique_ptr &) = delete;

        template<typename U>
        unique_ptr &operator=(const UniquePtr<U> &) = delete;

    };

    template<typename T>
    class UniquePtr<T[]> {
        typedef T *UniquePtr::* unspecified_bool_type;
        typedef T *UniquePtr::* unspecified_pointer_type;
        typedef UniquePtr<T> unique_ptr;

    public:
        typedef T *pointer;
        typedef T val_type;

    private:
        val_type *m_ptr;

    public:
        UniquePtr()
                : m_ptr(pointer()) {
        }

        explicit
        UniquePtr(pointer ptr)
                : m_ptr(ptr) {
        }

        ~UniquePtr() {
            reset();
        }

        unique_ptr &operator=(unique_ptr &&ptr) {
            reset(ptr.release());
            return *this;
        }

        template<typename U>
        unique_ptr &operator=(UniquePtr<U> &&ptr) {
            reset(ptr.release());
            return *this;
        };

        unique_ptr &operator=(unspecified_pointer_type) {
            reset();
            return *this;
        }

        typename add_lvalue_reference<val_type>::type operator[](size_type i) const {
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
                free<val_type>(m_ptr);
                m_ptr = ptr;
            }
        }

        template<typename U>
        void reset(U) = delete;

        void swap(unique_ptr &&u) {
            wlp::swap(m_ptr, u.m_ptr);
        }

    private:
        UniquePtr(const unique_ptr &) = delete;

        template<typename U>
        explicit
        UniquePtr(U *,
                  typename enable_if<
                          is_convertible<U *, pointer>::value
                  >::type * = 0
        ) = delete;

        unique_ptr &operator=(const unique_ptr &) = delete;

        template<typename U, typename U_Deleter>
        unique_ptr &operator=(const UniquePtr<U> &) = delete;
    };

    template<typename T>
    inline void swap(UniquePtr<T> &x, UniquePtr<T> &y) {
        x.swap(y);
    };

    template<typename T>
    inline void swap(UniquePtr<T> &&x, UniquePtr<T> &y) {
        x.swap(y);
    };

    template<typename T>
    inline void swap(UniquePtr<T> &x, UniquePtr<T> &&y) {
        x.swap(y);
    }

    template<typename T, typename U>
    inline bool operator==(const UniquePtr<T> &x, const UniquePtr<U> &y) {
        return x.get() == y.get();
    }

    template<typename T, typename U>
    inline bool operator!=(const UniquePtr<T> &x, const UniquePtr<U> &y) {
        return !(x.get() == y.get());
    }

    template<typename T, typename U>
    inline bool operator<(const UniquePtr<T> &x, const UniquePtr<U> &y) {
        return x.get() < y.get();
    }

    template<typename T, typename U>
    inline bool operator<=(const UniquePtr<T> &x, const UniquePtr<U> &y) {
        return !(y.get() < x.get());
    }

    template<typename T, typename U>
    inline bool operator>(const UniquePtr<T> &x, const UniquePtr<U> &y) {
        return y.get() < x.get();
    }

    template<typename T, typename U>
    inline bool operator>=(const UniquePtr<T> &x, const UniquePtr<U> &y) {
        return !(x.get() < y.get());
    }

    template<typename T, typename... Args>
    UniquePtr<T> make_unique(Args &&... args) {
        return UniquePtr<T>(malloc<T>(forward<Args>(args)...));
    };

};

#endif //EMBEDDEDCPLUSPLUS_UNIQUEPTR_H
