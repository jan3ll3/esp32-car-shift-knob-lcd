#include "GC9A01A_Display.h"
#include "DeviceState.h"
#include "PacketHandler.h"
#include "StartupWizard.h"
#include "BLESerialManager.h"
#include "FileManager.h"
#include "TensorManager.h"
#include "PinDefinitions.h"
#include <SPIFFS.h>


// Global objects
GC9A01A_Display display;
DeviceState deviceState;
const uint8_t hallSensorPins[] = HALL_SENSOR_PINS;

// Create managers
PacketHandler packetHandler(deviceState, hallSensorPins);
BLESerialManager bleManager("ESP32_Device", packetHandler);
StartupWizard startupWizard(display, deviceState);
TensorManager tensorManager;
FileManager fileManager;


// Add near the top with other global variables
const int MAX_HALL_SENSORS = 8;  // Maximum number of hall effect sensors
float sensorData[MAX_HALL_SENSORS];  // Array to hold hall sensor readings

// Add to the top with other constants
const uint8_t GEAR_REVERSE = 0;  // Use 0 to represent reverse gear
// Gears will now be: 0 (Reverse), 1-6 (Forward gears)

void handleTrainingData(uint8_t currentGear, uint8_t numSensors) {
    // Collect readings from all active hall effect sensors
    for (int i = 0; i < numSensors; i++) {
        sensorData[i] = analogRead(hallSensorPins[i]);
    }

    // Observe the training data
    tensorManager.observeTrainingData(sensorData, numSensors, currentGear);

    // If we're connected to BLE and have data to send
    if (bleManager.isConnected() && tensorManager.saveTrainingData()) {
        size_t dataSize;
        tensorManager.getTrainingData(nullptr, &dataSize);
        
        uint8_t* buffer = new uint8_t[dataSize];
        if (buffer != nullptr) {
            tensorManager.getTrainingData(buffer, &dataSize);
            
            // Convert buffer to String for BLESerialManager
            String dataString;
            for(size_t i = 0; i < dataSize; i++) {
                dataString += (char)buffer[i];
            }
            bleManager.sendData(dataString);
            
            delete[] buffer;
            tensorManager.clearTrainingData();
        }
    }
}

// Add this function for prediction
uint8_t predictGearPosition(uint8_t numSensors) {
    // Get current readings from all active hall sensors
    for (int i = 0; i < numSensors; i++) {
        sensorData[i] = analogRead(hallSensorPins[i]);
    }
    
    return tensorManager.predictGear(sensorData, numSensors);
}





void handleStartupInitSequence() {
    bool load = false;
    deviceState.loadStartupState();
    while(!load) {
        bleManager.checkData();
        if(!deviceState.isInitialized()) {
            if(bleManager.isConnected()) {startupWizard.showAppSyncPrompt();}
            else {startupWizard.showFirstBoot();}
        } else if(deviceState.isAppIdentified()) {
            if(deviceState.isCalibrated()) {load = true;}
            else {startupWizard.showCalibrationNeeded();}
        }else {load = true;}
    }
}


void displayHallSensorValues() {
    const int hallSensorPins[] = HALL_SENSOR_PINS;
    const int MAX_RADIUS = 15;       
    const int SPACING_X = 80;        
    const int SPACING_Y = 100;       
    const int START_X = 40;          
    const int START_Y = 60;          
    
    // Clear previous display
    display.clearDisplay(0x0000);    // Black background
    
    for (int i = 0; i < 6; i++) {    
        int rawValue = analogRead(hallSensorPins[i]);
        // Map the raw ADC value (0-4095) to 0-100 range
        int sensorValue = map(rawValue, 0, 4095, 0, 100);
        
        // Map sensor value (0-100) to radius
        int radius = map(sensorValue, 0, 100, 1, MAX_RADIUS);
        
        // Calculate circle position
        int x = START_X + (i % 3) * SPACING_X;
        int y = START_Y + (i / 3) * SPACING_Y;
        
        // Draw circle
        display.drawCircle(x, y, radius, 0xFFFF);
        
        // Display mapped value (0-100)
        display.drawText(x - 15, y + 25, sensorValue, 0xFFFF, 1);
    }
    
    display.updateDisplay();
}

void handleTrainingDataRequest(uint8_t gearPosition) {
    const uint8_t NUM_SAMPLES = 10;  // Number of samples to collect per request
    const uint8_t NUM_SENSORS = 6;   // Number of active sensors
    
    // Create a buffer for the complete packet
    uint8_t packet[256];  // Max packet size
    uint8_t checksum = 0;
    
    // Set header
    packet[0] = CMD_DATA;        // Command ID
    packet[2] = DATA_TRAINING;   // First payload byte: data type
    packet[3] = gearPosition;    // Second payload byte: gear position
    
    // Debug print header
    Serial.println("\n=== Training Data Response ===");
    Serial.printf("Command ID: 0x%02X\n", packet[0]);
    Serial.printf("Data Type: 0x%02X\n", packet[2]);
    Serial.printf("Gear Position: %d\n", packet[3]);
    
    // Collect sensor data into payload
    uint8_t payloadIndex = 4;
    Serial.println("\nSensor Readings:");
    for (int sample = 0; sample < NUM_SAMPLES; sample++) {
        Serial.printf("Sample %d: ", sample + 1);
        for (int i = 0; i < NUM_SENSORS; i++) {
            int rawValue = analogRead(hallSensorPins[i]);
            uint8_t mappedValue = map(rawValue, 0, 4095, 0, 100);
            packet[payloadIndex++] = mappedValue;
            Serial.printf("%d ", mappedValue);
        }
        Serial.println();
        delay(50);  // Small delay between samples
    }
    
    // Calculate payload length
    uint8_t payloadLength = payloadIndex - 2;
    packet[1] = payloadLength;
    Serial.printf("\nPayload Length: %d\n", payloadLength);
    
    // Calculate checksum
    for (int i = 0; i < payloadIndex; i++) {
        checksum += packet[i];
    }
    packet[payloadIndex++] = checksum;
    
    // Send the complete packet
    bleManager.sendData(String((char*)packet, payloadIndex));
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting up...");
    display.initDisplay();
    bleManager.begin();
    analogReadResolution(12);  // Set ADC resolution to 12 bits (0-4095)
    analogSetAttenuation(ADC_11db);  // Set ADC attenuation for 3.3V range

    // Initialize hall sensor pins
    for (int i = 0; i < MAX_HALL_SENSORS; i++) {
        pinMode(hallSensorPins[i], INPUT);
    }
    display.initDisplay();
    display.clearDisplay(0x8410); // Grey color in RGB565 format
    display.updateDisplay();
    delay(1000);
    // Initialize SPIFFS
    if(!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        display.clearDisplay(0xF800); // Pure red in RGB565 format
        display.updateDisplay();
        delay(1000);
        return;
    }
    Serial.println("SPIFFS Mounted Successfully");

    // Check if model exists
    if (fileManager.modelExists("shifter_model")) {
        Serial.println("Model exists, loading...");
        tensorManager.loadModel("shifter_model");
        deviceState.setCalibrated(true);
    } else {
        deviceState.setCalibrated(false);
    }

    
    display.clearDisplay(0x001F);
    display.updateDisplay();

    packetHandler.setBLEManager(&bleManager);
}

void loop() {
    bleManager.checkPackets();
    display.updateDisplay();
    delay(100);
}
//bleManager.sendData(data)