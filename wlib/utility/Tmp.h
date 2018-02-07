/**
 * @file Tmp.h
 * @brief Template helper types, structs, and metafunctions
 *
 * This class implements helper items that facilitate template
 * metaprogramming and functions that enable move semantics.
 *
 * @author Jeff Niu
 * @date November 6, 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDCPLUSPLUS_TMP_H
#define EMBEDDEDCPLUSPLUS_TMP_H

#include "wlib/Types.h"

namespace wlp {

    typedef decltype(nullptr) nullptr_t;

    /**
     * This function consumes, and does nothing with, the return
     * values of parameter pack expanded function calls. This function
     * enables individual functions to be called on each parameter pack
     * item by consuming the expansion.
     *
     * @tparam Types parameter pack of arbitrary return types
     * which may be void
     */
    template<typename... Types>
    void swallow(Types &&...) {
    }

    /**
     * Forward declaration of @code is_function @endcode.
     */
    template<typename>
    struct is_function;

    /**
     * Helper type that contains a single value of a certain type.
     * @tparam T value type
     * @tparam t contained value
     */
    template<typename T, T t>
    struct integral_constant {
        static constexpr T value = t;
        typedef T value_type;
        typedef integral_constant<T, t> type;

        constexpr operator value_type() const {
            return value;
        }

        constexpr value_type operator()() const {
            return value;
        }
    };

    /**
     * Constant expression shorthand.
     * @tparam T value type
     * @tparam t contained value
     */
    template<typename T, T t>
    constexpr T integral_constant<T, t>::value;

    /**
     * Container for @code true @endcode.
     */
    typedef integral_constant<bool, true> true_type;
    /**
     * Container for @code false @endcode.
     */
    typedef integral_constant<bool, false> false_type;

    /**
     * A boolean constant contains a value that is
     * either true or false.
     */
    template<bool t>
    using boolean_constant = integral_constant<bool, t>;

    /**
     * Building block type for boolean metafunctions.
     * This is the undefined base declaration.
     */
    template<bool, typename, typename>
    struct conditional;

    /**
     * The conditional contains a true type if the specified
     * condition is true.
     * @tparam Condition true in the case
     * @tparam IfTrue consequence of truth
     * @tparam IfFalse consequence of false
     */
    template<bool Condition, typename IfTrue, typename IfFalse>
    struct conditional {
        typedef IfTrue type;
    };

    /**
     * Template specialization for the false case.
     * @tparam IfTrue consequence of truth
     * @tparam IfFalse consequence of false
     */
    template<typename IfTrue, typename IfFalse>
    struct conditional<false, IfTrue, IfFalse> {
        typedef IfFalse type;
    };

    /**
     * Remove the const modifier from types for
     * type comparisons only. This is the base case
     * template class.
     * @tparam T const type
     */
    template<typename T>
    struct remove_const {
        typedef T type;
    };

    /**
     * Remove the const modifier from a type.
     * Template specialization for const types.
     * @tparam T const type
     */
    template<typename T>
    struct remove_const<T const> {
        typedef T type;
    };

    template<typename T>
    struct remove_volatile {
        typedef T type;
    };

    template<typename T>
    struct remove_volatile<T volatile> {
        typedef T type;
    };

    template<typename T>
    struct remove_cv {
        typedef typename
        remove_const<typename remove_volatile<T>::type>::type type;
    };

    /**
     * Base declaration for a boolean or.
     * @tparam ... ignored parameteres
     */
    template<typename...>
    struct or_;

    /**
     * Template specialization; the or with
     * no boolean operands is false.
     */
    template<>
    struct or_<>
            : public false_type {
    };

    /**
     * Template specialization for unary or.
     * @tparam Q1 boolean condition
     */
    template<typename Q1>
    struct or_<Q1>
            : public Q1 {
    };

    /**
     * Template specialization of binary or.
     * @tparam Q1 first boolean condition
     * @tparam Q2 second boolean condition
     */
    template<typename Q1, typename Q2>
    struct or_<Q1, Q2>
            : public conditional<Q1::value, Q1, Q2>::type {
    };

    template<typename Q1, typename Q2, typename Q3, typename... Qn>
    struct or_<Q1, Q2, Q3, Qn...>
            : public conditional<Q1::value, Q1, or_<Q2, Q3, Qn...>>::type {
    };

    template<typename...>
    struct and_;

    template<>
    struct and_<>
            : public true_type {
    };

    template<typename Q1>
    struct and_<Q1>
            : public Q1 {
    };

    template<typename Q1, typename Q2>
    struct and_<Q1, Q2>
            : public conditional<Q1::value, Q2, Q1>::type {
    };

    template<typename Q1, typename Q2, typename Q3, typename... Qn>
    struct and_<Q1, Q2, Q3, Qn...>
            : public conditional<Q1::value, and_<Q2, Q3, Qn...>, Q1>::type {
    };

    /**
     * This type inverts the boolean value of another boolean
     * constant type.
     * @tparam T boolean constant type
     */
    template<typename T>
    struct not_
            : public integral_constant<bool, !T::value> {
    };

    /**
     * Void checking type. Base case is false.
     */
    template<typename>
    struct __void__
            : public false_type {
    };

    /**
     * Void checking type template specialization
     * for void types is true.
     */
    template<>
    struct __void__<void>
            : public true_type {
    };

    /**
     * Metafunction checks if a type is void.
     * @tparam T type to check
     */
    template<typename T>
    struct is_void
            : public __void__<typename remove_const<T>::type>::type {
    };

    /**
     * L-value checking type. Base case is false.
     */
    template<typename>
    struct is_lvalue_reference
            : public false_type {
    };

    /**
     * L-value checking type template specialization
     * for lvalues, which is of true type.
     * @tparam T
     */
    template<typename T>
    struct is_lvalue_reference<T &>
            : public true_type {
    };

    /**
     * R-value checking type. Base case is false.
     */
    template<typename>
    struct is_rvalue_reference
            : public false_type {
    };

    /**
     * R-value checking type template specialization for rvalues,
     * which is of true type.
     * @tparam T
     */
    template<typename T>
    struct is_rvalue_reference<T &&>
            : public true_type {
    };

    /**
     * Checks whether a type is either an rvalue or lvalue.
     * @tparam T type to check
     */
    template<typename T>
    struct is_reference
            : public or_<is_lvalue_reference<T>, is_rvalue_reference<T>>::type {
    };

    template<typename T, typename>
    struct __remove_pointer__ {
        typedef T type;
    };

    template<typename T, typename U>
    struct __remove_pointer__<T, U *> {
        typedef U type;
    };

    template<typename T>
    struct remove_pointer
            : public __remove_pointer__<T, typename remove_cv<T>::type> {
    };

    template<typename T>
    using remove_pointer_type = typename remove_pointer<T>::type;

    /**
     * Remove reference type. Base case for non-reference types.
     * @tparam T non-reference type
     */
    template<typename T>
    struct remove_reference {
        typedef T type;
    };

    /**
     * Remove reference type. Template specialization for lvalues.
     * @tparam T lvalue type
     */
    template<typename T>
    struct remove_reference<T &> {
        typedef T type;
    };

    /**
     * Remove reference type. Template specialization for rvalues.
     * @tparam T rvalue type
     */
    template<typename T>
    struct remove_reference<T &&> {
        typedef T type;
    };

    /**
     * Shorthand for remove reference type.
     */
    template<typename T>
    using remove_reference_type = typename remove_reference<T>::type;

    /**
     * Remove extent type strips the type from
     * an array type. Base case for regular types.
     * @tparam T non-array type
     */
    template<typename T>
    struct remove_extent {
        typedef T type;
    };

    /**
     * Remove extent type template specialization for
     * fixed-sized array types.
     * @tparam T non-array type
     * @tparam size array size
     */
    template<typename T, size_type size>
    struct remove_extent<T[size]> {
        typedef T type;
    };

    /**
     * Remove extent type template specialization
     * for arrays whose sizes are not known.
     * @tparam T non-array type
     */
    template<typename T>
    struct remove_extent<T[]> {
        typedef T type;
    };

    /**
     * Metafunction checks whether a type is an object.
     * This returns true for primitives.
     * @tparam T type to check if object
     */
    template<typename T>
    struct is_object
            : public not_<or_<is_function<T>, is_reference<T>, is_void<T>>>::type {
    };

    /**
     * Metafunction checks whether a reference to
     * the provided type is legal.
     * @tparam T type to check
     */
    template<typename T>
    struct is_referenceable
            : public or_<is_object<T>, is_reference<T>>::type {
    };

    /**
     * Template specialization to check if the
     * return type of a function type and arguments type
     * is referenceable.
     * @tparam Result function type
     * @tparam Arguments argument type pack
     */
    template<typename Result, typename... Arguments>
    struct is_referenceable<Result(Arguments...)>
            : public true_type {
    };

    /**
     * Add pointer to a type. Template specialization for
     * pointer types.
     * @tparam T pointer type
     */
    template<typename T, bool = or_<is_referenceable<T>, is_void<T>>::value>
    struct __lpointer__ {
        typedef T type;
    };

    /**
     * Add pointer type. Template specialization
     * for non-pointer types.
     * @tparam T non-pointer type.
     */
    template<typename T>
    struct __lpointer__<T, true> {
        typedef typename remove_reference<T>::type *type;
    };

    /**
     * Add a pointer to a type.
     * @tparam T type to add pointer
     */
    template<typename T>
    struct add_pointer
            : public __lpointer__<T> {
    };

    /**
     * Checks if a type is an array. Base case is false.
     */
    template<typename>
    struct is_array
            : public false_type {
    };

    /**
     * Checks if a type is an array. Template specialization
     * for fixed-size arrays is true
     * @tparam T non-array type
     * @tparam _Size array size
     */
    template<typename T, size_type _Size>
    struct is_array<T[_Size]>
            : public true_type {
    };

    /**
     * Checks if a type is an array. Template specialization
     * for arrays of unknown sizes.
     * @tparam T non-array type
     */
    template<typename T>
    struct is_array<T[]>
            : public true_type {
    };

    template<typename>
    struct is_sized_array
            : public false_type {
    };

    template<typename T>
    struct is_sized_array<T[]>
            : public false_type {
    };

    template<typename T, size_type size>
    struct is_sized_array<T[size]>
            : public true_type {
    };

    template<typename>
    struct is_unsized_array
            : public false_type {
    };

    template<typename T>
    struct is_unsized_array<T[]>
            : public true_type {
    };

    template<typename T, size_type size>
    struct is_unsized_array<T[size]>
            : public false_type {
    };

    template<typename>
    struct get_array_size {
    };

    template<typename T, size_type size>
    struct get_array_size<T[size]> {
        static constexpr size_type value = size;
    };

    /**
     * Decay metafunction strips pointers and references
     * from a type, allowing comparisons of the
     * pure types.
     * @tparam U any type with any modifiers
     * @tparam IsArray whether the type is an array
     * @tparam IsFunction whether the type is a function
     */
    template<typename U,
            bool IsArray = is_array<U>::value,
            bool IsFunction = is_function<U>::value>
    struct __decay__;

    /**
     * Case neither array nor function.
     * @tparam U any type
     */
    template<typename U>
    struct __decay__<U, false, false> {
        typedef typename remove_const<U>::type type;
    };

    /**
     * Case is array.
     * @tparam U array type
     */
    template<typename U>
    struct __decay__<U, true, false> {
        typedef typename remove_extent<U>::type *type;
    };

    /**
     * Case is function.
     * @tparam U function type
     */
    template<typename U>
    struct __decay__<U, false, true> {
        typedef typename add_pointer<U>::type type;
    };

    /**
     * Decay struct strips reference and pointers
     * from a type.
     * @tparam T type to obtain pure type
     */
    template<typename T>
    class decay {
        typedef typename remove_reference<T>::type remove_type;

    public:
        typedef typename __decay__<remove_type>::type type;
    };

    /**
     * Shorthand for type decay.
     * @tparam T type to strip
     */
    template<typename T>
    using decay_type = typename decay<T>::type;

    /**
     * Enable if is a template helper that can be used
     * to disable visibility of functions and even classes
     * via overload resolution. Base case disables.
     * @tparam T
     */
    template<bool, typename T = void>
    struct enable_if {
    };

    /**
     * Template specialization for the true case,
     * which enables the template.
     * @tparam T
     */
    template<typename T>
    struct enable_if<true, T> {
        typedef T type;
    };

    /**
     * Is function base case is false.
     */
    template<typename>
    struct is_function
            : public false_type {
    };

    /**
     * Is function for argument pack is true.
     * @tparam Result function type
     * @tparam ArgTypes argument types
     */
    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes...)>
            : public true_type {
    };

    /**
     * Is function for argument lvalue pack is true.
     * @tparam Result function type
     * @tparam ArgTypes argument types
     */
    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes...) &>
            : public true_type {
    };

    /**
     * Is function for argument rvalue pack is true.
     * @tparam Result function type
     * @tparam ArgTypes argument types
     */
    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes...) &&>
            : public true_type {
    };

    /**
     * Is function for const type pack is true.
     * @tparam Result function type
     * @tparam ArgTypes argument types
     */
    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes...) const>
            : public true_type {
    };

    /**
     * Is function for const lvalue type pack.
     * @tparam Result function type
     * @tparam ArgTypes argument types
     */
    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes...) const &>
            : public true_type {
    };

    /**
     * Is function for const rvalue type pack.
     * @tparam Result function type
     * @tparam ArgTypes argument types
     */
    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes...) const &&>
            : public true_type {
    };

    /**
     * Is same checks whether two provided types are
     * the same type. Base case is false.
     */
    template<typename, typename>
    struct is_same
            : public false_type {
    };

    /**
     * Is same template specialization for the same types is true.
     * @tparam T
     */
    template<typename T>
    struct is_same<T, T>
            : public true_type {
    };

    /**
     * Recursive function applies an operator to each
     * type in a type pack.
     * @return false in the base case
     */
    template<template<class> class>
    constexpr bool is_any_of() {
        return false;
    }

    /**
     * Recursive function applies a boolean operator
     * type to each value in the type pack
     * @tparam Op a boolean operator type with a bool value
     * @tparam Head head type
     * @tparam Tail tail types
     * @return true if the operator is satisfied by any of the types
     */
    template<template<class> class Op, typename Head, typename... Tail>
    constexpr bool is_any_of() {
        return Op<Head>::value || is_any_of<Op, Tail...>();
    };

    /**
     * Add lvalue reference helper.
     *
     * @tparam T existing lvalue type
     */
    template<typename T, bool = is_referenceable<T>::value>
    struct __lvalue__ {
        typedef T type;
    };

    /**
     * Add lvalue reference helper.
     *
     * @tparam T non reference type
     */
    template<typename T>
    struct __lvalue__<T, true> {
        typedef T &type;
    };

    /**
     * Transform a type such that it is the lvalue reference
     * to that type.
     *
     * @tparam T type to add lvalue reference
     */
    template<typename T>
    struct add_lvalue_reference : public __lvalue__<T> {
    };

    /**
     * Add rvalue reference helper
     *
     * @tparam T existing rvalue type
     */
    template<typename T, bool = is_referenceable<T>::value>
    struct __rvalue__ {
        typedef T type;
    };

    /**
     * Add rvalue reference helper.
     *
     * @tparam T non reference type
     */
    template<typename T>
    struct __rvalue__<T, true> {
        typedef T &&type;
    };

    /**
     * Transform a type such that it is the rvalue reference
     * to that type.
     *
     * @tparam T type to add rvalue reference
     */
    template<typename T>
    struct add_rvalue_reference : public __rvalue__<T> {
    };

    template<typename T>
    struct add_const {
        typedef T const type;
    };

    /**
     * Declared value helper.
     *
     * @tparam T type to obtain the declared value
     */
    template<typename T>
    struct __declval__ {
        static const bool __stop__ = false;

        static typename add_rvalue_reference<T>::type __delegate__();
    };

    /**
     * Obtain a static declared value to the specified type.
     * This function should only be used in metafunction contexts
     * as the actual return value is undefined. This is usually
     * used in conjunction with @code decltype @endcode build-in function
     * to obtain the type of an unknown type.
     *
     * @tparam T type whose value to obtain
     * @return a value of the type
     */
    template<typename T>
    inline typename add_rvalue_reference<T>::type declval() noexcept {
        static_assert(__declval__<T>::__stop__, "declval is not callable");
        return __declval__<T>::__delegate__();
    }

    template<typename>
    struct __is_null_pointer_helper
            : public false_type {
    };

    template<>
    struct __is_null_pointer_helper<nullptr_t>
            : public true_type {
    };

    /**
     * checks if @p T type is a type that is a nullptr type
     *
     * @tparam T type being verified
     */
    template<typename T>
    struct is_null_pointer
            : public __is_null_pointer_helper<typename remove_cv<T>::type>::type {
    };

    /**
     * checks if @p T type is a type that is a nullptr type
     *
     * @tparam T type being verified
     */
    template<typename T>
    struct __is_nullptr_t
            : public is_null_pointer<T> {
    };

    template<typename>
    struct __is_integral_helper
            : public false_type {
    };

    template<>
    struct __is_integral_helper<bool>
            : public true_type {
    };

    template<>
    struct __is_integral_helper<char>
            : public true_type {
    };

    template<>
    struct __is_integral_helper<signed char>
            : public true_type {
    };

    template<>
    struct __is_integral_helper<unsigned char>
            : public true_type {
    };

    template<>
    struct __is_integral_helper<wchar_t>
            : public true_type {
    };

    template<>
    struct __is_integral_helper<char16_t>
            : public true_type {
    };

    template<>
    struct __is_integral_helper<char32_t>
            : public true_type {
    };

    template<>
    struct __is_integral_helper<short>
            : public true_type {
    };

    template<>
    struct __is_integral_helper<unsigned short>
            : public true_type {
    };

    template<>
    struct __is_integral_helper<int>
            : public true_type {
    };

    template<>
    struct __is_integral_helper<unsigned int>
            : public true_type {
    };

    template<>
    struct __is_integral_helper<long>
            : public true_type {
    };

    template<>
    struct __is_integral_helper<unsigned long>
            : public true_type {
    };

    template<>
    struct __is_integral_helper<long long>
            : public true_type {
    };

    template<>
    struct __is_integral_helper<unsigned long long>
            : public true_type {
    };

    /**
     * checks if @p T type is an integer
     *
     * @tparam T type being verified
     */
    template<typename T>
    struct is_integral
            : public __is_integral_helper<typename remove_cv<T>::type>::type {
    };

    template<typename>
    struct __is_floating_point_helper
            : public false_type {
    };

    template<>
    struct __is_floating_point_helper<float>
            : public true_type {
    };

    template<>
    struct __is_floating_point_helper<double>
            : public true_type {
    };

    template<>
    struct __is_floating_point_helper<long double>
            : public true_type {
    };

    template<typename>
    struct __is_pointer_helper
            : public false_type { };

    template<typename _Tp>
    struct __is_pointer_helper<_Tp*>
            : public true_type { };

    /*
     * checks if @p _Tp type is a pointer type
     *
     * @tparam _Tp type being verified
     */
    template<typename _Tp>
    struct is_pointer
            : public __is_pointer_helper<typename remove_cv<_Tp>::type>::type
    { };

    /**
     * checks if @p T type is a floating point
     *
     * @tparam T type being verified
     */
    template<typename T>
    struct is_floating_point
            : public __is_floating_point_helper<typename remove_cv<T>::type>::type {
    };

    /**
     * checks if @p T type is a type that allow arithmetic operations
     *
     * @tparam T type being verified
     */
    template<typename T>
    struct is_arithmetic
            : public or_<is_integral<T>, is_floating_point<T>>::type {
    };

    /**
     * checks if @p T type is a type that is a fundamental type and not an object
     *
     * @tparam T type being verified
     */
    template<typename T>
    struct is_fundamental
            : public or_<is_arithmetic<T>, is_void<T>,
                    is_null_pointer<T>>::type {
    };

    template<typename From, typename To, bool = or_<
            is_void<From>,
            is_function<To>,
            is_array<To>
    >::value>
    struct __convertible__ {
        typedef typename is_void<To>::type type;
    };

    template<typename From, typename To>
    struct __convertible__<From, To, false> {
    private:
        template<typename U>
        static void __aux__(U);

        template<typename F, typename T, typename = decltype(__aux__<T>(declval<F>()))>
        static true_type __test__(int);

        template<typename, typename>
        static false_type __test__(...);

    public:
        typedef decltype(__test__<From, To>(0)) type;
    };

    template<typename From, typename To>
    struct is_convertible
            : public __convertible__<From, To>::type {
    };

    template<size_type size>
    struct __storage__ {
        union __type__ {
            uint8_t __data__[size];
            struct __attribute__((__aligned__)) {
            } __align__;
        };
    };

    template<size_type size, size_type align = __alignof__(typename __storage__<size>::__type__)>
    struct aligned_storage {
        union type {
            uint8_t data[size];
            struct __attribute__((__aligned__((align)))) {
            } __align__;
        };
    };

    template<typename T>
    struct alignment_of
            : public integral_constant<size_type, __alignof__(T)> {
    };

}

#endif //EMBEDDEDCPLUSPLUS_TMP_H
