/**
 * @file Tuple.h
 * @brief Template oriented tuple implementation.
 *
 * @author Jeff Niu
 * @date November 5, 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDCPLUSPLUS_TUPLE_H
#define EMBEDDEDCPLUSPLUS_TUPLE_H

#include <stdint.h>

#include "Tmp.h"

namespace wlp {

    // TupleElement
    template<size_type I, typename T>
    struct TupleElement {
        T value;

        TupleElement() {}

        explicit TupleElement(T const &t) : value(t) {
        }

        explicit TupleElement(T &&t) : value(move(t)) {
        }

        template<typename U>
        explicit TupleElement(U &&u) : value(forward<U>(u)) {
        }
    };

    // IndexSequence
    template<size_type... Indices>
    struct IndexSequence {
        using type = IndexSequence<Indices...>;
    };

    // MakeIndexSequence
    template<size_type I, typename Sequence>
    struct AppendIndexSequence;

    template<size_type I, size_type... Indices>
    struct AppendIndexSequence<I, IndexSequence<Indices...>>
            : IndexSequence<Indices..., I> {
    };

    template<size_type N>
    struct MakeIndexSequence
            : AppendIndexSequence<N - 1, typename MakeIndexSequence<N - 1>::type>::type {
    };

    template<>
    struct MakeIndexSequence<1>
            : IndexSequence<0> {
    };

    // TypeAtIndex
    template<size_type I, typename Head, typename... Tail>
    struct TypeAtIndex {
        using type = typename TypeAtIndex<I - 1, Tail...>::type;
    };

    template<typename Head, typename... Tail>
    struct TypeAtIndex<0, Head, Tail...> {
        using type = Head;
    };

    template<size_type I, typename... Types>
    using TypeAtIndexType = typename TypeAtIndex<I, Types...>::type;

    // TupleImpl
    template<typename Sequence, typename...Types>
    struct TupleImpl;

    template<typename>
    struct IsTupleImpl
            : false_type {
    };


    template<size_type... Indices, typename... Types>
    struct IsTupleImpl<TupleImpl<IndexSequence<Indices...>, Types...>>
            : true_type {
    };

    template<size_type... Indices, typename... Types>
    struct TupleImpl<IndexSequence<Indices...>, Types...>
            : TupleElement<Indices, Types> ... {

        template<typename... OtherTypes>
        explicit TupleImpl(OtherTypes &&... elements)
                : TupleElement<Indices, Types>(forward<OtherTypes>(elements))... {
        }

        template<typename... OtherTypes, typename = typename enable_if<
                !is_any_of<IsTupleImpl, typename decay<OtherTypes>::type...>()
        >::type>
        explicit TupleImpl(OtherTypes &&... elements)
                : TupleElement<Indices, Types>(forward<OtherTypes>(elements))... {
        }

    };

    // Tuple
    template<typename... Types>
    class Tuple
            : public TupleImpl<typename MakeIndexSequence<sizeof...(Types)>::type, Types...> {

        Tuple(Tuple const &) = default;

        Tuple(Tuple &&) = default;

        Tuple &operator=(Tuple const &) = default;

        Tuple &operator=(Tuple &&) = default;

    public:

        using base_type = TupleImpl<typename MakeIndexSequence<sizeof...(Types)>::type, Types...>;

        explicit Tuple()
                : base_type(forward<Types>(Types())...) {
        }

        template<typename... OtherTypes, typename = typename enable_if<
                sizeof...(OtherTypes) == sizeof...(Types)
        >::type>
        explicit Tuple(OtherTypes &&... elements)
                : base_type(forward<OtherTypes>(elements)...) {
        }

        template<typename... OtherTypes>
        explicit Tuple(Tuple<OtherTypes...> const &tuple)
                : base_type(tuple) {
        }

        template<typename... OtherTypes>
        explicit Tuple(Tuple<OtherTypes...> &&tuple) : base_type(move(tuple)) {
        }

    };

    // Get
    template<size_type I, typename... Types>
    TypeAtIndexType<I, Types...> const &get(Tuple<Types...> const &tuple) {
        TupleElement<I, TypeAtIndexType<I, Types...>> const &base = tuple;
        return base.value;
    };

    template<size_type I, typename... Types>
    TypeAtIndexType<I, Types...> &get(Tuple<Types...> &tuple) {
        TupleElement<I, TypeAtIndexType<I, Types...>> &base = tuple;
        return base.value;
    };

    template<size_type I, typename... Types>
    remove_reference_type<TypeAtIndexType<I, Types...>> &&get(Tuple<Types...> &&tuple) {
        TupleElement<I, TypeAtIndexType<I, Types...>> base = move(tuple);
        return base.value;
    };

    template<typename>
    constexpr int count() {
        return 0;
    }

    template<typename T, typename Head, typename... Tail>
    constexpr int count() {
        return (is_same<T, Head>::value ? 1 : 0) + count<T, Tail...>();
    };

    template<typename>
    constexpr int find(int) {
        return -1;
    }

    template<typename T, typename Head, typename... Tail>
    constexpr int find(int index = 0) {
        return is_same<T, Head>::value ? index : find<T, Tail...>(index + 1);
    };

    template<typename T, typename... Types>
    T &get(Tuple<Types...> const &tuple) {
        static_assert(count<T, Types...>() == 1, "Type must be unique in Tuple");
        return get<find<T, Types...>()>(tuple);
    }

    template<typename T, typename... Types>
    T &get(Tuple<Types...> &tuple) {
        static_assert(count<T, Types...>() == 1, "Type must be unique in Tuple");
        return get<find<T, Types...>()>(tuple);
    }

    template<typename T, typename... Types>
    T &get(Tuple<Types...> &&tuple) {
        static_assert(count<T, Types...>() == 1, "Type must be unique in Tuple");
        return get<find<T, Types...>()>(tuple);
    }

}

#endif //EMBEDDEDCPLUSPLUS_TUPLE_H
