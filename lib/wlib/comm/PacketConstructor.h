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

#include "../stl/Bitset.h"

using namespace wlp;

/**
 *  @brief Blueprint for a packet of type Bitset. Cannot be instatiated.
*/
class PacketConstructor {

public:
    PacketConstructor(const float *p_data, const Bitset<2> &packetType, const Bitset<6> &packetName); // Create a new Packet

    /** @brief Accessor method for the data pointer
     *
     * @return data pointer
    */
    /*const float* getData() const;*/

    /** @brief Accessor method for the bit array
     *
     * @return bit array
    */
    /*Bitset<64> getBitArray();*/

    /** @brief Returns number specified by the 64-bit array
     *
     * @return number
    */
    uint64_t getBitsetNum();

    /** @brief Returns updated packet and updates the float array
     *
     * Must be declared in the derived class when instantiated.
     *
     * @param data pointer to the array of packet values
     * @return Bitset
    */
    virtual void getFromParent(float* data)=0;
private:
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
     * @return Void.
    */
    void setTypeAndName();

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
protected:
    /** @brief Returns updated packet and updates the float array
     *
     * @return Bitset
    */
    Bitset<64> get(float* data);
private:
    const float *m_pdata;
    Bitset<64> m_bitArray;
    const Bitset<2> m_packetType;
    const Bitset<6> m_packetName;
};

#endif // EMBEDDEDCPLUSPLUS_PACKETCONSTRUCTOR_H

class SensorPacket : public PacketConstructor {

public:
    SensorPacket(const float* p_data, const Bitset<2> &packetType, const Bitset<6> &packetName) : PacketConstructor(p_data, packetType, packetName) {}

    void getFromParent(float *data) {
        get(data);
    }
};

