#ifndef EMBEDDEDCPLUSPLUS_TYPETRAITS_H
#define EMBEDDEDCPLUSPLUS_TYPETRAITS_H

namespace wlp {

    template<class Iterator>
    struct IteratorValueType {
        static const Iterator::val_type *type;
    };

    template<class Iterator>
    struct IteratorSizeType {
        static const Iterator::size_type *type;
    };

}

#endif //EMBEDDEDCPLUSPLUS_TYPETRAITS_H
