/**
 * @file Concept.h
 * @brief Concept definitions.
 *
 * This class is where concept definitions should be located.
 * Concepts facilitate C++ polymorphism in the place of
 * complex inheritance trees.
 *
 * @author Jeff Niu
 * @date November 9, 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDCPLUSPLUS_CONCEPTCHECKS_H
#define EMBEDDEDCPLUSPLUS_CONCEPTCHECKS_H

#include "Pair.h"
#include "Tmp.h"
#include "TypeTraits.h"

#include "../Wlib.h"

/**
 * Definitions for a variadic macro that generates
 * code to verify whether a specified type has a
 * function with a function name, return type, and
 * argument types.
 */
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

    /**
     * Comparator concept defines a type that implements
     * the six rich comparison operators for a type.
     *
     * @tparam C comparator type
     * @tparam CmpType the compared type
     */
    template<typename C, typename CmpType>
    struct comparator_concept {
    private:
        template<typename T, typename Q>
        static constexpr auto check(T *) -> typename and_<
                __HAS_FCN(const T, __lt__, const Q &, const Q &, bool),
                __HAS_FCN(const T, __le__, const Q &, const Q &, bool),
                __HAS_FCN(const T, __eq__, const Q &, const Q &, bool),
                __HAS_FCN(const T, __ne__, const Q &, const Q &, bool),
                __HAS_FCN(const T, __gt__, const Q &, const Q &, bool),
                __HAS_FCN(const T, __ge__, const Q &, const Q &, bool)
        >::type;

        template<typename, typename>
        static constexpr false_type check(...);

        typedef decltype(check<C, CmpType>(nullptr)) type;

    public:
        static constexpr bool value = type::value;
    };

    /**
     * Shorthand method to check whether a class fits
     * the defined comparator concept.
     *
     * @tparam C class to check
     * @tparam T desired comparison type
     * @return true if the class is a comparator
     */
    template<typename C, typename T>
    static constexpr bool is_comparator() {
        return comparator_concept<C, T>::value;
    }

    /**
     * Base case forward iterator is false.
     *
     * @tparam C candidate forward iterator type
     */
    template<typename C, bool = has_size_type<C>::value && has_val_type<C>::value>
    struct forward_iterator_concept {
        static constexpr bool value = false;
    };

    /**
     * Forward iterator concept. Defines any iterator over a
     * data structure that can be moved forwards through the structure,
     * visiting each value in the structure exactly once.
     *
     * @tparam C forward iterator type
     */
    template<typename C>
    struct forward_iterator_concept<C, true> {
    private:
        typedef typename C::val_type val_type;
        typedef typename C::size_type size_type;
        typedef C iterator;

        template<typename T>
        static constexpr auto check(T *) -> typename and_<
                typename or_<
                        __HAS_FCN(const T, operator*, val_type &),
                        __HAS_FCN(const T, operator*, const val_type &)
                >::type,
                typename or_<
                        __HAS_FCN(T, operator->, val_type *),
                        __HAS_FCN(T, operator->, const val_type *)
                >::type,
                __HAS_FCN(T, operator++, iterator &),
                __HAS_FCN(T, operator++, int, iterator),
                __HAS_FCN(const T, operator==, iterator &, bool),
                __HAS_FCN(const T, operator==, const iterator &, bool),
                __HAS_FCN(const T, operator!=, iterator &, bool),
                __HAS_FCN(const T, operator!=, const iterator &, bool),
                __HAS_FCN(T, operator=, const iterator &, iterator &),
                __HAS_FCN(T, operator=, iterator &&, iterator &)
        >::type;


        template<typename>
        static constexpr false_type check(...);

        typedef decltype(check<C>(nullptr)) type;

    public:
        static constexpr bool value = type::value;
    };

    /**
     * Shorthand to check whether a class is a forward iterator.
     *
     * @tparam C candidate forward iterator type
     * @return true if the class is a forward iterator
     */
    template<typename C>
    static constexpr bool is_forward_iterator() {
        return forward_iterator_concept<C>::value;
    }

    /**
     * Random access iterator base case is false.
     *
     * @tparam C candidate random access iterator
     */
    template<typename C, bool = has_size_type<C>::value && has_val_type<C>::value>
    struct random_access_iterator_concept {
        static constexpr bool value = false;
    };

    /**
     * Random access iterator concept. Defines any iterator
     * over a random access structure, which can be used to
     * access and assign to any element in the structure
     * with constant time.
     *
     * @tparam C random access iterator type
     */
    template<typename C>
    struct random_access_iterator_concept<C, true> {
    private:
        typedef typename C::val_type val_type;
        typedef typename C::size_type size_type;
        typedef C iterator;

        template<typename T>
        static constexpr auto check(T *) -> typename and_<
                typename or_<
                        __HAS_FCN(const T, operator*, val_type &),
                        __HAS_FCN(const T, operator*, const val_type &)
                >::type,
                typename or_<
                        __HAS_FCN(const T, operator->, val_type *),
                        __HAS_FCN(const T, operator->, const val_type *)
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
                __HAS_FCN(const T, operator+, const size_type &, iterator),
                __HAS_FCN(const T, operator+, size_type &&, iterator),
                __HAS_FCN(const T, operator-, const size_type &, iterator),
                __HAS_FCN(const T, operator-, size_type &&, iterator),
                __HAS_FCN(const T, operator-, const iterator &, size_type),
                __HAS_FCN(const T, operator-, iterator &&, size_type),
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

    /**
     * Shorthand to check whether a class is a random
     * access iterator.
     *
     * @tparam C candidate random access iterator
     * @return true if the class is a random access iterator
     */
    template<typename C>
    static constexpr bool is_random_access_iterator() {
        return random_access_iterator_concept<C>::value;
    }

    /**
     * Map concept base case is false.
     *
     * @tparam C candidate map type
     */
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

    /**
     * Map concept. Defines a structure that uniquely associates
     * a key to a value, in such a way that the collection of the
     * structure's keys contains only unique values, but the collection
     * of the structure's values may not be unique.
     *
     * @tparam C map type
     */
    template<typename C>
    struct map_concept<C, true> {
    private:
        typedef typename C::key_type key_type;
        typedef typename C::val_type val_type;
        typedef typename C::size_type size_type;
        typedef typename C::iterator iterator;
        typedef typename C::const_iterator const_iterator;
        typedef C map_type;

        typedef Pair<iterator, bool> insert_ret_type;

        template<typename T>
        static constexpr auto check(T *) -> typename and_<
                __HAS_FCN(const T, size, size_type),
                __HAS_FCN(const T, capacity, size_type),
                __HAS_FCN(const T, empty, bool),
                __HAS_FCN(T, begin, iterator),
                __HAS_FCN(T, end, iterator),
                __HAS_FCN(const T, begin, const_iterator),
                __HAS_FCN(const T, end, const_iterator),
                __HAS_FCN(T, clear, void),
                __HAS_FCN(T, insert, key_type, val_type, insert_ret_type),
                __HAS_FCN(T, insert_or_assign, key_type, val_type, insert_ret_type),
                __HAS_FCN(T, erase, iterator &, iterator &),
                __HAS_FCN(T, erase, const_iterator &, const_iterator &),
                __HAS_FCN(T, erase, key_type &, bool),
                __HAS_FCN(T, at, const key_type &, iterator),
                __HAS_FCN(const T, at, const key_type &, const_iterator),
                __HAS_FCN(const T, contains, const key_type &, bool),
                __HAS_FCN(T, find, const key_type &, iterator),
                __HAS_FCN(const T, find, const key_type &, const_iterator),
                __HAS_FCN(T, operator[], const key_type &, val_type &),
                __HAS_FCN(T, operator=, map_type &&, map_type &)
        >::type;

        template<typename>
        static constexpr false_type check(...);

        typedef decltype(check<C>(nullptr)) type;

    public:
        static constexpr bool value = type::value;
    };

    /**
     * Shorthand to check whether a class is a map.
     *
     * @tparam C candidate map type
     * @return true if the class is a map
     */
    template<typename C>
    static constexpr bool is_map() {
        return map_concept<C>::value;
    }

}

#endif //EMBEDDEDCPLUSPLUS_CONCEPTCHECKS_H
