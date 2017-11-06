#ifndef EMBEDDEDCPLUSPLUS_TMP_H
#define EMBEDDEDCPLUSPLUS_TMP_H

#include <stdint.h>

namespace wlp {

    typedef uint16_t size_type;


    template<typename>
    struct is_function;

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

    template<typename T, T t>
    constexpr T integral_constant<T, t>::value;

    typedef integral_constant<bool, true> true_type;
    typedef integral_constant<bool, false> false_type;

    template<bool t>
    using boolean_constant = integral_constant<bool, t>;

    template<bool, typename, typename>
    struct conditional;

    template<bool Condition, typename IfTrue, typename IfFalse>
    struct conditional {
        typedef IfTrue type;
    };

    template<typename IfTrue, typename IfFalse>
    struct conditional<false, IfTrue, IfFalse> {
        typedef IfFalse type;
    };

    template<typename T>
    struct remove_const {
        typedef T type;
    };

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
    struct remove_const_volatile {
        typedef typename remove_const<typename remove_volatile<T>::type>::type type;
    };

    template<typename...>
    struct or_;

    template<>
    struct or_<>
            : public false_type {
    };

    template<typename B1>
    struct or_<B1>
            : public B1 {
    };

    template<typename B1, typename B2>
    struct or_<B1, B2>
            : public conditional<B1::value, B1, B2>::type {
    };

    template<typename B1, typename B2, typename B3>
    struct or_<B1, B2, B3>
            : public conditional<B1::value, B1, or_<B2, B3>>::type {
    };

    template<typename T>
    struct not_
            : public integral_constant<bool, !T::value> {
    };

    template<typename>
    struct __void__
            : public false_type {
    };

    template<>
    struct __void__<void>
            : public true_type {
    };

    template<typename T>
    struct is_void
            : public __void__<typename remove_const_volatile<T>::type>::type {
    };

    template<typename>
    struct is_lvalue_reference
            : public false_type {
    };

    template<typename T>
    struct is_lvalue_reference<T &>
            : public true_type {
    };

    template<typename>
    struct is_rvalue_reference
            : public false_type {
    };

    template<typename T>
    struct is_rvalue_reference<T &&>
            : public true_type {
    };

    template<typename T>
    struct is_reference
            : public or_<is_lvalue_reference<T>, is_rvalue_reference<T>>::type {
    };

    template<typename T>
    struct remove_reference {
        typedef T type;
    };

    template<typename T>
    struct remove_reference<T &> {
        typedef T type;
    };

    template<typename T>
    struct remove_reference<T &&> {
        typedef T type;
    };

    template<typename T>
    using remove_reference_type = typename remove_reference<T>::type;

    template<typename T>
    struct remove_extent {
        typedef T type;
    };

    template<typename T, size_type size>
    struct remove_extent<T[size]> {
        typedef T type;
    };

    template<typename T>
    struct remove_extent<T[]> {
        typedef T type;
    };

    template<typename T>
    struct is_object
            : public not_<or_<is_function<T>, is_reference<T>, is_void<T>>>::type {
    };

    template<typename T>
    struct is_referenceable
            : public or_<is_object<T>, is_reference<T>>::type {
    };

    template<typename Result, typename... Arguments>
    struct is_referenceable<Result(Arguments...)>
            : public true_type {
    };

    template<typename Result, typename... Arguments>
    struct is_referenceable<Result(Arguments......)>
            : public true_type {
    };

    template<typename T, bool = or_<is_referenceable<T>, is_void<T>>::value>
    struct __lpointer__ {
        typedef T type;
    };

    template<typename T>
    struct __lpointer__<T, true> {
        typedef typename remove_reference<T>::type *type;
    };

    template<typename T>
    struct add_pointer
            : public __lpointer__<T> {
    };

    template<typename>
    struct is_array
            : public false_type {
    };

    template<typename T, std::size_t _Size>
    struct is_array<T[_Size]>
            : public true_type {
    };

    template<typename T>
    struct is_array<T[]>
            : public true_type {
    };

    template<typename U,
            bool IsArray = is_array<U>::value,
            bool IsFunction = is_function<U>::value>
    struct __decay__;

    template<typename U>
    struct __decay__<U, false, false> {
        typedef typename remove_const_volatile<U>::type __type;
    };

    template<typename _Up>
    struct __decay__<_Up, true, false> {
        typedef typename remove_extent<_Up>::type *__type;
    };

    template<typename _Up>
    struct __decay__<_Up, false, true> {
        typedef typename add_pointer<_Up>::type __type;
    };

    template<typename T>
    class decay {
        typedef typename remove_reference<T>::type __remove_type;

    public:
        typedef typename __decay__<__remove_type>::__type type;
    };


    template<typename T>
    constexpr typename remove_reference<T>::type &&move(T &&t) noexcept {
        return static_cast<typename remove_reference<T>::type &&>(t);
    }

    template<typename T>
    constexpr T &&forward(typename remove_reference<T>::type &t) noexcept {
        return static_cast<T &&>(t);
    }

    template<typename T>
    constexpr T &&forward(typename remove_reference<T>::type &&t) noexcept {
        return static_cast<T &&>(t);
    }

    template<bool, typename T = void>
    struct enable_if {
    };

    template<typename T>
    struct enable_if<true, T> {
        typedef T type;
    };

    template<typename>
    struct is_function
            : public false_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes...)>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes...) &>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes...) &&>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes......)>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes......) &>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes......) &&>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes...) const>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes...) const &>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes...) const &&>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes......) const>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes......) const &>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes......) const &&>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes...) volatile>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes...) volatile &>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes...) volatile &&>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes......) volatile>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes......) volatile &>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes......) volatile &&>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes...) const volatile>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes...) const volatile &>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes...) const volatile &&>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes......) const volatile>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes......) const volatile &>
            : public true_type {
    };

    template<typename Result, typename... ArgTypes>
    struct is_function<Result(ArgTypes......) const volatile &&>
            : public true_type {
    };

    template<typename, typename>
    struct is_same
            : public false_type {
    };

    template<typename T>
    struct is_same<T, T>
            : public true_type {
    };

    template<template<class> typename>
    constexpr bool is_any_of() {
        return false;
    }

    template<template<class> typename Op, typename Head, typename... Tail>
    constexpr bool is_any_of() {
        return Op<Head>::value || is_any_of<Op, Tail...>();
    };

}

#endif //EMBEDDEDCPLUSPLUS_TMP_H
