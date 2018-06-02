#ifndef EMBEDDEDCPLUSPLUS_COMPARE_H
#define EMBEDDEDCPLUSPLUS_COMPARE_H

namespace wlp {

    template<typename comp_t, comp_t target, comp_t head, comp_t ...tail>
    struct is_one_of_ct {
        static const bool value = target == head || is_one_of_ct<comp_t, target, tail...>::value;
    };

    template<typename comp_t, comp_t target, comp_t head>
    struct is_one_of_ct<comp_t, target, head> {
        static const bool value = target == head;
    };

    template<typename comp_t, comp_t target, comp_t head, comp_t ...tail>
    struct isnt_any_of_ct {
        static const bool value = target != head && isnt_any_of_ct<comp_t, target, tail...>::value;
    };

    template<typename comp_t, comp_t target, comp_t head>
    struct isnt_any_of_ct<comp_t, target, head> {
        static const bool value = target != head;
    };

    template<typename comp_t, comp_t head, comp_t ...tail>
    struct is_one_of {
        static bool apply(comp_t target) {
            return target == head || is_one_of<comp_t, tail...>::apply(target);
        }
    };

    template<typename comp_t, comp_t head>
    struct is_one_of<comp_t, head> {
        static bool apply(comp_t target) {
            return target == head;
        }
    };

    template<typename comp_t, comp_t head, comp_t ...tail>
    struct isnt_any_of {
        static bool apply(comp_t target) {
            return target != head && isnt_any_of<comp_t, tail...>::apply(target);
        }
    };

    template<typename comp_t, comp_t head>
    struct isnt_any_of<comp_t, head> {
        static bool apply(comp_t target) {
            return target != head;
        }
    };

}

#endif //EMBEDDEDCPLUSPLUS_COMPARE_H
