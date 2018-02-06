/**
 * @file PacketConstructor.h
 * @brief
 *
 * @author Heather D'Souza
 * @author Jeff Niu
 * @date December 2, 2017
 * @bug No known bugs.
 */

#ifndef EMBEDDEDCPLUSPLUS_PACKETCONSTRUCTOR_H
#define EMBEDDEDCPLUSPLUS_PACKETCONSTRUCTOR_H

#include "wlib/Types.h"
#include "wlib/stl/Bitset.h"

namespace wlp {

    typedef Bitset<64> packet64;
    typedef Bitset<3> packet_type;
    typedef Bitset<7> packet_name;

    struct __packet_maker {
        static packet64 build(const float data[3], const packet_type &type, const packet_name &name);
    };

    struct PacketType {
        enum {
            SENSOR, COMMAND, STATE, LOG, NUM_TYPES
        };
    };

    inline packet_type __type_bits_from_enum(int type_enum) {
        packet_type type;
        switch (type_enum) {
            case PacketType::SENSOR:
                break;
            case PacketType::COMMAND:
                *type.data() |= 1;
                break;
            case PacketType::STATE:
                *type.data() |= 2;
                break;
            case PacketType::LOG:
                *type.data() |= 3;
                break;
            default:
                break;
        }
        return type;
    }

    inline packet64 make_packet64(
            const float data[3],
            const int &type,
            const uint8_t &name_chr
    ) {
        packet_name name;
        *name.data() |= name_chr;
        return __packet_maker::build(data, __type_bits_from_enum(type), name);
    }

}

#endif // EMBEDDEDCPLUSPLUS_PACKETCONSTRUCTOR_H



