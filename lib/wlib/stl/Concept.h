#ifndef EMBEDDEDCPLUSPLUS_CONCEPTCHECKS_H
#define EMBEDDEDCPLUSPLUS_CONCEPTCHECKS_H

#include "Tmp.h"
#include "TypeTraits.h"

#include "../Wlib.h"

#define __HAS_FCN(...) VFUNC(__HAS_FCN, __VA_ARGS__)
#define __HAS_FCN3(TypeName, FcnName, RetType) \
    is_same<decltype(declval<TypeName>().FcnName( \
    )), RetType>
#define __HAS_FCN4(TypeName, FcnName, ArgType1, RetType) \
    is_same<decltype(declval<TypeName>().FcnName( \
        declval<ArgType1>()  \
    )), RetType>
#define __HAS_FCN5(TypeName, FcnName, ArgType1, ArgType2, RetType) \
    is_same<decltype(declval<TypeName>().FcnName( \
        declval<ArgType1>(), \
        declval<ArgType2>()  \
    )), RetType>
#define __HAS_FCN6(TypeName, FcnName, ArgType1, ArgType2, ArgType3, RetType) \
    is_same<decltype(declval<TypeName>().FcnName( \
        declval<ArgType1>(), \
        declval<ArgType2>(), \
        declval<ArgType3>()  \
    )), RetType>
#define __HAS_FCN7(TypeName, FcnName, ArgType1, ArgType2, ArgType3, ArgType4, RetType) \
    is_same<decltype(declval<TypeName>().FcnName( \
        declval<ArgType1>(), \
        declval<ArgType2>(), \
        declval<ArgType3>(), \
        declval<ArgType4>()  \
    )), RetType>
#define __HAS_FCN8(TypeName, FcnName, ArgType1, ArgType2, ArgType3, ArgType4, ArgType5, RetType) \
    is_same<decltype(declval<TypeName>().FcnName( \
        declval<ArgType1>(), \
        declval<ArgType2>(), \
        declval<ArgType3>(), \
        declval<ArgType4>(), \
        declval,ArgType5>()  \
    )), RetType>

namespace wlp {

    template<typename C, typename CmpType>
    struct comparator_concept {
    private:
        template<typename T, typename Q>
        static constexpr auto check(T *) -> typename and_<
                __HAS_FCN(T, __lt__, const Q &, const Q &, bool),
                __HAS_FCN(T, __le__, const Q &, const Q &, bool),
                __HAS_FCN(T, __eq__, const Q &, const Q &, bool),
                __HAS_FCN(T, __ne__, const Q &, const Q &, bool),
                __HAS_FCN(T, __gt__, const Q &, const Q &, bool),
                __HAS_FCN(T, __ge__, const Q &, const Q &, bool)
        >::type;

        template<typename, typename>
        static constexpr false_type check(...);

        typedef decltype(check<C, CmpType>(nullptr)) type;

    public:
        static constexpr bool value = type::value;
    };

    template<typename C, typename T>
    static constexpr bool is_comparator() {
        return comparator_concept<C, T>::value;
    }

    template<typename C, bool = has_size_type<C>::value && has_val_type<C>::value>
    struct forward_iterator_concept {
        static constexpr bool value = false;
    };

    template<typename C>
    struct forward_iterator_concept<C, true> {
    private:
        typedef typename C::val_type val_type;
        typedef typename C::size_type size_type;
        typedef C iterator;

        template<typename T>
        static constexpr auto check(T *) -> typename and_<
                typename or_<
                        __HAS_FCN(T, operator*, val_type &),
                        __HAS_FCN(T, operator*, const val_type &)
                >::type,
                typename or_<
                        __HAS_FCN(T, operator->, val_type *),
                        __HAS_FCN(T, operator->, const val_type *)
                >::type,
                __HAS_FCN(T, operator++, iterator &),
                __HAS_FCN(T, operator++, int, iterator),
                __HAS_FCN(T, operator==, iterator &, bool),
                __HAS_FCN(T, operator==, const iterator &, bool),
                __HAS_FCN(T, operator!=, iterator &, bool),
                __HAS_FCN(T, operator!=, const iterator &, bool),
                __HAS_FCN(T, operator=, const iterator &, iterator &),
                __HAS_FCN(T, operator=, iterator &&, iterator &)
        >::type;


        template<typename>
        static constexpr false_type check(...);

        typedef decltype(check<C>(nullptr)) type;

    public:
        static constexpr bool value = type::value;
    };

    template<typename C>
    static constexpr bool is_forward_iterator() {
        return forward_iterator_concept<C>::value;
    }

    template<typename C, bool = has_size_type<C>::value && has_val_type<C>::value>
    struct random_access_iterator_concept {
        static constexpr bool value = false;
    };

    template<typename C>
    struct random_access_iterator_concept<C, true> {
    private:
        typedef typename C::val_type val_type;
        typedef typename C::size_type size_type;
        typedef C iterator;

        template<typename T>
        static constexpr auto check(T *) -> typename and_<
                typename or_<
                        __HAS_FCN(T, operator*, val_type &),
                        __HAS_FCN(T, operator*, const val_type &)
                >::type,
                typename or_<
                        __HAS_FCN(T, operator->, val_type *),
                        __HAS_FCN(T, operator->, const val_type *)
                >::type,
                __HAS_FCN(T, operator++, iterator &),
                __HAS_FCN(T, operator++, int, iterator),
                __HAS_FCN(T, operator--, iterator &),
                __HAS_FCN(T, operator--, int, iterator),
                __HAS_FCN(T, operator==, iterator &, bool),
                __HAS_FCN(T, operator==, const iterator &, bool),
                __HAS_FCN(T, operator!=, iterator &, bool),
                __HAS_FCN(T, operator!=, iterator &, bool),
                __HAS_FCN(T, operator=, iterator &&, iterator &),
                __HAS_FCN(T, operator=, const iterator &, iterator &),
                __HAS_FCN(T, operator+, const size_type &, iterator),
                __HAS_FCN(T, operator+, size_type &&, iterator),
                __HAS_FCN(T, operator-, const size_type &, iterator),
                __HAS_FCN(T, operator-, size_type &&, iterator),
                __HAS_FCN(T, operator-, const iterator &, size_type),
                __HAS_FCN(T, operator-, iterator &&, size_type),
                __HAS_FCN(T, operator-=, const size_type &, iterator &),
                __HAS_FCN(T, operator-=, size_type &&, iterator &),
                __HAS_FCN(T, operator+=, const size_type &, iterator &),
                __HAS_FCN(T, operator+=, size_type &&, iterator &)
        >::type;

        template<typename>
        static constexpr false_type check(...);

        typedef decltype(check<C>(nullptr)) type;

    public:
        static constexpr bool value = type::value;
    };

    template<typename C>
    static constexpr bool is_random_access_iterator() {
        return random_access_iterator_concept<C>::value;
    }

    template<typename C, bool =
        has_key_type<C>::value &&
        has_val_type<C>::value &&
        has_size_type<C>::value &&
        has_iterator<C>::value &&
        has_const_iterator<C>::value
    >
    struct map_concept {
        static constexpr bool value = false;
    };

}

#endif //EMBEDDEDCPLUSPLUS_CONCEPTCHECKS_H
