// GC9A01A_Display.h
#ifndef GC9A01A_DISPLAY_H
#define GC9A01A_DISPLAY_H

#include <Arduino.h>
#include "SPI_DMA.h" // Include SPI_DMA
#include "images/images.h"  // This will give us access to all images and ImageData struct

// Define TFT Pins (adjust these according to your wiring)
#define TFT_MOSI 38       // Data In (DIN)
#define TFT_SCK 39        // Clock (CLK)
#define TFT_CS 40         // Chip Select (CS)
#define TFT_DC 41         // Data/Command (DC)
#define TFT_RST 37        // Reset (RST)
#define TFT_BL 36         // Backlight

class GC9A01A_Display {
public:
    static const uint16_t SCREEN_WIDTH = 240;
    static const uint16_t SCREEN_HEIGHT = 240;

    GC9A01A_Display();  // Constructor

    void initDisplay();
    void generateRandomPixelArray();
    void drawRandomPixels();
    void updateDisplay();
    void clearDisplay(uint16_t color = 0x0000);  // Default to black if no color specified
    void drawImage(const ImageData* image, uint16_t x, uint16_t y);
    void drawTransparentImage(const TransparentImageData* image, uint16_t x, uint16_t y, uint8_t opacity = 255);
    void drawChar(uint16_t x, uint16_t y, char c, uint16_t color);
    void drawText(uint16_t x, uint16_t y, const char* text, uint16_t color, uint8_t size = 1);
    void drawText(uint16_t x, uint16_t y, int number, uint16_t color, uint8_t size = 1);
    void drawText(uint16_t x, uint16_t y, float number, uint16_t color, uint8_t size = 1);
    void drawText(uint16_t x, uint16_t y, double number, uint16_t color, uint8_t size = 1);
    void setTextSize(uint8_t size);

private:
    uint8_t* frameBuffer; // Frame buffer to store pixel data
    SPI_DMA spi_dma; // SPI DMA object
    static const int DISPLAY_WIDTH = 240;
    static const int DISPLAY_HEIGHT = 240;
    uint16_t pixelArray[DISPLAY_WIDTH * DISPLAY_HEIGHT];  // Static array instead of pointer
    uint8_t textSize = 1;

    // Declare other private methods used in the implementation
    void setPins();
    void writeCommand(uint8_t cmd);
    void writeData(uint8_t data);
    void resetDisplay();
};

#endif // GC9A01A_DISPLAY_H
