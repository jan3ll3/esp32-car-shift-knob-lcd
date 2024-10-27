// GC9A01A_Display.h
#ifndef GC9A01A_DISPLAY_H
#define GC9A01A_DISPLAY_H

#include <Arduino.h>
#include "SPI_DMA.h" // Include SPI_DMA

// Define TFT Pins (adjust these according to your wiring)
#define TFT_MOSI 38       // Data In (DIN)
#define TFT_SCK 39        // Clock (CLK)
#define TFT_CS 40         // Chip Select (CS)
#define TFT_DC 41         // Data/Command (DC)
#define TFT_RST 37        // Reset (RST)
#define TFT_BL 36         // Backlight

class GC9A01A_Display {
public:
    GC9A01A_Display();
    void initDisplay();
    void fillScreen(uint16_t color);

private:
    SPI_DMA spi_dma; // SPI_DMA instance
    void writeCommand(uint8_t cmd);
    void writeData(uint8_t data);
    void setPins();
    void resetDisplay();
};

#endif // GC9A01A_DISPLAY_H
