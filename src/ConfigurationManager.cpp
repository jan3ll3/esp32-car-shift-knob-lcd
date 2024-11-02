#include "ConfigurationManager.h"
#include "BLEManager.h"
#include "DeviceState.h"



ConfigurationManager::ConfigurationManager(GC9A01A_Display& display, DeviceState& deviceState, BLEManager& bleManager)
    : display(display), deviceState(deviceState), bleManager(bleManager) {
    
    // Set up authentication callbacks
    bleManager.setAuthenticationFailedCallback([this]() {
        this->handleAuthenticationFailed();
    });

    bleManager.setAuthenticationCallback([this](bool authenticated) {
        if (authenticated) {
            this->handleAuthenticationSuccess();
        }
    });

    // Set the ConfigurationManager reference in BLEManager
    bleManager.setConfigManager(this);
}

void ConfigurationManager::handleAuthenticationSuccess() {
    Serial.println("Auth success handler called");  // Debug print
    showBluetoothConnected();
    showingBtConnected = true;
    btConnectedTimestamp = millis();
}

void ConfigurationManager::updateStateDisplay() {
    // First check if device has been identified by app
    if (deviceState.isAppIdentified()) {
        // Device is identified, check calibration
        if (!deviceState.isInitialized()) {
            showCalibrationNeeded();
        }
        // If calibrated, return to let main loop handle normal runtime
        return;
    }

    // Handle app recognized message
    if (showingAppRecognized) {
        if (millis() - appRecognizedTimestamp >= 5000) {
            showingAppRecognized = false;
            showCalibrationNeeded();
        }
        return;
    }

    // Device not identified - check connection and authentication
    if (bleManager.isDeviceConnected()) {
        if (!bleManager.isAuthenticated()) {
            // Show PIN prompt until authenticated
            showPinPrompt(bleManager.getPin());
            hasShownBtConnected = false;  // Reset this flag when showing PIN
            return;
        }
        
        // Authentication just completed
        if (!hasShownBtConnected) {
            showBluetoothConnected();
            hasShownBtConnected = true;
            btConnectedTimestamp = millis();
            return;
        }
        
        // After 3 seconds of showing connected message
        if (millis() - btConnectedTimestamp >= 3000) {
            showAppSyncPrompt();
        }
        return;
    }

    // No connection - show first boot
    showFirstBoot();
}

void ConfigurationManager::setAppIdentified(bool identified) {
    deviceState.setAppIdentified(identified);
    updateStateDisplay();
}

void ConfigurationManager::showFirstBoot() {
    display.clearDisplay(0x0000);
    
    // Draw QR code placeholder (centered square)
    const uint16_t QR_SIZE = 120;  // Size of QR code square
    const uint16_t QR_X = (240 - QR_SIZE) / 2;  // Center horizontally (240 is display width)
    const uint16_t QR_Y = 20;  // Position from top
    display.drawRect(QR_X, QR_Y, QR_SIZE, QR_SIZE, 0xFFFF);  // White rectangle
    
    // Draw text below QR code with larger size (size 2)
    const uint16_t TEXT_Y = QR_Y + QR_SIZE + 20;  // 20 pixels below QR code
    display.drawText(20, TEXT_Y, "Please download the app", 0xFFFF, 1);  
    display.drawText(20, TEXT_Y+10, "and connect to bluetooth device:", 0xFFFF, 1);  
    display.drawText(20, TEXT_Y+20, "SmartShift", 0xFFFF, 1);  
    
    display.updateDisplay();
}

void ConfigurationManager::showCalibrationNeeded() {
    display.clearDisplay(0x0000);
    display.drawText(20, 100, "Calibation", 0xFFFF, 2);
    display.drawText(20, 120, "not detected.", 0xFFFF, 2);
    display.drawText(20, 150, "Please use the app ", 0xFFFF, 1);
    display.drawText(20, 160, "to calibrate the device.", 0xFFFF, 1);
    display.updateDisplay();
}

void ConfigurationManager::showPinPrompt(const char* pin) {
    Serial.printf("showPinPrompt called with PIN: %s\n", pin);
    
    display.clearDisplay(0x0000);
    display.drawText(20, 80, "Enter PIN:", 0xFFFF, 2);
    display.drawText(20, 120, pin, 0xFFFF, 3);  // Larger size for PIN
    display.updateDisplay();
}

void ConfigurationManager::showBluetoothConnected() {
    display.clearDisplay(0x0000);
    display.drawText(20, 100, "Bluetooth Connected!", 0x07E0, 2);  // Green text
    display.updateDisplay();
}

void ConfigurationManager::showBluetoothError() {
    display.clearDisplay(0x0000);
    display.drawText(20, 100, "Bluetooth Error", 0xF800, 2);  // Red text
    display.drawText(20, 140, "Try Again", 0xF800, 2);
    display.updateDisplay();
}

void ConfigurationManager::showAppSyncPrompt() {
    display.clearDisplay(0x0000);
    display.drawText(20, 60, "Go to the app's", 0xFFFF, 2);
    display.drawText(20, 100, "settings page and", 0xFFFF, 2);
    display.drawText(20, 140, "select your device", 0xFFFF, 2);
    display.updateDisplay();
}

void ConfigurationManager::handleAppRecognized() {
    showAppRecognized();
    showingAppRecognized = true;
    appRecognizedTimestamp = millis();
}

void ConfigurationManager::showAppRecognized() {
    display.clearDisplay(0x0000);
    display.drawText(20, 100, "App Recognized!", 0x07E0, 2);  // Green text
    display.updateDisplay();
}

void ConfigurationManager::handleBluetoothError() {
    showBluetoothError();
    showingBtError = true;
    btErrorTimestamp = millis();
}

void ConfigurationManager::handleAuthenticationFailed() {
    showAuthenticationFailed();
    showingAuthFailed = true;
    authFailedTimestamp = millis();
}

void ConfigurationManager::showAuthenticationFailed() {
    display.clearDisplay(0x0000);
    display.drawText(20, 100, "Authentication Failed", 0xF800, 2);  // Red text
    display.updateDisplay();
}