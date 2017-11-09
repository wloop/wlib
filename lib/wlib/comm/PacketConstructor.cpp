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
    array.setFromNumber(0);
    //type="00";
    //name="000000";
    data[0]=0;
    data[1]=0;
    data[2]=0;
}

PacketConstructor::~PacketConstructor()
{

}

PacketConstructor::PacketConstructor(float* data)
{
    this->data = data;
    type="00";
    name="000000";
}

void PacketConstructor::IntToBin(int c, int pos)
{
    //bit assignment
    for(int i = 0; i < 7; i++)
        ((1<<i) & c) != 0 ? array.set(pos+i):array.reset(pos+i);

    for(int i=0;i<64;i++)
        std::cout << array[i];

}

void PacketConstructor::FloatToBin(int c, int pos)
{
    //bit assignment
   for(int i = 0; i < 10; i++)
        ((1<<i) & c) != 0 ? array.set(pos+i):array.reset(pos+i);

}

void PacketConstructor::FloatsToBin()
{
    int intPart, floatPart;
    int posArray [] = {11,21,29,39,47,57};

    //for (int i=0;i<sizeof(posArray);i=i+2)
    for (int j=0;j<3;j++) {
        //negative or positive bit
        (data[j] < 0) ? array.set(posArray[j*2-1]):array.set(posArray[j*2-1]);
        data[j] = abs(data[j]);

        //separate integer and decimal part of float; problem is you are implicitly converting int and float to char
        intPart = static_cast<int>(data[j]);
        floatPart = abs(nearbyint((((data[j] - static_cast<float>(intPart)) * 100) + 0.5)));

        IntToBin(intPart, posArray[j*2]);
        FloatToBin(floatPart, posArray[j*2+1]);
    }
    for (int i=0;i<64;i++) {
        std::cout << array[i]; }

}

