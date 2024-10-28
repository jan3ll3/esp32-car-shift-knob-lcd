#include "BLEManager.h"

bool BLEManager::deviceConnected = false;
bool BLEManager::authenticated = false;
const char* BLEManager::SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
const char* BLEManager::CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
const char* BLEManager::PIN_CODE = "123456";

class BLEManager::MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
        // Serial.println("Device connected!");
    };

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
        authenticated = false;
        // Serial.println("Device disconnected!");
        BLEDevice::startAdvertising();
    }
};

class BLEManager::MySecurity : public BLESecurityCallbacks {
    bool onConfirmPIN(uint32_t pin) {
        // Serial.println("onConfirmPIN called");
        bool match = (pin == atoi(PIN_CODE));
        // Serial.printf("Received PIN: %d, Expected: %s, Match: %d\n", pin, PIN_CODE, match);
        return match;
    }

    uint32_t onPassKeyRequest() {
        // Serial.println("onPassKeyRequest called");
        uint32_t passkey = atoi(PIN_CODE);
        // Serial.printf("Returning passkey: %d\n", passkey);
        return passkey;
    }

    void onPassKeyNotify(uint32_t pass_key) {
        // Serial.println("onPassKeyNotify called");
        // Serial.printf("Passkey to enter: %06d\n", pass_key);
    }

    bool onSecurityRequest() {
        // Serial.println("onSecurityRequest called");
        return true;
    }

    void onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl) {
        // Serial.println("onAuthenticationComplete called");
        // Serial.printf("Success: %d\n", cmpl.success);
        // Serial.printf("Fail reason: %d\n", cmpl.fail_reason);
        // Serial.printf("Auth type: %d\n", cmpl.auth_mode);
        // Serial.printf("Key present: %d\n", cmpl.key_present);
        // Serial.printf("Key type: %d\n", cmpl.key_type);
        
        authenticated = cmpl.success;
    }
};

BLEManager::BLEManager() {
    pServer = nullptr;
    pCharacteristic = nullptr;
}

void BLEManager::begin() {
    Serial.begin(115200);
    
    BLEDevice::init("Bmw e34");

    // Set encryption level and static passkey
    BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_MITM);
    BLESecurity *pSecurity = new BLESecurity();
    
    // Declare the passkey variable
    uint32_t passkey = 123456;  // Your static passkey

    // Set security parameters
    esp_ble_auth_req_t auth_req = ESP_LE_AUTH_REQ_SC_MITM_BOND;
    esp_ble_io_cap_t iocap = ESP_IO_CAP_OUT;
    uint8_t key_size = 16;
    uint8_t init_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
    uint8_t rsp_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;

    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_STATIC_PASSKEY, &passkey, sizeof(uint32_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth_req, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE, &iocap, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_MAX_KEY_SIZE, &key_size, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_INIT_KEY, &init_key, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY, &rsp_key, sizeof(uint8_t));

    // Set security callbacks
    BLEDevice::setSecurityCallbacks(new MySecurity());
    
    // Create server and start advertising
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    BLEService *pService = pServer->createService(SERVICE_UUID);
    
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY
    );

    pCharacteristic->addDescriptor(new BLE2902());
    
    pService->start();
    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
    
    Serial.println("BLE Setup complete!");
}
