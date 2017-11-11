/** @file PacketConstructor.c
 *  @brief Creates a packet for sending data from the slave systems to the main computer.
 *
 *  Implements the functions defined in PacketConstructor.h.
 *
 *  @author Heather D'Souza (heathkd)
 *  @date November 7, 2017
 *  @bug No known bugs.
 */

#include "../stl/Bitset.h"
#include <iostream>
#include "PacketConstructor.h"
#include <math.h>
#include <cmath>

PacketConstructor::PacketConstructor() {
    m_pdata[0] = 0;
    m_pdata[1] = 0;
    m_pdata[2] = 0;
}

PacketConstructor::~PacketConstructor() {
    delete m_pdata;
}

PacketConstructor::PacketConstructor(float* p_data) {
    m_pdata = p_data;
    setStartAndEnd();
    dataToBin();
}

float* PacketConstructor::getData() {
    return m_pdata;
}

Bitset<64> PacketConstructor::getBitArray() {
    return m_bitArray;
}

void PacketConstructor::setStartAndEnd() {
    m_bitArray.set(0);
    m_bitArray.set(63);
}

void PacketConstructor::dataToBin()
{
    int intPart, floatPart;
    int posArray [] = {10,20,28,38,46,56};

    for (int j=0;j<3;j++) {
        //negative or positive bit
        if (m_pdata[j] < 0) {
            m_bitArray.set(posArray[j*2] - 1);
            m_pdata[j] = m_pdata[j] * (-1);
        }
        else {
            m_bitArray.reset(posArray[j*2]-1);
        }

        //separate integer and decimal part of float;
        intPart = static_cast<int>(m_pdata[j]);
        floatPart = abs(nearbyint((((m_pdata[j] - static_cast<float>(intPart)) * 100))));

        intToBin(intPart, posArray[j*2]);
        floatToBin(floatPart, posArray[j*2+1]);
    }
}

void PacketConstructor::intToBin(int num, int pos) {
    //bit assignment
    for(int i = 0; i < 10; i++)
        ((1<<i) & num) != 0 ? m_bitArray.set(pos+i):m_bitArray.reset(pos+i);
}

void PacketConstructor::floatToBin(int num, int pos) {
    //bit assignment
    for(int i = 0; i < 7; i++)
        ((1<<i) & num) != 0 ? m_bitArray.set(pos+i):m_bitArray.reset(pos+i);
}








