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

union __float_encoder {
    static_assert(sizeof(float) == sizeof(uint32_t), "Expected float to be 32-bits");
    float r;
    uint32_t n;
};

inline uint32_t __encode_float18(float val) {
    if (val == 0.0f) {
        // hack for zero values in place of a better compression algorithm
        return 0;
    }
    __float_encoder encoder{};
    encoder.r = val;
    uint32_t t = (encoder.n & 0x7ff800) >> 11;
    t |= (((encoder.n & 0x7f800000) >> 23) - 0x70) << 12;
    t |= (encoder.n & 0x80000000) >> 14;
    return t;
}

inline uint64_t __join_data(uint32_t bits1, uint32_t bits2, uint32_t bits3) {
    return static_cast<uint64_t>(bits1) | (static_cast<uint64_t>(bits2) << 18) | (static_cast<uint64_t>(bits3) << 36);
}

inline void __set_packet_type(packet64 &packet, const packet_type &type) {
    *packet.data() |= *type.data();
}

inline void __set_packet_name(packet64 &packet, packet_name name) {
    *packet.data() |= *name.data() << 3;
}

inline void __set_bulk_data(packet64 &packet, uint64_t joined_bits) {
    *packet.data() |= static_cast<uint32_t>(joined_bits << 10);
    *(packet.data() + 1) |= static_cast<uint32_t>(joined_bits >> 22);
}

packet64 __packet_maker::build(const float data[3], const packet_type &type, const packet_name &name) {
    packet64 packet;
    // type and name are less than or equal to a byte
    // so byte order does not matter
    __set_packet_type(packet, type);
    __set_packet_name(packet, name);
    __set_bulk_data(packet, __join_data(
            __encode_float18(data[0]),
            __encode_float18(data[1]),
            __encode_float18(data[2])
    ));
    return packet;
}
