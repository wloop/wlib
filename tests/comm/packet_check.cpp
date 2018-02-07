#include <gtest/gtest.h>
#include <wlib/comm/PacketConstructor.h>

using namespace wlp;

TEST(packet_constructor_test, test_make_packet_all_zero_data) {
    float data[3] = {0, 0, 0};
    packet64 packet = make_packet64(data, PacketType::SENSOR, 0);
    ASSERT_EQ(0u, packet.data()[0]);
    ASSERT_EQ(0u, packet.data()[1]);
}

TEST(packet_constructor_test, test_make_packet_sets_type) {
    float data[3] = {0, 0, 0};
    int types[4] = {PacketType::SENSOR, PacketType::COMMAND, PacketType::STATE, PacketType::LOG};
    uint64_t expected[4] = {0, 1, 2, 3};
    for (int i = 0; i < 4; i++) {
        packet64 packet = make_packet64(data, types[i], 0);
        ASSERT_EQ(expected[i], packet.to_uint64());
    }
}

TEST(packet_constructor_test, test_make_packet_sets_name) {
    float data[3] = {0, 0, 0};
    uint8_t names[] = {0, 12, 63, 22, 53, 110};
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(static_cast<uint64_t>(names[i] << 3), make_packet64(data, 0, names[i]).to_uint64());
    }
}

TEST(packet_constructor_test, test_make_packet_sets_data) {
    float data[3] = {-724.99f, 846.53f, 442.59f};
    uint64_t expected = 6839376459708669362u;
    packet64 packet = make_packet64(data, PacketType::STATE, 54);
    ASSERT_EQ(expected, packet.to_uint64());
}
