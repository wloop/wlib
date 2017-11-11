/** @file PacketConstructor.h
 *  @brief Creates a packet for sending data from the slave systems to the main computer.
 *
 *  PacketConstructor is an abstract class. Therefore, it cannot be instantiated.
 *  Subclasses include SensorPacket, CommandPacket, StatePacket, and LogPacket.
 *
 *  @author Heather D'Souza (heathkd)
 *  @date November 7, 2017
 *  @bug No known bugs.
 */

#ifndef EMBEDDEDCPLUSPLUS_PACKETCONSTRUCTOR_H
#define EMBEDDEDCPLUSPLUS_PACKETCONSTRUCTOR_H

#include <string>
#include "../stl/Bitset.h"

using namespace wlp;

/**
 *  @brief Blueprint for a packet of type Bitset. Cannot be instatiated.
*/
class PacketConstructor {

public:
    PacketConstructor();
    PacketConstructor(float *p_data); // Create a new Packet
    ~PacketConstructor(); // Destroy this packet freeing all dynamically allocated memory.

    //Helper Methods
    /** @brief Sets the start and end bits.
     *
     * Each packet should begin and end with a '1' bit so that
     * parser can distinguish the end of one packet and the
     * beginning of another.
     *
     * @return Void.
    */
    void setStartAndEnd();
    /** @brief Sets the type and name of the packet.
     *
     * There are 4 types of packets: sensor, command, state, and log.
     * This method sets the bit representation of these types and the name
     * pertaining to the specific packet. This function must be defined in each
     * subclass of PacketConstructor so that the child class may be instantiated.
     *
     * @return Void.
    */
    virtual void setTypeAndName()=0;
    /** @brief Converts data points addressed by the float pointer to bits.
     *
     * Splits each data point into its integer and decimal parts. It send the integer
     * part to intToBin() and the decimal part to floatToBin().
     *
     * @return Void.
    */
    void dataToBin();
    /** @brief Converts the integer portion of a float to its 10 bit representation.
     *
     * @param num the integer portion of the float
     * @param pos the position at which to set bits in the bit array
     *
     * @return Void.
    */
    void intToBin(int num, int pos);
    /** @brief Converts the decimal portion of a float to its 7 bit representation.
     *
     * @param num the decimal portion of the float
     * @param pos the position at which to set bits in the bit array
     *
     * @return Void.
    */
    void floatToBin(int num, int pos);
    /** @brief Accessor method for the data pointer
     *
     * @return data pointer
    */
    float* getData();
    /** @brief Accessor method for the bit array
     *
     * @return bit array
    */
    Bitset<64> getBitArray();

private:
    float *m_pdata = new float [3];
    Bitset<64> m_bitArray;
};

/**
 *  @brief Child of PacketConstructor for sensor data.
*/
class SensorPacket : public PacketConstructor {
    using PacketConstructor::PacketConstructor;
    void setTypeAndName() override {
        getBitArray().reset(1);
        getBitArray().reset(2);
    }
};

/**
 *  @brief Child of PacketConstructor for command data.
*/
class CommandPacket : public PacketConstructor {
    using PacketConstructor::PacketConstructor;
    void setTypeAndName() override {
        getBitArray().reset(1);
        getBitArray().set(2);
    }
};

/**
 *  @brief Child of PacketConstructor for state data.
 *
 *  This data will be used to monitor the state of each part of the pod.
*/
class StatePacket : public PacketConstructor {

    public:

        StatePacket(float *data) : PacketConstructor(data) {
            setTypeAndName();
        }
    void setTypeAndName() override {
        getBitArray().set(1);
        getBitArray().reset(2);
    }
};

/**
 *  @brief Child of PacketConstructor for log data.
*/
class LogPacket : public PacketConstructor {

    using PacketConstructor::PacketConstructor;
    void setTypeAndName() override {
        getBitArray().set(1);
        getBitArray().set(2);
    }
};

#endif //EMBEDDEDCPLUSPLUS_PACKETCONSTRUCTOR_H
