/** @file PacketConstructor.cpp
 *  @brief Creates a packet for sending data from the slave systems to the main computer.
 *
 *  Implements the functions defined in PacketConstructor.h.
 *
 *  @author Heather D'Souza (heathkd)
 *  @date November 7, 2017
 *  @bug No known bugs.
 */

#include <stdlib.h>
//#include "../stl/Bitset.h"
#include "PacketConstructor.h"
#include <iostream>

PacketConstructor::PacketConstructor(const float* p_data, const Bitset<2> &packetType, const Bitset<6> &packetName)
        : m_pdata{p_data}, m_packetType{packetType}, m_packetName{packetName}  {
    m_pdata = p_data;
}
/*
const float* PacketConstructor::getData() const {
    return m_pdata;
}

//returns current Bitset
Bitset<64> PacketConstructor::getBitArray() {
    return m_bitArray;
}*/

void PacketConstructor::setStartAndEnd() {
    m_bitArray.set(0);
    m_bitArray.set(63);
}

void PacketConstructor::dataToBin()
{
    uint16_t intPart, floatPart;
    uint16_t posArray [] = {10,20,28,38,46,56};
    float temp;

    for (int j=0;j<3;j++) {
        //negative or positive bit
        if (m_pdata[j] < 0) {
            m_bitArray.set(posArray[j*2] - 1);
            temp = m_pdata[j] * (-1);
        }
        else {
            m_bitArray.reset(posArray[j*2]-1);
            temp = m_pdata[j];
        }

        //separate integer and decimal part of float;
        intPart = static_cast<int>(temp);
        floatPart = abs(nearbyint((((temp - static_cast<float>(intPart)) * 100))));

        intToBin(intPart, posArray[j*2]);
        floatToBin(floatPart, posArray[j*2+1]);
        std::cout << m_pdata[j] << std::endl;
        std::cout << temp << std::endl;

    }
}

void PacketConstructor::intToBin(int num, int pos) {
    //bit assignment
    for(int i = 0; i < 10; i++)
        ((1<<i) & num) != 0 ? m_bitArray.set(pos+i):m_bitArray.reset(pos+i);
    std::cout << num << std::endl;
}

void PacketConstructor::floatToBin(int num, int pos) {
    //bit assignment
    for(int i = 0; i < 7; i++)
        ((1<<i) & num) != 0 ? m_bitArray.set(pos+i):m_bitArray.reset(pos+i);

    std::cout << num << std::endl;
}

uint64_t PacketConstructor::getBitsetNum() {
    return m_bitArray.to_uint64_t();
}


Bitset<64> PacketConstructor::get(float *data) {
    m_pdata = data;
    setStartAndEnd();
    setTypeAndName();
    dataToBin();
}

void PacketConstructor::setTypeAndName() {
    Bitset<1> zero;
    zero.reset(0);
    for (int i=1;i<3;i++) {
        if (!(m_packetType.test(0)==zero.test(0))) {
            m_bitArray.set(i);
        }
    }
    for (int i=3;i<9;i++) {
        if (!(m_packetName.test(0)==zero.test(0))) {
            m_bitArray.set(i);
        }
    }
}










