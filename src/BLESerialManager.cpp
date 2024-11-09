#include "BLESerialManager.h"
#include "PacketHandler.h"

BLESerialManager::BLESerialManager(const char* name, PacketHandler& handler) 
    : deviceName(name), packetHandler(handler) {}

void BLESerialManager::begin() {
    serialBLE.begin(deviceName);
    deviceAuthenticated = false;
    Serial.printf("BLE started with device name: %s\n", deviceName);
}

bool BLESerialManager::isConnected() {
    return serialBLE.connected();
}

void BLESerialManager::checkPackets() {
    if (serialBLE.available()) {
        // Debug: Print how many bytes are available
        uint16_t size = serialBLE.available();
        Serial.printf("\n[DEBUG] Received %d bytes\n", size);
        
        // Create a buffer and read all bytes
        uint8_t* buffer = new uint8_t[size];
        Serial.print("[DEBUG] Raw bytes: ");
        
        for (uint16_t i = 0; i < size; i++) {
            buffer[i] = serialBLE.read();
            Serial.printf("0x%02X ", buffer[i]);
        }
        Serial.println();

        // Try to parse as packet
        Packet packet = parser.parsePacket(buffer, size);

        // Print detailed packet info
        Serial.printf("[DEBUG] Parsed packet - Command: 0x%02X, Length: %d, Checksum: 0x%02X\n", 
            packet.commandId, packet.length, packet.checksum);

        // Clean up
        delete[] buffer;

        // Handle the packet if valid
        if (packet.commandId != 0) {
            Serial.println("[DEBUG] Valid command ID, handling packet");
            packetHandler.handlePacket(packet);
        }
    }
}

void BLESerialManager::checkData() { //this is for debugging
    if (serialBLE.available()) {
        Serial.println("\n[DEBUG] Data received!");
        
        while (serialBLE.available()) {
            uint8_t byte = serialBLE.read();
            Serial.printf("Received byte: 0x%02X\n", byte);
        }
    }
}

void BLESerialManager::sendData(const String& data) {
    serialBLE.println(data);
    Serial.printf("Sent data: %s\n", data.c_str());
} 