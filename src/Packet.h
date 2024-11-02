#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

enum PacketType {
    APP_IDENTIFICATION,
    SETUP_START,
    SETUP_COMPLETE,
    DEVICE_RESET
};

struct Packet {
    PacketType type;
    uint8_t* payload;
    uint16_t payloadLength;
    uint16_t checksum;
};

#endif // PACKET_H 