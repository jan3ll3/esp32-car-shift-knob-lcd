#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <functional>
#include "PacketHandler.h"
#include "ConfigurationManager.h"

// Forward declare callback classes
class MyCallbacks;
class MyServerCallbacks;
class MySecurity;

class BLEManager {
    friend class MyCallbacks;
    friend class MyServerCallbacks;
    friend class MySecurity;

public:
    BLEManager();  // Default constructor
    void init(PacketHandler* packetHandler, ConfigurationManager* configManager);
    void begin();
    bool isDeviceConnected() const { return deviceConnected; }
    bool isAuthenticated() const { return authenticated; }
    const char* getPin() const { return PIN_CODE; }
    
    // Callback setters
    void setAuthenticationFailedCallback(std::function<void()> callback) {
        onAuthenticationFailed = callback;
    }
    
    void setAuthenticationCallback(std::function<void(bool)> callback) {
        onAuthentication = callback;
    }
    
    void setConfigManager(ConfigurationManager* manager) {
        configManager = manager;
    }

    static BLEManager* instance;
    PacketHandler* packetHandler = nullptr;
    ConfigurationManager* configManager = nullptr;

    // Make these static members public
    static bool deviceConnected;
    static bool authenticated;

private:
    BLEServer* pServer = nullptr;
    BLECharacteristic* pCharacteristic = nullptr;
    
    std::function<void()> onAuthenticationFailed;
    std::function<void(bool)> onAuthentication;
    
    static const char* SERVICE_UUID;
    static const char* CHARACTERISTIC_UUID;
    static const char* PIN_CODE;
};

#endif