#ifndef EMBEDDEDCPLUSPLUS_CONCEPTCHECKS_H
#define EMBEDDEDCPLUSPLUS_CONCEPTCHECKS_H

#include <utility>
#include "Tmp.h"
#include "TypeTraits.h"

namespace wlp {

    template<typename, typename T>
    struct comparator_concept {
        static constexpr bool value = false;
    };

    template<typename C, typename Ret, typename... Args>
    struct comparator_concept<C, Ret(Args...)> {
    private:
        template<typename T>
        static constexpr auto check(T*) -> typename and_<
                is_same<decltype(declval<T>().__lt__(declval<Args>()...)), Ret>,
                is_same<decltype(declval<T>().__le__(declval<Args>()...)), Ret>,
                is_same<decltype(declval<T>().__eq__(declval<Args>()...)), Ret>,
                is_same<decltype(declval<T>().__ne__(declval<Args>()...)), Ret>,
                is_same<decltype(declval<T>().__gt__(declval<Args>()...)), Ret>,
                is_same<decltype(declval<T>().__ge__(declval<Args>()...)), Ret>
        >::type;

        template<typename>
        static constexpr false_type check(...);

        typedef decltype(check<C>(0)) type;

    public:
        static constexpr bool value = type::value;
    };

    template<typename C, typename T>
    static constexpr bool is_comparator() {
        return comparator_concept<C, bool(const T &, const T&)>::value;
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
        static constexpr auto check(T*) -> typename and_<
            is_same<decltype(declval<T>().operator*()), val_type &>,
            is_same<decltype(declval<T>().operator->()), val_type *>,
            is_same<decltype(declval<T>().operator++()), iterator &>,
            is_same<decltype(declval<T>().operator++(declval<int>())), iterator>,
            is_same<decltype(declval<T>().operator--()), iterator &>,
            is_same<decltype(declval<T>().operator--(declval<int>())), iterator>,
            is_same<decltype(declval<T>().operator==(declval<iterator &>())), bool>,
            is_same<decltype(declval<T>().operator==(declval<const iterator &>())), bool>,
            is_same<decltype(declval<T>().operator!=(declval<iterator &>())), bool>,
            is_same<decltype(declval<T>().operator!=(declval<const iterator &>())), bool>,
            is_same<decltype(declval<T>().operator=(declval<iterator &>())), iterator &>,
            is_same<decltype(declval<T>().operator=(declval<const iterator &>())), iterator &>,
            is_same<decltype(declval<T>().operator+(declval<const size_type &>())), iterator>,
            is_same<decltype(declval<T>().operator+(declval<size_type &&>())), iterator>,
            is_same<decltype(declval<T>().operator-(declval<const size_type &>())), iterator>,
            is_same<decltype(declval<T>().operator-(declval<size_type &&>())), iterator>,
            is_same<decltype(declval<T>().operator-(declval<const iterator &>())), size_type>,
            is_same<decltype(declval<T>().operator-(declval<iterator &&>())), size_type>,
            is_same<decltype(declval<T>().operator-=(declval<const size_type &>())), iterator &>,
            is_same<decltype(declval<T>().operator-=(declval<size_type &&>())), iterator &>,
            is_same<decltype(declval<T>().operator+=(declval<const size_type &>())), iterator &>,
            is_same<decltype(declval<T>().operator+=(declval<size_type &&>())), iterator &>
        >::type;

        template<typename>
        static constexpr false_type check(...);

        typedef decltype(check<C>(0)) type;

    public:
        static constexpr bool value = type::value;
    };

    template<typename C>
    static constexpr bool is_random_access_iterator() {
        return random_access_iterator_concept<C>::value;
    }

}

#endif //EMBEDDEDCPLUSPLUS_CONCEPTCHECKS_H
