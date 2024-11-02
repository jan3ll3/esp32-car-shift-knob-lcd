#include "BLEManager.h"
#include "PacketParser.h"

// Define server callbacks first
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        BLEManager::deviceConnected = true;
        Serial.println("Device connected");
        if (BLEManager::instance && BLEManager::instance->onAuthentication) {
            BLEManager::instance->onAuthentication(true);
        }
    }

    void onDisconnect(BLEServer* pServer) {
        BLEManager::deviceConnected = false;
        BLEManager::authenticated = false;
        Serial.println("Device disconnected");
        if (BLEManager::instance && BLEManager::instance->onAuthentication) {
            BLEManager::instance->onAuthentication(false);
        }
        // Restart advertising
        BLEDevice::startAdvertising();
    }
};

// Then define characteristic callbacks
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        
        // Debug print raw data
        Serial.println("\n=== BLE Data Received ===");
        Serial.print("Raw bytes: ");
        for(int i = 0; i < value.length(); i++) {
            Serial.printf("0x%02X ", (uint8_t)value[i]);
        }
        Serial.printf("\nLength: %d bytes\n", value.length());
        Serial.println("=====================\n");

        if (value.length() > 0) {
            PacketParser parser;
            Packet packet = parser.parsePacket((uint8_t*)value.data(), value.length());
            
            // Only process if we have a valid packet
            if (packet.commandId != 0) {  // Assuming 0 is invalid
                if (BLEManager::instance && BLEManager::instance->packetHandler) {
                    BLEManager::instance->packetHandler->handlePacket(packet);
                } else {
                    Serial.println("Error: BLEManager instance or packetHandler is null");
                }
            } else {
                Serial.println("Error: Invalid packet received");
            }
        }
    }
};

// Define security callbacks
class MySecurity : public BLESecurityCallbacks {
    uint32_t onPassKeyRequest() {
        uint32_t pin = 123456;
        Serial.printf("PassKey Request: %06d\n", pin);
        if (BLEManager::instance && BLEManager::instance->configManager) {
            char pinStr[7];
            snprintf(pinStr, sizeof(pinStr), "%06d", pin);
            Serial.printf("Showing PIN from onPassKeyRequest: %s\n", pinStr);
            BLEManager::instance->configManager->showPinPrompt(pinStr);
        }
        return pin;
    }

    void onPassKeyNotify(uint32_t pass_key) {
        // We can ignore this since we're using static PIN
        Serial.printf("PassKey Notify received (ignoring): %06d\n", pass_key);
    }

    bool onConfirmPIN(uint32_t pass_key) {
        Serial.printf("Confirming PIN: %06d\n", pass_key);
        return pass_key == 123456;  // Only confirm if it matches our static PIN
    }

    bool onSecurityRequest() {
        Serial.println("Security Request");
        return true;
    }

    void onAuthenticationComplete(esp_ble_auth_cmpl_t auth_cmpl) {
        if(auth_cmpl.success) {
            Serial.println("Authentication Success");
            BLEManager::authenticated = true;
            if (BLEManager::instance && BLEManager::instance->onAuthentication) {
                BLEManager::instance->onAuthentication(true);
            }
        } else {
            Serial.println("Authentication Failed");
            if (BLEManager::instance && BLEManager::instance->onAuthenticationFailed) {
                BLEManager::instance->onAuthenticationFailed();
            }
        }
    }
};

// Define static members
BLEManager* BLEManager::instance = nullptr;
bool BLEManager::deviceConnected = false;
bool BLEManager::authenticated = false;
const char* BLEManager::SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
const char* BLEManager::CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
const char* BLEManager::PIN_CODE = "123456";

BLEManager::BLEManager() {
    instance = this;
}

void BLEManager::init(PacketHandler* ph, ConfigurationManager* cm) {
    packetHandler = ph;
    configManager = cm;
}

void BLEManager::begin() {
    // Initialize BLE
    BLEDevice::init("BMW E34");
    
    // Set up security with static PIN
    BLESecurity *pSecurity = new BLESecurity();
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM);  // Changed this
    pSecurity->setCapability(ESP_IO_CAP_OUT);
    pSecurity->setKeySize(16);
    pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
    pSecurity->setRespEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
    
    // Set static passkey
    uint32_t passkey = 123456;  // Your static PIN
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_STATIC_PASSKEY, &passkey, sizeof(uint32_t));
    
    // Set the PIN callback
    BLEDevice::setSecurityCallbacks(new MySecurity());
    
    // Create server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    
    // Create the service
    BLEService *pService = pServer->createService(SERVICE_UUID);
    
    // Create characteristic with security
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY
    );
    
    // Set security for the characteristic
    pCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
    
    // Add callbacks
    pCharacteristic->setCallbacks(new MyCallbacks());
    
    // Start the service
    pService->start();
    
    // Start advertising with security flags
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    
    Serial.println("BLE device ready with security enabled");
}
