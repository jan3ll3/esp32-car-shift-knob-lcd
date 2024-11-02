#ifndef DEVICE_STATE_H
#define DEVICE_STATE_H

#include <Preferences.h>

class DeviceState {
public:
    DeviceState() : preferences() {}
    
    void begin() {
        preferences.begin("device", false);  // false = RW mode
        calibrated = preferences.getBool("calibrated", false);
        appIdentified = preferences.getBool("app_setup", false);
    }
    
    bool isInitialized() const { return calibrated; }
    bool isAppIdentified() const { return appIdentified; }
    
    void setCalibrated(bool status) {
        calibrated = status;
        preferences.putBool("calibrated", status);
    }
    
    void setAppIdentified(bool status) {
        appIdentified = status;
        preferences.putBool("app_setup", status);
    }
    
    void completeSetup() {
        setCalibrated(true);
        setAppIdentified(true);
    }
    
    void resetDevice() {
        calibrated = false;
        appIdentified = false;
        preferences.putBool("calibrated", false);
        preferences.putBool("app_setup", false);
        // Add any other reset operations here
    }
    
    void setDeviceName(const char* name) {
        preferences.putString("device_name", name);
    }
    
    String getDeviceName() {
        return preferences.getString("device_name", "BMW E34");  // Default name
    }

private:
    Preferences preferences;
    bool calibrated = false;
    bool appIdentified = false;
};

#endif // DEVICE_STATE_H 