#ifndef EMBEDDEDCPLUSPLUS_CONCEPTCHECKS_H
#define EMBEDDEDCPLUSPLUS_CONCEPTCHECKS_H

namespace wlp {

    template<typename, typename T>
    struct comparator_concept {
        static_assert(integral_constant)
    };

}

#endif //EMBEDDEDCPLUSPLUS_CONCEPTCHECKS_H
