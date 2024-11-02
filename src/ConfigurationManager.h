#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

#include <Arduino.h>
#include "ForwardDeclarations.h"
#include "GC9A01A_Display.h"

// Forward declare the security class
class MySecurity;

class ConfigurationManager {
    friend class MySecurity;  // Allow MySecurity to access private members

public:
    ConfigurationManager(GC9A01A_Display& display, DeviceState& deviceState, BLEManager& bleManager);
    void updateStateDisplay();
    void handleAppRecognized();
    void handleBluetoothError();
    void handleAuthenticationFailed();
    void handleAuthenticationSuccess();
    void setAppIdentified(bool identified);

private:
    GC9A01A_Display& display;
    DeviceState& deviceState;
    BLEManager& bleManager;
    
    // Display methods
    void showFirstBoot();
    void showCalibrationNeeded();
    void showPinPrompt(const char* pin);
    void showBluetoothConnected();
    void showBluetoothError();
    void showAppRecognized();
    void showAppSyncPrompt();
    void showAuthenticationFailed();

    // State tracking variables
    bool showingBtConnected = false;
    unsigned long btConnectedTimestamp = 0;
    
    bool showingAppRecognized = false;
    unsigned long appRecognizedTimestamp = 0;
    
    bool showingBtError = false;
    unsigned long btErrorTimestamp = 0;
    
    bool showingAuthFailed = false;
    unsigned long authFailedTimestamp = 0;
    
    bool hasShownBtConnected = false;
};

#endif // CONFIGURATION_MANAGER_H 