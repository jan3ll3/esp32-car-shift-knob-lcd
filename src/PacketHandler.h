#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include "PacketDefinitions.h"
#include "ForwardDeclarations.h"

class PacketHandler {
public:
    PacketHandler(DeviceState& deviceState, ConfigurationManager& configManager)
        : deviceState(deviceState), configManager(configManager) {}
    
    void handlePacket(const Packet& packet);

private:
    void handleAppIdentification(const Packet& packet);
    
    DeviceState& deviceState;
    ConfigurationManager& configManager;
};

#endif