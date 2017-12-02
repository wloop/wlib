/**
 * @file PacketConstructor.cpp
 * @brief Creates a packet for sending data from the slave systems to the main computer.
 *
 * Implements the functions defined in PacketConstructor.h.
 *
 * @author Heather D'Souza
 * @author Jeff Niu
 * @date December 2, 2017
 * @bug No known bugs
 */

#include "PacketConstructor.h"

using namespace wlp;

inline uint32_t __encode_float18(float val) {
    uint32_t sign = 0;
    if (val < 0) {
        sign = 1;
        val = -val;
    }
    uint16_t int_part = static_cast<uint16_t>(val);
    uint16_t float_part = static_cast<uint16_t>(floorf(((val - int_part) * 100) + 0.5f));
    uint32_t res = (sign << 17) | (int_part << 7) | float_part;
    return res;
}

inline uint64_t __join_data(uint32_t bits1, uint32_t bits2, uint32_t bits3) {
    return static_cast<uint64_t>(bits1) | (static_cast<uint64_t>(bits2) << 18) | (static_cast<uint64_t>(bits3) << 36);
}

inline void __set_start_and_end(packet64 &packet) {
    packet.set(0);
    packet.set(63);
}

inline void __set_packet_type(packet64 &packet, const packet_type &type) {
    *packet.data() |= *type.data() << 1;
}

inline void __set_packet_name(packet64 &packet, packet_name name) {
    *packet.data() |= *name.data() << 3;
}

inline void __set_bulk_data(packet64 &packet, uint64_t joined_bits) {
    *packet.data() |= static_cast<uint32_t>(joined_bits << 9);
    *(packet.data() + 1) |= static_cast<uint32_t>(joined_bits >> 23);
}

packet64 __packet_maker::build(const float data[3], const packet_type &type, const packet_name &name) {
    packet64 packet;
    __set_start_and_end(packet);
    __set_packet_type(packet, type);
    __set_packet_name(packet, name);
    __set_bulk_data(packet, __join_data(
            __encode_float18(data[0]),
            __encode_float18(data[1]),
            __encode_float18(data[2])
    ));
    return packet;
}
