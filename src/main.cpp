#include "BLEManager.h"
#include "GC9A01A_Display.h"
#include "TensorFlowLiteManager.h"
#include "PacketParser.h"
#include "PacketHandler.h"
#include "ConfigurationManager.h"
#include "DeviceState.h"

GC9A01A_Display display;
BLEManager bleManager;
TensorFlowLiteManager tfLiteManager;
DeviceState deviceState;
ConfigurationManager configManager(display, deviceState, bleManager);

PacketParser parser;
PacketHandler packetHandler(deviceState, configManager);

void processSerialDataNonBlocking();

void setup() {
    Serial.begin(115200);
    display.initDisplay();
    delay(1000);  // Give display time to initialize

    // Initialize device state (will load saved state from preferences)
    deviceState.begin();
    
    // Initialize BLE
    bleManager.init(&packetHandler, &configManager);
    bleManager.begin();

    // Initial display update based on current state
    configManager.updateStateDisplay();
}

void loop() {
    // Update display based on current state
    configManager.updateStateDisplay();
    delay(100);  // Small delay to prevent too frequent updates

    /* Original loop code
    processSerialDataNonBlocking();
    configManager.updateDisplay();
    delay(10);
    */
}

void processSerialDataNonBlocking() {
    static uint8_t buffer[256]; // Adjust size as needed
    static uint16_t index = 0;

    while (Serial.available() > 0) {
        buffer[index++] = Serial.read();
        if (index >= sizeof(buffer)) {
            // Process the packet if buffer is full
            Packet packet = parser.parsePacket(buffer, index);
            packetHandler.handlePacket(packet);
            index = 0; // Reset index after processing
        }
    }
}
