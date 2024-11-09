#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include "DeviceState.h"
#include "PacketDefinitions.h"
#include "FileManager.h"
#include "PinDefinitions.h"
#include "ForwardDeclarations.h"

class PacketHandler {
public:
    PacketHandler(DeviceState& state, const uint8_t* pins);
    void handlePacket(const Packet& packet);
    void setBLEManager(BLESerialManager* manager) { bleManager = manager; }

private:
    DeviceState& deviceState;
    FileManager fileManager;
    const uint8_t* hallSensorPins;
    BLESerialManager* bleManager = nullptr;
    static const uint16_t MAX_PACKET_SIZE = 256;
    
    // Command handlers
    void handleInitCommand(const Packet& packet);
    void handleThemesCommand(const Packet& packet);
    void handleData(const Packet& packet);
    void handleConfigCommand(const Packet& packet);
    void handleAck(const Packet& packet);
    void handleNack(const Packet& packet);
    void handleStatus(const Packet& packet);
    void handleNotification(const Packet& packet);
    void handleTrainingDataRequest(uint8_t gearPosition);
    
    // Utility methods
    uint8_t calculateChecksum(const uint8_t* data, uint16_t length);
    void sendPacket(const Packet& packet);
};

#endif