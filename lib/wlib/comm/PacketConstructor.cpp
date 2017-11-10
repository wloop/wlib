//
// Created by Heather D'Souza on 2017-11-07.
//

#include "../stl/Bitset.h"

#include <iostream>
#include "PacketConstructor.h"
#include <math.h>
#include <cmath>

PacketConstructor::PacketConstructor()
{
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
}

PacketConstructor::~PacketConstructor()
{
    delete data;
}

PacketConstructor::PacketConstructor(float* data)
{
    this->data = data;
    SetStartAndEnd();
    DataToBin();
}

void PacketConstructor::SetStartAndEnd() {
    array.set(0);
    array.set(63);
}

void PacketConstructor::IntToBin(int c, int pos)
{
    //bit assignment
    for(int i = 0; i < 10; i++)
        ((1<<i) & c) != 0 ? array.set(pos+i):array.reset(pos+i);
}

void PacketConstructor::FloatToBin(int c, int pos)
{
    std::cout << c << std::endl;
    //bit assignment
    for(int i = 0; i < 7; i++) {
        if (1 << i & c) {
            array.set(pos + i);
        }
        else {
            array.reset(pos + i);
        }
    }
}

void PacketConstructor::DataToBin()
{
    int intPart, floatPart;
    int posArray [] = {10,20,28,38,46,56};

    for (int j=0;j<3;j++) {
        //negative or positive bit
        if (data[j] < 0) {
            array.set(posArray[j*2] - 1);
            data[j] = data[j] * (-1);
        }
        else {
            array.reset(posArray[j*2]-1);
        }

        //separate integer and decimal part of float;
        intPart = static_cast<int>(data[j]);
        floatPart = abs(nearbyint((((data[j] - static_cast<float>(intPart)) * 100))));

        IntToBin(intPart, posArray[j*2]);
        FloatToBin(floatPart, posArray[j*2+1]);
    }
}

Bitset<64> PacketConstructor::GetArray() {
    return array;
}




