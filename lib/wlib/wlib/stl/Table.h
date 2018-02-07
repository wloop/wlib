#ifndef EMBEDDEDCPLUSPLUS_TABLECOMMON_H
#define EMBEDDEDCPLUSPLUS_TABLECOMMON_H

#include "Tuple.h"
#include "wlib/utility/Tmp.h"

namespace wlp {

    template<typename Key, typename Val>
    struct MapGetKey {
        typedef Tuple<Key, Val> element_type;
        typedef Key key_type;

        template<typename E>
        const key_type &operator()(E &&element) const {
            return get<0>(forward<E>(element));
        }
    };

    template<typename Key, typename Val>
    struct MapGetVal {
        typedef Tuple<Key, Val> element_type;
        typedef Val val_type;

        template<typename E>
        val_type &operator()(E &&element) const {
            return get<1>(forward<E>(element));
        }
    };

    template<typename Key>
    struct SetGetKey {
        typedef Key element_type;
        typedef Key key_type;

        template<typename E>
        const key_type &operator()(E &&element) const {
            return forward<E>(element);
        }
    };

    template<typename Key>
    struct SetGetVal {
        typedef Key element_type;
        typedef Key val_type;

        template<typename E>
        val_type &operator()(E &&element) const {
            return forward<E>(element);
        }
    };

}

#endif //EMBEDDEDCPLUSPLUS_TABLECOMMON_H
