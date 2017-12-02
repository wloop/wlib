//
// Created by Heather D'Souza on 2017-11-29.
//

#include "comm/PacketConstructor.h"
#include "../../include/gtest-1.8.0/include/gtest/gtest.h"

using namespace wlp;

class TestPacket : public PacketConstructor {
public:
    TestPacket(const float* p_data, const Bitset<2> &packetType, const Bitset<6> &packetName) : PacketConstructor(p_data, packetType, packetName) {}

    void getFromParent(float *data) {
        get(data);
    }
};

float *data = new float [3];
Bitset<2> type1;
Bitset<6> name;
Bitset<1> test;

TEST(packet_test, test_getBitsetNum){
    TestPacket tp(data,type1,name) {};
    ASSERT_EQ(0, tp.getBitsetNum());
}

TEST(packet_test, test_setStartAndEnd) {
    TestPacket tp(data,type1,name) {};
    ASSERT_EQ(0, tp.setStartAndEnd());
}

