#ifndef PIN_DEFINITIONS_H
#define PIN_DEFINITIONS_H

// Display pins (GC9A01A)
#define TFT_MOSI 38       // Data In (DIN)
#define TFT_SCK  39       // Clock (CLK)
#define TFT_CS   40       // Chip Select (CS)
#define TFT_DC   41       // Data/Command (DC)
#define TFT_RST  37       // Reset (RST)
#define TFT_BL   36       // Backlight

// Hall Effect Sensor Pins
#define ACTIVE_HALL_SENSORS 8
// Using verified ADC-capable GPIO pins on ESP32-S3-DevKitC-1
#define HALL_SENSOR_1 1   // GPIO1  - ADC1_CH0
#define HALL_SENSOR_2 2   // GPIO2  - ADC1_CH1
#define HALL_SENSOR_3 4   // GPIO4  - ADC1_CH3
#define HALL_SENSOR_4 5   // GPIO5  - ADC1_CH4
#define HALL_SENSOR_5 6   // GPIO6  - ADC1_CH5
#define HALL_SENSOR_6 7   // GPIO7  - ADC1_CH6
#define HALL_SENSOR_7 8   // GPIO8  - ADC1_CH7
#define HALL_SENSOR_8 9   // GPIO9  - ADC1_CH8

// Array of hall sensor pins for easy iteration
#define HALL_SENSOR_PINS { \
    HALL_SENSOR_1, \
    HALL_SENSOR_2, \
    HALL_SENSOR_3, \
    HALL_SENSOR_4, \
    HALL_SENSOR_5, \
    HALL_SENSOR_6, \
    HALL_SENSOR_7, \
    HALL_SENSOR_8  \
}

#endif // PIN_DEFINITIONS_H 