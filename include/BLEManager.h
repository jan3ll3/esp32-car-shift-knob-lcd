#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

class BLEManager {
public:
    BLEManager();
    void begin();
    bool isConnected() { return deviceConnected; }
    bool isAuthenticated() { return authenticated; }
    const char* getPinCode() { return PIN_CODE; }  // Add this getter

private:
    BLEServer* pServer;
    BLECharacteristic* pCharacteristic;
    static bool deviceConnected;
    static bool authenticated;
    
    class MyServerCallbacks;
    class MySecurity;

    static const char* SERVICE_UUID;
    static const char* CHARACTERISTIC_UUID;
    static const char* PIN_CODE;
};

#endif // BLE_MANAGER_H
