#include <GC9A01A_Display.h>
#include <Arduino.h>

GC9A01A_Display display;

void setup() {
    display.initDisplay();
    display.fillScreen(0x001F);  // Fill with blue
}

uint16_t randomColor() {
    return random(0x0000, 0xFFFF);  // Generate a random 16-bit color value
}

void loop() {
    // Code for continuous display updates or other functionalities
    display.fillScreen(randomColor());

    // Wait 2 seconds before changing color again
    delay(2000);
}
