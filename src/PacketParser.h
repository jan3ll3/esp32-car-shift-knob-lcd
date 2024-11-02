#ifndef PACKET_PARSER_H
#define PACKET_PARSER_H

#include "PacketDefinitions.h"

class PacketParser {
public:
    Packet parsePacket(const uint8_t* data, uint16_t size);
private:
    uint8_t calculateChecksum(const uint8_t* data, uint16_t length);
};

#endif