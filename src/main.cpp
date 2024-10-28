#include "BLEManager.h"
#include "GC9A01A_Display.h"

GC9A01A_Display display;
BLEManager bleManager;

unsigned long connectedTime = 0;
const unsigned long DISPLAY_DURATION = 5000; // 5 seconds in milliseconds

void setup() {
    display.initDisplay();
    bleManager.begin();
}

void loop() {
    display.clearDisplay(0x0000);
    
    if (bleManager.isConnected() && bleManager.isAuthenticated()) {
        // If just connected, set the time
        if (connectedTime == 0) {
            connectedTime = millis();
        }
        
        // Only show "Connected!" for 5 seconds
        if (millis() - connectedTime <= DISPLAY_DURATION) {
            display.drawText(20, 100, "Connected!", 0xFFFF, 2);
        }
    } else if (bleManager.isConnected() && !bleManager.isAuthenticated()) {
        char buffer[20];
        sprintf(buffer, "PIN: %s", bleManager.getPinCode());
        display.drawText(20, 100, buffer, 0xFFFF, 2);
        connectedTime = 0;  // Reset the connected time
    } else {
        connectedTime = 0;  // Reset the connected time
    }
    
    display.updateDisplay();
    delay(10);
}
