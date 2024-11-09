#include "StartupWizard.h"

void StartupWizard::showPinPrompt(const char* pin) {
    display.clearDisplay(0x0000);
    display.drawText(20, 80, "Enter PIN:", 0xFFFF, 2);
    display.drawText(20, 120, pin, 0xFFFF, 3);
    display.updateDisplay();
}

void StartupWizard::showFirstBoot() {
    display.clearDisplay(0x0000);
    
    // Draw QR code placeholder
    const uint16_t QR_SIZE = 120;
    const uint16_t QR_X = (240 - QR_SIZE) / 2;
    const uint16_t QR_Y = 20;
    display.drawRect(QR_X, QR_Y, QR_SIZE, QR_SIZE, 0xFFFF);
    
    const uint16_t TEXT_Y = QR_Y + QR_SIZE + 20;
    display.drawText(20, TEXT_Y, "Please download the app", 0xFFFF, 1);
    display.drawText(20, TEXT_Y+10, "and connect to bluetooth device:", 0xFFFF, 1);
    display.drawText(20, TEXT_Y+20, "SmartShift", 0xFFFF, 1);
    
    display.updateDisplay();
}

void StartupWizard::showCalibrationNeeded() {
    display.clearDisplay(0x0000);
    display.drawText(20, 100, "Calibration", 0xFFFF, 2);
    display.drawText(20, 120, "not detected.", 0xFFFF, 2);
    display.drawText(20, 150, "Please use the app ", 0xFFFF, 1);
    display.drawText(20, 160, "to calibrate the device.", 0xFFFF, 1);
    display.updateDisplay();
}

void StartupWizard::showAppSyncPrompt() {
    display.clearDisplay(0x0000);
    display.drawText(20, 60, "Use the setup", 0xFFFF, 2);
    display.drawText(20, 100, "wizard to ", 0xFFFF, 2);
    display.drawText(20, 140, "configure your", 0xFFFF, 2);
    display.drawText(20, 180, "device", 0xFFFF, 2);
    display.updateDisplay();
}

