#ifndef BLE_SERIAL_MANAGER_H
#define BLE_SERIAL_MANAGER_H

#include <BleSerial.h>
#include "PacketParser.h"
#include "ForwardDeclarations.h"

class BLESerialManager {
public:
    BLESerialManager(const char* deviceName, PacketHandler& packetHandler);
    
    void begin();
    bool isConnected();
    void checkData();
    void checkPackets();
    void sendData(const String& data);
    
    // Getters
    static const char* getServiceUUID() { return "0000FFE0-0000-1000-8000-00805F9B34FB"; }
    static const char* getCharacteristicUUID() { return "0000FFE1-0000-1000-8000-00805F9B34FB"; }
    const char* getDeviceName() { return deviceName; }
    const char* getPin() { return staticPin; }

private:
    BleSerial serialBLE;
    PacketParser parser;
    PacketHandler& packetHandler;
    const char* deviceName;
    const char* staticPin = "123456";
    bool deviceAuthenticated = false;
};

#endif 