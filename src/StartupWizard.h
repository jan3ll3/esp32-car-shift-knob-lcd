#pragma once

#include "GC9A01A_Display.h"
#include "DeviceState.h"

class StartupWizard {
private:
    GC9A01A_Display& display;
    DeviceState& deviceState;

public:
    StartupWizard(GC9A01A_Display& display, DeviceState& deviceState)
        : display(display), deviceState(deviceState) {}

    void showPinPrompt(const char* pin);
    void showFirstBoot();
    void showCalibrationNeeded();
    void showAppSyncPrompt();
}; 