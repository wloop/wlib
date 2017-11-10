//
// Created by Heather D'Souza on 2017-11-07.
//

#ifndef EMBEDDEDCPLUSPLUS_PACKETCONSTRUCTOR_H
#define EMBEDDEDCPLUSPLUS_PACKETCONSTRUCTOR_H

#include <string>
#include "../stl/Bitset.h"
using namespace wlp;

class PacketConstructor
{

// CONSTRUCTOR/DESTRUCTOR
public:
    // Create a new Packet
    PacketConstructor(); //need to have default constructor with no parameters
    PacketConstructor(float *data);
    // Destroy this packet freeing all dynamically allocated memory.
    ~PacketConstructor();

// ACCESSORS (methods that get information)


// MUTATORS (methods that change things)

// HELPER
    void IntToBin(int c, int pos);
    void FloatToBin(int c, int pos);
    void DataToBin();
    void SetStartAndEnd();
    virtual void SetTypeAndName()=0;
    Bitset<64> GetArray();

// MEMBER VARIABLES
public:
    float *data = new float [3];
    Bitset<64> array;
};

class CommandPacket : public PacketConstructor {

    using PacketConstructor::PacketConstructor;
    void SetTypeAndName() override {
        array.reset(1);
        array.set(2);
    }
};

class StatePacket : public PacketConstructor {

    public:

        StatePacket(float *data) : PacketConstructor(data) {
            SetTypeAndName();
        }
    void SetTypeAndName() override {
        array.set(1);
        array.reset(2);
    }
};

class LogPacket : public PacketConstructor {

    using PacketConstructor::PacketConstructor;
    void SetTypeAndName() override {
        array.set(1);
        array.set(2);
    }
};

#endif //EMBEDDEDCPLUSPLUS_PACKETCONSTRUCTOR_H