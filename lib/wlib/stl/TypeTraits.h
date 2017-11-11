#ifndef EMBEDDEDCPLUSPLUS_TYPETRAITS_H
#define EMBEDDEDCPLUSPLUS_TYPETRAITS_H

#include "Tmp.h"

namespace wlp {

    template<class C>
    struct obtain_val_type_ptr {
        static const typename C::val_type *type;
    };

    template<class C>
    struct obtain_size_type_ptr {
        static const typename C::size_type *type;
    };

    template<class C>
    struct obtain_val_type {
        typedef typename C::val_type type;
    };

    template<class C>
    struct obtain_size_type {
        typedef typename C::size_type type;
    };

    template<typename C>
    struct has_size_type {
    private:
        template<typename T>
        static constexpr typename T::size_type test(int);

        template<typename>
        static constexpr void test(...);

    public:
        static constexpr bool value = !is_void<decltype(test<C>(0))>::value;
    };

    template<typename C>
    struct has_val_type {
    private:
        template<typename T>
        static constexpr typename T::val_type test(int);

        template<typename>
        static constexpr void test(...);

    public:
        static constexpr bool value = !is_void<decltype(test<C>(0))>::value;
    };

}

#endif //EMBEDDEDCPLUSPLUS_TYPETRAITS_H
