/**
 * @file UniquePtr.h
 * @brief
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

#include "Tuple.h"
#include "DefaultAlloc.h"

#include "../memory/Memory.h"

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
     * @tparam T the type pointed to by this unique pointer
     * @tparam T_Deleter deleter type used to free the pointer
     */
    template<typename T, typename T_Deleter = DefaultDeleter<T>>
    class UniquePtr {
        typedef Tuple<T *, T_Deleter> tuple_type;
        typedef tuple_type UniquePtr::* unspecified_bool_type;
        typedef T *UniquePtr::* unspecified_pointer_type;
        typedef UniquePtr<T, T_Deleter> unique_ptr;

    public:
        typedef T *pointer;
        typedef T val_type;
        typedef T_Deleter deleter_type;

    private:
        tuple_type m_ptr;

    public:

        UniquePtr()
                : m_ptr(pointer(), deleter_type()) {
        }

        explicit
        UniquePtr(pointer ptr)
                : m_ptr(ptr, deleter_type()) {
        }

        UniquePtr(pointer ptr, typename conditional<
                is_reference<deleter_type>::value,
                deleter_type,
                const deleter_type &>
        ::type deleter)
                : m_ptr(ptr, deleter) {
        }

        UniquePtr(pointer ptr, typename remove_reference<deleter_type>::type &&deleter)
                : m_ptr(move(ptr), move(deleter)) {
        }

        UniquePtr(unique_ptr &&ptr)
                : m_ptr(ptr.release(), forward<deleter_type>(ptr.get_deleter())) {
        }

        template<typename U, typename U_Deleter>
        UniquePtr(UniquePtr<U, U_Deleter> &&ptr)
                : m_ptr(ptr.release(), forward<deleter_type>(ptr.get_deleter())) {
        };

        ~UniquePtr() {
            reset();
        }

        unique_ptr &operator=(unique_ptr &&ptr) {
            reset(ptr.release());
            get_deleter() = move(ptr.get_deleter());
            return *this;
        }

        template<typename U, typename U_Deleter>
        unique_ptr &operator=(UniquePtr<U, U_Deleter> &&ptr) {
            reset(ptr.release());
            get_deleter() = move(ptr.get_deleter());
            return *this;
        };

        unique_ptr &operator=(unspecified_pointer_type) {
            reset();
            return *this;
        }

        typename add_lvalue_reference<val_type>::type operator*() const {
            return *get();
        };

        pointer operator->() const {
            return get();
        }

        pointer get() const {
            return wlp::get<0>(m_ptr);
        }

        typename add_lvalue_reference<deleter_type>::type get_deleter() {
            return wlp::get<1>(m_ptr);
        }

        typename add_lvalue_reference<
                typename add_const<deleter_type>::type
        >::type get_deleter() const {
            return wlp::get<1>(m_ptr);
        }

        operator unspecified_bool_type() const {
            return get() == nullptr ? 0 : &unique_ptr::m_ptr;
        }

        pointer release() {
            pointer ptr = get();
            wlp::get<0>(m_ptr) = nullptr;
            return ptr;
        }

        void reset(pointer ptr = pointer()) {
            if (ptr != get()) {
                get_deleter()(get());
                wlp::get<0>(m_ptr) = ptr;
            }
        }

        void swap(unique_ptr &&ptr) {
            wlp::swap(m_ptr, ptr.m_ptr);
        }

    private:
        UniquePtr(const unique_ptr &) = delete;

        template<typename U, typename U_Deleter>
        UniquePtr(const UniquePtr<U, U_Deleter> &) = delete;

        unique_ptr &operator=(const unique_ptr &) = delete;

        template<typename U, typename U_Deleter>
        unique_ptr &operator=(const UniquePtr<U, U_Deleter> &) = delete;

    };

    template<typename T, typename T_Deleter>
    class UniquePtr<T[], T_Deleter> {
        typedef Tuple<T *, T_Deleter> tuple_type;
        typedef tuple_type UniquePtr::* unspecified_bool_type;
        typedef T *UniquePtr::* unspecified_pointer_type;
        typedef UniquePtr<T, T_Deleter> unique_ptr;

    public:
        typedef T *pointer;
        typedef T val_type;
        typedef T_Deleter deleter_type;

    private:
        tuple_type m_ptr;

    public:
        UniquePtr()
                : m_ptr(pointer(), deleter_type()) {
        }

        explicit
        UniquePtr(pointer ptr)
                : m_ptr(ptr, deleter_type()) {
        }

        UniquePtr(pointer ptr, typename conditional<
                is_reference<deleter_type>::value,
                deleter_type,
                const deleter_type &
        >::type deleter)
                : m_ptr(ptr, deleter) {
        }

        UniquePtr(pointer ptr, typename remove_reference<deleter_type>::type &&deleter)
                : m_ptr(move(ptr), move(deleter)) {
        }

        UniquePtr(unique_ptr &&ptr)
                : m_ptr(ptr.release(), forward<deleter_type>(ptr.get_deleter())) {
        }

        template<typename U, typename U_Deleter>
        UniquePtr(UniquePtr<U, U_Deleter> &&ptr)
                : m_ptr(ptr.release(), forward<deleter_type>(ptr.get_deleter())) {
        };

        ~UniquePtr() {
            reset();
        }

        unique_ptr &operator=(unique_ptr &&ptr) {
            reset(ptr.release());
            get_deleter() = move(ptr.get_deleter());
            return *this;
        }

        template<typename U, typename U_Deleter>
        unique_ptr &operator=(UniquePtr<U, U_Deleter> &&ptr) {
            reset(ptr.release());
            get_deleter() = move(ptr.get_deleter());
            return *this;
        };

        unique_ptr &operator=(unspecified_pointer_type) {
            reset();
            return *this;
        }

        typename add_lvalue_reference<val_type>::type operator[](size_type i) const {
            return get()[i];
        }

        pointer get() const {
            return wlp::get<0>(m_ptr);
        }

        typename add_lvalue_reference<deleter_type>::type get_deleter() {
            return wlp::get<1>(m_ptr);
        }

        typename add_lvalue_reference<
                typename add_const<deleter_type>::type
        >::type get_deleter() const {
            return wlp::get<1>(m_ptr);
        }

        operator unspecified_bool_type() const {
            return get() == nullptr ? 0 : &unique_ptr::m_ptr;
        };

        pointer release() {
            pointer ptr = get();
            wlp::get<0>(m_ptr) = nullptr;
            return ptr;
        }

        void reset(pointer ptr = pointer()) {
            if (ptr != get()) {
                get_deleter()(get());
                wlp::get<0>(m_ptr) = ptr;
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
        UniquePtr(U *,
                  typename conditional<
                          is_reference<deleter_type>::value,
                          deleter_type,
                          const deleter_type &
                  >::type,
                  typename enable_if<
                          is_convertible<U *, pointer>::value
                  >::type * = nullptr
        ) = delete;

        template<typename U>
        UniquePtr(U *,
                  typename remove_reference<deleter_type>::type &&,
                  typename enable_if<
                          is_convertible<U *, pointer>::value
                  >::type * = nullptr
        ) = delete;

        template<typename U>
        explicit
        UniquePtr(U *,
                  typename enable_if<
                          is_convertible<U *, pointer>::value
                  >::type * = nullptr
        ) = delete;

        unique_ptr &operator=(const unique_ptr &) = delete;

        template<typename U, typename U_Deleter>
        unique_ptr &operator=(const UniquePtr<U, U_Deleter> &) = delete;
    };

    template<typename T, typename T_Deleter>
    inline void swap(UniquePtr<T, T_Deleter> &x, UniquePtr<T, T_Deleter> &y) {
        x.swap(y);
    };

    template<typename T, typename T_Deleter>
    inline void swap(UniquePtr<T, T_Deleter> &&x, UniquePtr<T, T_Deleter> &y) {
        x.swap(y);
    };

    template<typename T, typename T_Deleter>
    inline void swap(UniquePtr<T, T_Deleter> &x, UniquePtr<T, T_Deleter> &&y) {
        x.swap(y);
    }

    template<typename T, typename T_Deleter, typename U, typename U_Deleter>
    inline bool operator==(const UniquePtr<T, T_Deleter> &x, const UniquePtr<U, U_Deleter> &y) {
        return x.get() == y.get();
    }

    template<typename T, typename T_Deleter, typename U, typename U_Deleter>
    inline bool operator!=(const UniquePtr<T, T_Deleter> &x, const UniquePtr<U, U_Deleter> &y) {
        return !(x.get() == y.get());
    }

    template<typename T, typename T_Deleter, typename U, typename U_Deleter>
    inline bool operator<(const UniquePtr<T, T_Deleter> &x, const UniquePtr<U, U_Deleter> &y) {
        return x.get() < y.get();
    }

    template<typename T, typename T_Deleter, typename U, typename U_Deleter>
    inline bool operator<=(const UniquePtr<T, T_Deleter> &x, const UniquePtr<U, U_Deleter> &y) {
        return !(y.get() < x.get());
    }

    template<typename T, typename T_Deleter, typename U, typename U_Deleter>
    inline bool operator>(const UniquePtr<T, T_Deleter> &x, const UniquePtr<U, U_Deleter> &y) {
        return y.get() < x.get();
    }

    template<typename T, typename T_Deleter, typename U, typename U_Deleter>
    inline bool operator>=(const UniquePtr<T, T_Deleter> &x, const UniquePtr<U, U_Deleter> &y) {
        return !(x.get() < y.get());
    }

    // TODO waiting on vararg `malloc`
    /*template<typename T, typename... Args>
    UniquePtr<T> make_unique(Args &&... args) {
        return UniquePtr<T>(malloc<T>(forward<Args>(args)...));
    };*/

};

#endif //EMBEDDEDCPLUSPLUS_UNIQUEPTR_H
