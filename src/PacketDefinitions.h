#ifndef PACKET_DEFINITIONS_H
#define PACKET_DEFINITIONS_H

#include <Arduino.h>

// Command IDs
enum CommandID {
    APP_IDENTIFICATION = 0x01
};

// Packet structure
struct Packet {
    uint8_t commandId;
    uint8_t length;
    uint8_t payload[256];
    uint8_t checksum;
};

#endif // PACKET_DEFINITIONS_H 