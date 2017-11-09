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
    void FloatsToBin();
    void SetStartAndEnd();

// MEMBER VARIABLES
private:
    std::string type, name;
    float *data = new float [3];
    Bitset<64> array;
};

#endif //EMBEDDEDCPLUSPLUS_PACKETCONSTRUCTOR_H
