#include "PacketHandler.h"
#include "ConfigurationManager.h"
#include "DeviceState.h"

void PacketHandler::handlePacket(const Packet& packet) {
    // Debug print packet info
    Serial.println("\n=== Received Packet ===");
    Serial.printf("Command ID: 0x%02X\n", packet.commandId);
    Serial.printf("Length: %d\n", packet.length);
    Serial.print("Payload: ");
    for(int i = 0; i < packet.length; i++) {
        Serial.printf("0x%02X ", packet.payload[i]);
    }
    Serial.println();
    Serial.printf("Checksum: 0x%02X\n", packet.checksum);
    Serial.println("====================\n");

    switch (packet.commandId) {
        case APP_IDENTIFICATION:
            Serial.println("Processing: App Identification");
            handleAppIdentification(packet);
            break;
            
        default:
            Serial.printf("Unknown command ID: 0x%02X\n", packet.commandId);
            break;
    }
}

void PacketHandler::handleAppIdentification(const Packet& packet) {
    Serial.println("App identification successful");
    deviceState.setAppIdentified(true);
    configManager.handleAppRecognized();
} 