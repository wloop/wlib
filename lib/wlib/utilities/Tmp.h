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

/*
 * TODO: Robust is_function, and_, recursive or_
 */

#ifndef EMBEDDEDCPLUSPLUS_TMP_H
#define EMBEDDEDCPLUSPLUS_TMP_H

#include "../Types.h"

namespace wlp {

#if __cplusplus >= 201103L
    typedef decltype(nullptr) nullptr_t;
#endif

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
     * @tparam B1 boolean condition
     */
    template<typename B1>
    struct or_<B1>
            : public B1 {
    };

    /**
     * Template specialization of binary or.
     * @tparam B1 first boolean condition
     * @tparam B2 second boolean condition
     */
    template<typename B1, typename B2>
    struct or_<B1, B2>
            : public conditional<B1::value, B1, B2>::type {
    };

    template<typename B1, typename B2, typename B3, typename... Bn>
    struct or_<B1, B2, B3, Bn...>
            : public conditional<B1::value, B1, or_<B2, B3, Bn...>>::type {
    };

    template<typename...>
    struct and_;

    template<>
    struct and_<>
            : public true_type {
    };

    template<typename B1>
    struct and_<B1>
            : public B1 {
    };

    template<typename B1, typename B2>
    struct and_<B1, B2>
            : public conditional<B1::value, B2, B1>::type {
    };

    template<typename B1, typename B2, typename B3, typename... Bn>
    struct and_<B1, B2, B3, Bn...>
            : public conditional<B1::value, and_<B2, B3, Bn...>, B1>::type {
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

    template<typename _Tp>
    struct remove_volatile {
        typedef _Tp type;
    };

    template<typename _Tp>
    struct remove_volatile<_Tp volatile> {
        typedef _Tp type;
    };

    template<typename _Tp>
    struct remove_cv {
        typedef typename
        remove_const<typename remove_volatile<_Tp>::type>::type type;
    };

    template<typename>
    struct __is_null_pointer_helper
            : public false_type {
    };

    template<>
    struct __is_null_pointer_helper<nullptr_t>
            : public true_type {
    };

    /**
     * checks if @p _Tp type is a type that is a nullptr type
     *
     * @tparam _Tp type being verified
     */
    template<typename _Tp>
    struct is_null_pointer
            : public __is_null_pointer_helper<typename remove_cv<_Tp>::type>::type {
    };

    /**
     * checks if @p _Tp type is a type that is a nullptr type
     *
     * @tparam _Tp type being verified
     */
    template<typename _Tp>
    struct __is_nullptr_t
            : public is_null_pointer<_Tp> {
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

#ifdef _GLIBCXX_USE_WCHAR_T
    template<>
    struct __is_integral_helper<wchar_t>
    : public true_type { };
#endif

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
     * checks if @p _Tp type is an integer
     *
     * @tparam _Tp type being verified
     */
    template<typename _Tp>
    struct is_integral
            : public __is_integral_helper<typename remove_cv<_Tp>::type>::type {
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

#if !defined(__STRICT_ANSI__) && defined(_GLIBCXX_USE_FLOAT128)
    template<>
    struct __is_floating_point_helper<__float128>
    : public true_type { };
#endif

    /**
     * checks if @p _Tp type is a floating point
     *
     * @tparam _Tp type being verified
     */
    template<typename _Tp>
    struct is_floating_point
            : public __is_floating_point_helper<typename remove_cv<_Tp>::type>::type {
    };

    /**
     * checks if @p _Tp type is a type that allow arithmetic operations
     *
     * @tparam _Tp type being verified
     */
    template<typename _Tp>
    struct is_arithmetic
            : public or_<is_integral<_Tp>, is_floating_point<_Tp>>::type {
    };

    /**
     * checks if @p _Tp type is a type that is a fundamental type and not an object
     *
     * @tparam _Tp type being verified
     */
    template<typename _Tp>
    struct is_fundamental
            : public or_<is_arithmetic<_Tp>, is_void<_Tp>,
                    is_null_pointer<_Tp>>::type {
    };
}

#endif //EMBEDDEDCPLUSPLUS_TMP_H
