#include "PacketParser.h"

Packet PacketParser::parsePacket(const uint8_t* data, uint16_t size) {
    Serial.println("\n=== Parsing Packet ===");
    Serial.printf("Received size: %d bytes\n", size);
    
    Packet packet = {0};
    
    if (size < 3) {
        Serial.println("Error: Packet too small");
        return packet;
    }

    Serial.printf("Command ID byte: 0x%02X\n", data[0]);
    Serial.printf("Length byte: 0x%02X\n", data[1]);
    Serial.printf("Checksum byte: 0x%02X\n", data[2]);

    packet.commandId = data[0];
    packet.length = data[1];
    
    // Validate packet length
    if (size < packet.length + 3) {
        Serial.println("Error: Incomplete packet");
        return packet;
    }

    // Copy payload if any
    if (packet.length > 0) {
        memcpy(packet.payload, &data[2], packet.length);
    }
    
    // Get checksum
    packet.checksum = data[packet.length + 2];
    
    // Verify checksum
    uint8_t calculatedChecksum = calculateChecksum(data, packet.length + 2);
    Serial.printf("Calculated checksum: 0x%02X\n", calculatedChecksum);
    
    if (calculatedChecksum != packet.checksum) {
        Serial.println("Error: Checksum mismatch");
        memset(&packet, 0, sizeof(Packet));
        return packet;
    }

    Serial.println("Packet parsed successfully");
    Serial.println("=====================\n");

    return packet;
}

uint8_t PacketParser::calculateChecksum(const uint8_t* data, uint16_t length) {
    uint8_t sum = 0;
    for (uint16_t i = 0; i < length; i++) {
        sum += data[i];
    }
    return sum;
} 