/**
 * @file TypeTraits.h
 * @brief Metafunctions for types.
 *
 * Works in conjunction with @code Concept.h @endcode to facilitate
 * C++ polymorphism.
 *
 * @author Jeff Niu
 * @date November 9, 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDCPLUSPLUS_TYPETRAITS_H
#define EMBEDDEDCPLUSPLUS_TYPETRAITS_H

#include "wlib/utility/Tmp.h"

/**
 * Code generation macro to create a metafunction
 * that verifies whether a type has the specified
 * typedef.
 */
#define __WLIB_HAS_TYPE(TypeName) \
template<typename C> \
struct has_##TypeName { \
private: \
    template<typename T> \
    static constexpr typename T::TypeName test(char); \
    template<typename> \
    static constexpr void test(...); \
public: \
    static constexpr bool value = !is_void<decltype(test<C>(0))>::value; \
};

/**
 * Code generation macro to create a metafunction
 * that obtains the type of the specified kind
 * in a class.
 */
#define __WLIB_OBTAIN_TYPE(TypeName) \
template<typename C, typename = typename enable_if< \
    has_##TypeName<C>::value \
>::type> \
struct obtain_##TypeName { \
    typedef typename C::TypeName type; \
};

namespace wlp {

    __WLIB_HAS_TYPE(size_type)
    __WLIB_HAS_TYPE(val_type)
    __WLIB_HAS_TYPE(key_type)
    __WLIB_HAS_TYPE(iterator)
    __WLIB_HAS_TYPE(const_iterator)
    __WLIB_HAS_TYPE(map_type)
    __WLIB_HAS_TYPE(node_type)
    __WLIB_HAS_TYPE(diff_type)

    __WLIB_OBTAIN_TYPE(size_type)
    __WLIB_OBTAIN_TYPE(val_type)
    __WLIB_OBTAIN_TYPE(key_type)
    __WLIB_OBTAIN_TYPE(iterator)
    __WLIB_OBTAIN_TYPE(const_iterator)
    __WLIB_OBTAIN_TYPE(map_type)
    __WLIB_OBTAIN_TYPE(node_type)
    __WLIB_OBTAIN_TYPE(diff_type)

}

#endif //EMBEDDEDCPLUSPLUS_TYPETRAITS_H
