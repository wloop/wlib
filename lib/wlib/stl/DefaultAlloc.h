#ifndef EMBEDDEDCPLUSPLUS_DEFAULTALLOC_H
#define EMBEDDEDCPLUSPLUS_DEFAULTALLOC_H

#include "../memory/Memory.h"

namespace wlp {

    /**
     * Default deleter uses @code free @endcode from Memory.
     *
     * @tparam T type to delete
     */
    template<typename T>
    struct DefaultDeleter {
        DefaultDeleter() {}

        template<typename U>
        DefaultDeleter(const DefaultDeleter<U> &) {}

        void operator()(T *ptr) const {
            static_assert(sizeof(T) > 0, "Cannot delete pointer to incomplete type");
            free<T>(ptr);
        }
    };

    /**
     * Default array deleter uses @code free @endcode from Memory.
     *
     * @tparam T type whose array to delete
     */
    template<typename T>
    struct DefaultDeleter<T[]> {
        DefaultDeleter() {}

        template<typename U>
        DefaultDeleter(const DefaultDeleter<U> &) {}

        void operator()(T *ptr) const {
            static_assert(sizeof(T) > 0, "Cannot delete pointer to incomplete type");
            free<T>(ptr);
        }
    };


    template<typename T>
    struct DefaultAlloc {
        DefaultAlloc() {}

        template<typename U>
        DefaultAlloc(const DefaultAlloc<U> &) {}

        T *operator()(size_type num = 1) const {
            return malloc<T>(num);
        }

        void deallocate(T *ptr, size_type num = 1) const {
            free<T>(ptr);
        }
    };

    template<typename T>
    struct DefaultAlloc<T[]> {
        DefaultAlloc() {}

        template<typename U>
        DefaultAlloc(const DefaultAlloc<U> &) {}

        T *operator()(size_type num = 1) const {
            return malloc<T>(num);
        }

        void deallocate(T *ptr, size_type num = 1) const {
            return free<T>(ptr);
        }
    };

}

#endif //EMBEDDEDCPLUSPLUS_DEFAULTALLOC_H
