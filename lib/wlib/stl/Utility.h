#ifndef EMBEDDEDCPLUSPLUS_UTILITY_H
#define EMBEDDEDCPLUSPLUS_UTILITY_H

#include "Tmp.h"

namespace wlp {

    /**
     * Move function for move semantics. Transforms a universal type
     * to an rvalue.
     * @tparam T universal type
     * @param t type value
     * @return rvalue of the type
     */
    template<typename T>
    constexpr typename remove_reference<T>::type &&move(T &&t) noexcept {
        return static_cast<typename remove_reference<T>::type &&>(t);
    }

    /**
     * Forward consumes either an lvalue and rvalue and returns a universal
     * rvalue type that maintains its properties. This overloads for lvalues.
     * @tparam T lvalue type
     * @param t type value
     * @return rvalue capture
     */
    template<typename T>
    constexpr T &&forward(typename remove_reference<T>::type &t) noexcept {
        return static_cast<T &&>(t);
    }

    /**
     * Overloads for rvalues.
     * @tparam T rvalue type
     * @param t type value
     * @return rvalue capture
     */
    template<typename T>
    constexpr T &&forward(typename remove_reference<T>::type &&t) noexcept {
        return static_cast<T &&>(t);
    }

    template<typename T>
    void swap(T &v1, T &v2) {
        T tmp(move(v1));
        v1 = move(v2);
        v2 = move(tmp);
    }

}

#endif //EMBEDDEDCPLUSPLUS_UTILITY_H
