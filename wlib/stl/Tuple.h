/**
 * @file Tuple.h
 * @brief Template oriented tuple implementation.
 *
 * The template oriented tuple implementation is flexible and
 * performant and resolves at compile time, resulting in
 * good runtime performance.
 *
 * @author Jeff Niu
 * @date November 5, 2017
 * @bug No known bugs
 */

/*
 * TODO: implement a better tuple concatenation function, template specializations for Tuple, copy ctors
 */

#ifndef EMBEDDEDCPLUSPLUS_TUPLE_H
#define EMBEDDEDCPLUSPLUS_TUPLE_H

#include "Pair.h"
#include "../utility/Tmp.h"

namespace wlp {

    /**
     * Tuple is a flat implementation so that compile-time template
     * instantiations are not recursive. The tuple implementation will
     * inherit from an element type that contains the type index
     * and the held type. The index is needed to discern multiple types.
     * @tparam I the element index
     * @tparam T the tuple element type
     */
    template<size_type I, typename T>
    struct TupleElement {
        /**
         * Value contained in this tuple element.
         */
        T value;

        /**
         * Assignment operator to another tuple type. This operator
         * is only enabled for tuple elements at the same index
         * and of the same type.
         * @tparam J assigned tuple element index
         * @tparam Q assigned tuple element type
         * @param te tuple element to assign
         * @return a reference to this element
         */
        template<size_type J, typename Q, typename = typename enable_if<
                I == J &&
                is_same<decay_type<T>, decay_type<Q>>::value
        >::type>
        TupleElement<I, T> &operator=(TupleElement<J, Q> const &te) {
            value = te.value;
            return *this;
        }

        /**
         * Move assignment operator to another tuple type.
         * @tparam J assigned tuple element index
         * @tparam Q assigned tuple element type
         * @param te tuple element to assign
         * @return a reference to this element
         */
        template<size_type J, typename Q, typename = typename enable_if<
                I == J &&
                is_same<decay_type<T>, decay_type<Q>>::value
        >::type>
        TupleElement<I, T> &operator=(TupleElement<J, Q> &&te) {
            value = move(te.value);
            return *this;
        };

        /**
         * Universal constructor sets the contained
         * value in the element
         * @tparam U value type
         * @param u value
         */
        template<typename U>
        explicit TupleElement(U &&u) : value(forward<U>(u)) {
        }

    };

    /**
     * An index sequence is a container object for
     * an index pack and is needed to expand the index
     * pack to instantiate tuples.
     * @tparam Indices the index pack to expand
     */
    template<size_type... Indices>
    struct IndexSequence {
        using type = IndexSequence<Indices...>;
    };

    /**
     * This type is used in the creation of an index
     * sequence and appends an index to the sequence.
     * This is the undefined base type.
     * @tparam I index to append
     * @tparam Sequence index sequence to append to
     */
    template<size_type I, typename Sequence>
    struct AppendIndexSequence;

    /**
     * Append an index to a index sequence pack.
     * This template specialization creates an
     * index sequence with the appended index
     * and the existing index sequence.
     * @tparam I index to append
     * @tparam Indices existing index pack
     */
    template<size_type I, size_type... Indices>
    struct AppendIndexSequence<I, IndexSequence<Indices...>>
            : IndexSequence<Indices..., I> {
    };

    /**
     * Recursive type instantiates an index sequence
     * with value up to N.
     * @tparam N the current index to append
     */
    template<size_type N>
    struct MakeIndexSequence
            : AppendIndexSequence<N - 1, typename MakeIndexSequence<N - 1>::type>::type {
    };

    /**
     * Base case type creates an index sequence
     * with one value.
     */
    template<>
    struct MakeIndexSequence<1>
            : IndexSequence<0> {
    };

    /**
     * Recursive type contains the type in a type pack
     * at a certain index.
     * @tparam I the index to get the type
     * @tparam Head the head of the type pack
     * @tparam Tail the remaining types in the pack
     */
    template<size_type I, typename Head, typename... Tail>
    struct TypeAtIndex {
        using type = typename TypeAtIndex<I - 1, Tail...>::type;
    };

    /**
     * Base case of the type grabs
     * the current head type.
     * @tparam Head the head of the current type pack
     * @tparam Tail the remaining types, if any
     */
    template<typename Head, typename... Tail>
    struct TypeAtIndex<0, Head, Tail...> {
        using type = Head;
    };

    /**
     * Shorthand to obtain the type at an index.
     */
    template<size_type I, typename... Types>
    using TypeAtIndexType = typename TypeAtIndex<I, Types...>::type;

    /**
     * Undefined base type of the tuple implementation.
     * @tparam Sequence index sequence containing the indices
     * @tparam Types tuple type pack
     */
    template<typename Sequence, typename... Types>
    struct TupleImpl;

    /**
     * Boolean type for whether a type is an
     * instance of TupleImpl. Base case is false.
     */
    template<typename>
    struct IsTupleImpl
            : false_type {
    };

    /**
     * Template specialization for TupleImpl
     * is true type.
     * @tparam Indices tuple indices
     * @tparam Types tuple types
     */
    template<size_type... Indices, typename... Types>
    struct IsTupleImpl<TupleImpl<IndexSequence<Indices...>, Types...>>
            : true_type {
    };

    /**
     * The TupleImpl is the base type for tuple that
     * contains the type pack and the index pack, and
     * will inherit from each tuple element.
     * @tparam Indices the index pack
     * @tparam Types the type pack
     */
    template<size_type... Indices, typename... Types>
    struct TupleImpl<IndexSequence<Indices...>, Types...>
            : TupleElement<Indices, Types> ... {

        /**
         * Assignment operator to another TupleImpl will expand
         * the assignment operator on each individual tuple element
         * from which this class inherits.
         *
         * @tparam OtherTypes type pack of the assigned TupleImpl
         * @param tuple the TupleImpl to assign
         * @return a reference to this tuple
         */
        template<typename... OtherTypes, typename = typename enable_if<
                sizeof...(Types) == sizeof...(OtherTypes)
        >::type>
        TupleImpl<IndexSequence<Indices...>, Types...> &
        operator=(TupleImpl<IndexSequence<Indices...>, OtherTypes...> const &tuple) {
            swallow(static_cast<TupleElement<Indices, TypeAtIndexType<Indices, Types...>> &>(*this).operator=(
                    static_cast<TupleElement<Indices, TypeAtIndexType<Indices, OtherTypes...>> const &>(tuple))...
            );
            return *this;
        }

        /**
         * Move assignment operator for TupleImpl.
         *
         * @tparam OtherTypes type pack of the assigned TupleImpl
         * @param tuple the TupleImpl to assign
         * @return a reference to this tuple
         */
        template<typename... OtherTypes, typename = typename enable_if<
                sizeof...(Types) == sizeof...(OtherTypes)
        >::type>
        TupleImpl<IndexSequence<Indices...>, Types...> &
        operator=(TupleImpl<IndexSequence<Indices...>, OtherTypes...> &&tuple) {
            swallow(static_cast<TupleElement<Indices, TypeAtIndexType<Indices, Types...>> &>(*this).operator=(move(
                    static_cast<TupleElement<Indices, TypeAtIndexType<Indices, OtherTypes...>> &&>(tuple)))...
            );
            return *this;
        };

        /**
         * TupleImpl constructor will instantiate each
         * of its inherit types. This is a universal constructor.
         *
         * @tparam OtherTypes type pack of the constructed types
         * @param elements values for each tuple element
         */
        template<typename... OtherTypes, typename = typename enable_if<
                !is_any_of<IsTupleImpl, decay_type<OtherTypes>...>()
        >::type>
        explicit TupleImpl(OtherTypes &&... elements)
                : TupleElement<Indices, Types>(forward<OtherTypes>(elements))... {
        }

    };

    /**
     * Main Tuple class. Inherits from TupleImpl
     * and hides the index parameter.
     *
     * @tparam Types the tuple types
     */
    template<typename... Types>
    class Tuple
            : public TupleImpl<typename MakeIndexSequence<sizeof...(Types)>::type, Types...> {

    public:

        /**
         * Shorthand for the inherited TupleImpl type.
         */
        using base_type = TupleImpl<typename MakeIndexSequence<sizeof...(Types)>::type, Types...>;

        /**
         * Assignment operator for tuple casts to the inherited type
         * and defers the operation there.
         *
         * @tparam OtherTypes types of the assigned tuple
         * @param tuple assigned tuple
         * @return a reference to this tuple
         */
        template<typename... OtherTypes>
        Tuple<Types...> &operator=(Tuple<OtherTypes...> const &tuple) {
            static_cast<base_type &>(*this) = tuple;
            return *this;
        }

        /**
         * Move assignment operator.
         *
         * @tparam OtherTypes types of assigned tuple
         * @param tuple assigned tuple
         * @return a reference to this tuple
         */
        template<typename... OtherTypes>
        Tuple<Types...> &operator=(Tuple<OtherTypes...> &&tuple) {
            static_cast<base_type &>(*this) = move(tuple);
            return *this;
        }

        /**
         * Assignment operator for a pair. Currently, a pair
         * can be assigned to any tuple with at least two elements,
         * and will assign to the first two of the elements.
         *
         * @tparam A pair first type
         * @tparam B pair second type
         * @param pair the pair to assign
         * @return a reference to this tuple
         */
        template<typename A, typename B>
        Tuple<Types...> &operator=(Pair<A, B> const &pair) {
            static_cast<TupleElement<0, TypeAtIndexType<0, Types...>> &>(*this).value = pair.m_first;
            static_cast<TupleElement<1, TypeAtIndexType<1, Types...>> &>(*this).value = pair.m_second;
            return *this;
        }

        /**
         * Pair move assignment operator.
         *
         * @tparam A pair first type
         * @tparam B pair second type
         * @param pair the pair to assign
         * @return a reference to this tuple
         */
        template<typename A, typename B>
        Tuple<Types...> &operator=(Pair<A, B> &&pair) {
            static_cast<TupleElement<0, TypeAtIndexType<0, Types...>> &>(*this).value = move(pair.m_first);
            static_cast<TupleElement<1, TypeAtIndexType<1, Types...>> &>(*this).value = move(pair.m_second);
            return *this;
        }

        /**
         * Default constructor initializes with empty values.
         */
        explicit Tuple()
                : base_type(forward<Types>(Types())...) {
        }

        /**
         * Constructor for a tuple of each its elements.
         * Enabled only if the correct number of element
         * values have been provided.
         *
         * @tparam OtherTypes types of the element values
         * @param elements element values pack
         */
        template<typename... OtherTypes, typename = typename enable_if<
                sizeof...(OtherTypes) == sizeof...(Types)
        >::type>
        explicit Tuple(OtherTypes &&... elements)
                : base_type(forward<OtherTypes>(elements)...) {
        }

        /**
         * Copy constructor for tuple defers
         * to inherited copy constructor.
         *
         * @tparam OtherTypes types of the copied tuple
         * @param tuple copied tuple
         */
        template<typename... OtherTypes, typename = typename enable_if<
                sizeof...(OtherTypes) == sizeof...(Types)
        >::type>
        explicit Tuple(Tuple<OtherTypes...> const &tuple)
                : base_type(tuple) {
        }

        /**
         * Move copy constructor for tuple
         * defers to inherited move copy constructor.
         *
         * @tparam OtherTypes types of the copied tuple
         * @param tuple copied tuple
         */
        template<typename... OtherTypes, typename = typename enable_if<
                sizeof...(OtherTypes) == sizeof...(Types)
        >::type>
        explicit Tuple(Tuple<OtherTypes...> &&tuple)
                : base_type(move(tuple)) {
        }

    };

    /**
     * Get function for const lvalue tuple. Returns the
     * tuple element value at the specified index.
     * @tparam I the index of the element to get
     * @tparam Types the tuple types
     * @param tuple the tuple from which to get
     * @return the element value at the index
     */
    template<size_type I, typename... Types>
    TypeAtIndexType<I, Types...> const &get(Tuple<Types...> const &tuple) {
        TupleElement<I, TypeAtIndexType<I, Types...>> const &base = tuple;
        return base.value;
    };

    /**
     * Get function for lvalue tuple.
     * @tparam I index of the element to get
     * @tparam Types the tuple types
     * @param tuple the tuple from which to get
     * @return the element value at the index
     */
    template<size_type I, typename... Types>
    TypeAtIndexType<I, Types...> &get(Tuple<Types...> &tuple) {
        TupleElement<I, TypeAtIndexType<I, Types...>> &base = tuple;
        return base.value;
    };

    /**
     * Get function for rvalue tuple returns an rvalue
     * value type.
     * @tparam I index of the element to get
     * @tparam Types the tuple types
     * @param tuple the tuple from which to get
     * @return the element value at the index
     */
    template<size_type I, typename... Types>
    remove_reference_type<TypeAtIndexType<I, Types...>> &&get(Tuple<Types...> &&tuple) {
        TupleElement<I, TypeAtIndexType<I, Types...>> base = tuple;
        return forward<TypeAtIndexType<I, Types...>>(base.value);
    };

    /**
     * Base case for count is zero.
     * @return
     */
    template<typename>
    constexpr int count() {
        return 0;
    }

    /**
     * This function counts the number of times a particular
     * type appears in a tuple type pack.
     * @tparam T the type to count
     * @tparam Head tuple head type
     * @tparam Tail tuple tail types
     * @return the number of types
     */
    template<typename T, typename Head, typename... Tail>
    constexpr int count() {
        return (is_same<T, Head>::value ? 1 : 0) + count<T, Tail...>();
    };

    /**
     * Grab the index of a type. Base
     * case is not found.
     * @return not found index
     */
    template<typename>
    constexpr int find(int) {
        return -1;
    }

    /**
     * Find the index of a type in a tuple's type pack.
     * @tparam T the type to find
     * @tparam Head tuple head type
     * @tparam Tail tuple tail types
     * @param index the current search index
     * @return the index of the type
     */
    template<typename T, typename Head, typename... Tail>
    constexpr int find(int index = 0) {
        return is_same<T, Head>::value ? index : find<T, Tail...>(index + 1);
    };

    /**
     * Get function for a tuple by type.
     * @tparam T type to get
     * @tparam Types tuple types
     * @param tuple tuple from which to get
     * @return the value of the type
     */
    template<typename T, typename... Types>
    T &get(Tuple<Types...> const &tuple) {
        static_assert(count<T, Types...>() == 1, "Type must be unique in Tuple");
        return get<find<T, Types...>()>(tuple);
    }

    /**
     * Get function for a tuple by type.
     * @tparam T type to get
     * @tparam Types tuple types
     * @param tuple tuple from which to get
     * @return the value of the type
     */
    template<typename T, typename... Types>
    T &get(Tuple<Types...> &tuple) {
        static_assert(count<T, Types...>() == 1, "Type must be unique in Tuple");
        return get<find<T, Types...>()>(tuple);
    }

    /**
     * Get function for a tuple by type.
     * @tparam T type to get
     * @tparam Types tuple types
     * @param tuple tuple from which to get
     * @return the value of the type
     */
    template<typename T, typename... Types>
    T &get(Tuple<Types...> &&tuple) {
        static_assert(count<T, Types...>() == 1, "Type must be unique in Tuple");
        return get<find<T, Types...>()>(tuple);
    }

    /**
     * Undefined tuple size base type.
     */
    template<typename>
    struct tuple_size;

    /**
     * Tuple size recursively computes the
     * size of a tuple at compile time.
     * @tparam Types the types of the tuple
     */
    template<typename... Types>
    struct tuple_size<Tuple<Types...>> : integral_constant<size_type, sizeof...(Types)> {
    };

    /**
     * Shorthand to obtain the size of a
     * tuple at runtime.
     * @tparam Types the types to count
     * @return the number of types
     */
    template<typename... Types>
    constexpr size_type get_tuple_size(Tuple<Types...>) {
        return sizeof...(Types);
    }

    /**
     * Forwards elements into a tuple.
     * @tparam Types types to forward
     * @param elements element values to forward
     * @return a tuple of lvalues or rvalues
     */
    template<typename... Types>
    Tuple<Types &&...> forward_as_tuple(Types &&... elements) {
        return Tuple<Types &&...>(forward<Types>(elements)...);
    }

    /**
     * Ignore universal type is used during
     * tuple assignment to tie to ignore a
     * particular tuple value.
     */
    struct ignore_type {
        template<typename U>
        const ignore_type &operator=(U &&) const {
            return *this;
        }
    };

    const ignore_type ignore{};

    /**
     * Tie creates a tuple of lvalues that can be
     * used as a helper to quickly assign
     * tuple values to variables.
     * @tparam Types
     * @param elements
     * @return
     */
    template<typename... Types>
    Tuple<Types &...> tie(Types &... elements) {
        return Tuple<Types &...>(elements...);
    }

    template<typename Type, size_type>
    struct repeat_tuple_type_hopper {
        typedef Type type;
    };

    template<typename, typename>
    struct repeat_tuple_type_sub;

    template<typename Type, size_type ...Indices>
    struct repeat_tuple_type_sub<Type, IndexSequence<Indices...>> {
        using type = Tuple<typename repeat_tuple_type_hopper<Type, Indices>::type...>;
    };

    template<typename Type, size_type Repeat>
    struct repeat_tuple_type {
        using Sequence = typename MakeIndexSequence<Repeat>::type;
        typedef typename repeat_tuple_type_sub<Type, Sequence>::type type;
    };

    template<typename Type, size_type Repeat>
    using RepeatTuple = typename repeat_tuple_type<Type, Repeat>::type;

    /**
     * Undefined base type for type at tuple.
     */
    template<size_type, typename>
    struct TypeAtTuple;

    /**
     * Returns the tuple at the specified index in the tuple
     * @tparam I the index whose type to get
     * @tparam Types the type pack
     */
    template<size_type I, typename... Types>
    struct TypeAtTuple<I, Tuple<Types...>>
            : TypeAtIndex<I, Types...> {
    };

    /**
     * Base case pair tuple concatenation type is undefined.
     */
    template<typename, typename, typename, typename>
    struct cat_pair_type_sub;

    /**
     * Helper type for determining the resultant type of
     * a tuple concatenation.
     * @tparam TupleA first tuple type
     * @tparam IndicesA first tuple index pack
     * @tparam TupleB second tuple type
     * @tparam IndicesB second tuple index pack
     */
    template<typename TupleA, size_type... IndicesA, typename TupleB, size_type... IndicesB>
    struct cat_pair_type_sub<
            TupleA, IndexSequence<IndicesA...>,
            TupleB, IndexSequence<IndicesB...>
    > {
        using type = Tuple<
                typename TypeAtTuple<IndicesA, TupleA>::type...,
                typename TypeAtTuple<IndicesB, TupleB>::type...
        >;
    };

    /**
     * Obtain the resultant type of the
     * concatenation of two tuples.
     * @tparam TupleA the first tuple type
     * @tparam TupleB the second tuple type
     */
    template<typename TupleA, typename TupleB>
    struct cat_pair_type {
        static constexpr size_type SizeA = tuple_size<decay_type<TupleA>>::value;
        static constexpr size_type SizeB = tuple_size<decay_type<TupleB>>::value;
        using SequenceA = typename MakeIndexSequence<SizeA>::type;
        using SequenceB = typename MakeIndexSequence<SizeB>::type;
        using type = typename cat_pair_type_sub<
                decay_type<TupleA>, SequenceA,
                decay_type<TupleB>, SequenceB
        >::type;
    };

    /**
     * Universal helper function to create
     * a tuple from elements.
     * @tparam Types the element types
     * @param elements the element values
     * @return a tuple sized to the number of elements
     */
    template<typename... Types>
    Tuple<decay_type<Types>...> make_tuple(Types &&... elements) {
        return Tuple<decay_type<Types>...>(forward<Types>(elements)...);
    }

    /**
     * Tuple concatenation helper function.
     * @tparam TupleA first tuple type
     * @tparam IndicesA first tuple index pack
     * @tparam TupleB second tuple type
     * @tparam IndicesB second tuple index pack
     * @param tupleA first tuple
     * @param tupleB second tuple
     * @return concatenation of the two tuples
     */
    template<typename TupleA, size_type... IndicesA, typename TupleB, size_type... IndicesB>
    Tuple<
            typename TypeAtTuple<IndicesA, decay_type<TupleA>>::type...,
            typename TypeAtTuple<IndicesB, decay_type<TupleB>>::type...
    >
    tuple_cat_pair_sub(
            TupleA &&tupleA, TupleB &&tupleB,
            IndexSequence<IndicesA...>, IndexSequence<IndicesB...>) {
        return make_tuple(
                get<IndicesA>(forward<TupleA>(tupleA))...,
                get<IndicesB>(forward<TupleB>(tupleB))...
        );
    }

    /**
     * Concatenate exactly two tuples together.
     * @tparam TupleA first tuple type
     * @tparam TupleB second tuple type
     * @param tupleA first tuple to concate
     * @param tupleB second tuple to concate
     * @return the tuples combined
     */
    template<typename TupleA, typename TupleB>
    typename cat_pair_type<TupleA, TupleB>::type
    tuple_cat_pair(TupleA &&tupleA, TupleB &&tupleB) {
        return tuple_cat_pair_sub(
                forward<TupleA>(tupleA),
                forward<TupleB>(tupleB),
                MakeIndexSequence<tuple_size<decay_type<TupleA>>::value>{},
                MakeIndexSequence<tuple_size<decay_type<TupleB>>::value>{}
        );
    };

    /**
     * Base case tuple concatenation for a single tuple
     * returns the tuple.
     * @tparam HeadTuple the tuple head
     * @param tuple the singular tuple
     * @return forwarded tuple
     */
    template<typename HeadTuple>
    HeadTuple &&tuple_cat(HeadTuple &&tuple) {
        return forward<HeadTuple>(tuple);
    }

    /**
     * Base case for the concatenation of two tuples
     * calls the tuple pair concatenation function.
     * @tparam HeadTupleA first tuple type
     * @tparam HeadTupleB second tuple type
     * @param tupleA first tuple
     * @param tupleB second tuple
     * @return the concatenated tuples
     */
    template<typename HeadTupleA, typename HeadTupleB>
    typename cat_pair_type<HeadTupleA, HeadTupleB>::type
    tuple_cat(HeadTupleA &&tupleA, HeadTupleB &&tupleB) {
        return tuple_cat_pair(
                forward<HeadTupleA>(tupleA),
                forward<HeadTupleB>(tupleB)
        );
    };

    template<typename...>
    struct tuple_cat_type;

    template<typename Tuple>
    struct tuple_cat_type<Tuple> {
        typedef Tuple type;
    };

    template<typename TupleA, typename TupleB>
    struct tuple_cat_type<TupleA, TupleB> {
        typedef typename cat_pair_type<TupleA, TupleB>::type type;
    };

    template<typename TupleA, typename TupleB, typename... Tuples>
    struct tuple_cat_type<TupleA, TupleB, Tuples...> {
    private:
        typedef typename cat_pair_type<TupleA, TupleB>::type pair_type;
    public:
        typedef typename tuple_cat_type<pair_type, Tuples...>::type type;
    };

    /**
     * Concatenate an arbitrary number of tuples together.
     * This concatenation method expands linearly.
     * @tparam HeadTupleA first head tuple type
     * @tparam HeadTupleB second head tuple tye
     * @tparam TailTuples tail tuple types
     * @param tupleA first tuple
     * @param tupleB second tuple
     * @param tail the tail tuples
     * @return concatenated tuples
     */
    template<typename HeadTupleA, typename HeadTupleB, typename... TailTuples>
    typename tuple_cat_type<HeadTupleA, HeadTupleB, TailTuples...>::type
    tuple_cat(HeadTupleA &&tupleA, HeadTupleB &&tupleB, TailTuples &&... tail) {
        auto tuple = tuple_cat_pair(
                forward<HeadTupleA>(tupleA),
                forward<HeadTupleB>(tupleB)
        );
        return tuple_cat(
                tuple,
                forward<TailTuples>(tail)...
        );
    };

}

#endif //EMBEDDEDCPLUSPLUS_TUPLE_H
