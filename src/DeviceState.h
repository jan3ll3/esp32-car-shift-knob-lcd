#ifndef DEVICE_STATE_H
#define DEVICE_STATE_H

#include <Arduino.h>
#include <Preferences.h>

class DeviceState {
private:
    Preferences preferences;

    struct PreferencesState {
        bool initialized;
        bool appIdentified;
        bool calibrated;
    };

    PreferencesState state;

public:
    DeviceState() : state{false, false, false} {}
    
    PreferencesState loadStartupState() {
        if (preferences.begin("smartshift", false)) {
            // Check initialized
            if (!preferences.isKey("initialized")) {
                Serial.println("Creating initialized preference");
                preferences.putBool("initialized", false);
            } else {
                state.initialized = preferences.getBool("initialized");
            }

            // Check appIdentified
            if (!preferences.isKey("appIdentified")) {
                Serial.println("Creating appIdentified preference");
                preferences.putBool("appIdentified", false);
            } else {
                state.appIdentified = preferences.getBool("appIdentified");
            }

            // Check calibrated
            if (!preferences.isKey("calibrated")) {
                Serial.println("Creating calibrated preference");
                preferences.putBool("calibrated", false);
            } else {
                state.calibrated = preferences.getBool("calibrated");
            }
        } else {
            Serial.println("Failed to initialize preferences");
        }
        
        return state;
    }

    bool isCalibrated() const { return state.calibrated; }
    bool isInitialized() const { return state.initialized; }
    bool isAppIdentified() const { return state.appIdentified; }
    
    void setCalibrated(bool status) {
        state.calibrated = status;
        preferences.putBool("calibrated", status);
    }
    
    void setAppIdentified(bool status) {
        state.appIdentified = status;
        preferences.putBool("appIdentified", status);
    }
    
    void completeSetup() {
        setCalibrated(true);
        setAppIdentified(true);
    }
    
    void resetDevice() {
        state.calibrated = false;
        state.appIdentified = false;
        preferences.putBool("calibrated", false);
        preferences.putBool("appIdentified", false);
        // Add any other reset operations here
    }
    
    void setDeviceName(const char* name) {
        preferences.putString("device_name", name);
    }
    
    String getDeviceName() {
        return preferences.getString("device_name", "BMW E34");  // Default name
    }
    
    void setInitialized(bool status) {
        state.initialized = status;
        preferences.putBool("initialized", status);
        Serial.printf("Device initialization set to: %s\n", status ? "true" : "false");
    }
};

#endif // DEVICE_STATE_H 