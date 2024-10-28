#include "GC9A01A_Display.h"
#include "spi_dma.h" // Include the SPI DMA header
#include <stdlib.h> // Include for rand()

GC9A01A_Display::GC9A01A_Display() {
    // Remove spi.initSPI() call and frameBuffer allocation
    setPins();
}

void GC9A01A_Display::setPins() {
    pinMode(TFT_MOSI, OUTPUT);
    pinMode(TFT_SCK, OUTPUT);
    pinMode(TFT_CS, OUTPUT);
    pinMode(TFT_DC, OUTPUT);
    pinMode(TFT_RST, OUTPUT);
    pinMode(TFT_BL, OUTPUT);
}


void GC9A01A_Display::writeCommand(uint8_t cmd) {
    digitalWrite(TFT_DC, LOW); // Command mode
    digitalWrite(TFT_CS, LOW); // Select display
    
    // Send command byte over SPI
    spi_dma.transferData(&cmd, 1);
    
    digitalWrite(TFT_CS, HIGH); // Deselect display
}

void GC9A01A_Display::writeData(uint8_t data) {
    digitalWrite(TFT_DC, HIGH); // Data mode
    digitalWrite(TFT_CS, LOW);  // Select display
    
    // Send data byte over SPI
    spi_dma.transferData(&data, 1);
    
    digitalWrite(TFT_CS, HIGH); // Deselect display
}

void GC9A01A_Display::resetDisplay() {
    digitalWrite(TFT_RST, HIGH);
    delay(10);
    digitalWrite(TFT_RST, LOW);
    delay(10);
    digitalWrite(TFT_RST, HIGH);
    delay(10);
}

void GC9A01A_Display::initDisplay() {
    spi_dma.initSPI();          // Initialize SPI with DMA
    digitalWrite(TFT_CS, HIGH);
    digitalWrite(TFT_DC, HIGH);
    digitalWrite(TFT_SCK, LOW);
    resetDisplay();

    // Initialization sequence for GC9A01A
    writeCommand(0xEF);

    writeCommand(0xEB);
    writeData(0x14);

    writeCommand(0xFE);
    writeCommand(0xEF);

    writeCommand(0xEB);
    writeData(0x14);

    writeCommand(0x84);
    writeData(0x40);

    writeCommand(0x85);
    writeData(0xFF);

    writeCommand(0x86);
    writeData(0xFF);

    writeCommand(0x87);
    writeData(0xFF);

    writeCommand(0x88);
    writeData(0x0A);

    writeCommand(0x89);
    writeData(0x21);

    writeCommand(0x8A);
    writeData(0x00);

    writeCommand(0x8B);
    writeData(0x80);

    writeCommand(0x8C);
    writeData(0x01);

    writeCommand(0x8D);
    writeData(0x01);

    writeCommand(0x8E);
    writeData(0xFF);

    writeCommand(0x8F);
    writeData(0xFF);

    writeCommand(0xB6);
    writeData(0x00);
    writeData(0x20);

    writeCommand(0x36);
    writeData(0x08);

    writeCommand(0x3A);
    writeData(0x05);

    writeCommand(0x90);
    writeData(0x08);
    writeData(0x08);
    writeData(0x08);
    writeData(0x08);

    writeCommand(0xBD);
    writeData(0x06);

    writeCommand(0xBC);
    writeData(0x00);

    writeCommand(0xFF);
    writeData(0x60);
    writeData(0x01);
    writeData(0x04);

    writeCommand(0xC3);
    writeData(0x13);
    writeCommand(0xC4);
    writeData(0x13);

    writeCommand(0xC9);
    writeData(0x22);

    writeCommand(0xBE);
    writeData(0x11);

    writeCommand(0xE1);
    writeData(0x10);
    writeData(0x0E);

    writeCommand(0xDF);
    writeData(0x21);
    writeData(0x0c);
    writeData(0x02);

    writeCommand(0xF0);
    writeData(0x45);
    writeData(0x09);
    writeData(0x08);
    writeData(0x08);
    writeData(0x26);
    writeData(0x2A);

    writeCommand(0xF1);
    writeData(0x43);
    writeData(0x70);
    writeData(0x72);
    writeData(0x36);
    writeData(0x37);
    writeData(0x6F);

    writeCommand(0xF2);
    writeData(0x45);
    writeData(0x09);
    writeData(0x08);
    writeData(0x08);
    writeData(0x26);
    writeData(0x2A);

    writeCommand(0xF3);
    writeData(0x43);
    writeData(0x70);
    writeData(0x72);
    writeData(0x36);
    writeData(0x37);
    writeData(0x6F);

    writeCommand(0xED);
    writeData(0x1B);
    writeData(0x0B);

    writeCommand(0xAE);
    writeData(0x77);

    writeCommand(0xCD);
    writeData(0x63);

    writeCommand(0x70);
    writeData(0x07);
    writeData(0x07);
    writeData(0x04);
    writeData(0x0E);
    writeData(0x0F);
    writeData(0x09);
    writeData(0x07);
    writeData(0x08);
    writeData(0x03);

    writeCommand(0xE8);
    writeData(0x34);

    writeCommand(0x62);
    writeData(0x18);
    writeData(0x0D);
    writeData(0x71);
    writeData(0xED);
    writeData(0x70);
    writeData(0x70);
    writeData(0x18);
    writeData(0x0F);
    writeData(0x71);
    writeData(0xEF);
    writeData(0x70);
    writeData(0x70);

    writeCommand(0x63);
    writeData(0x18);
    writeData(0x11);
    writeData(0x71);
    writeData(0xF1);
    writeData(0x70);
    writeData(0x70);
    writeData(0x18);
    writeData(0x13);
    writeData(0x71);
    writeData(0xF3);
    writeData(0x70);
    writeData(0x70);

    writeCommand(0x64);
    writeData(0x28);
    writeData(0x29);
    writeData(0xF1);
    writeData(0x01);
    writeData(0xF1);
    writeData(0x00);
    writeData(0x07);

    writeCommand(0x66);
    writeData(0x3C);
    writeData(0x00);
    writeData(0xCD);
    writeData(0x67);
    writeData(0x45);
    writeData(0x45);
    writeData(0x10);
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);

    writeCommand(0x67);
    writeData(0x00);
    writeData(0x3C);
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);
    writeData(0x01);
    writeData(0x54);
    writeData(0x10);
    writeData(0x32);
    writeData(0x98);

    writeCommand(0x74);
    writeData(0x10);
    writeData(0x85);
    writeData(0x80);
    writeData(0x00);
    writeData(0x00);
    writeData(0x4E);
    writeData(0x00);

    writeCommand(0x98);
    writeData(0x3E);
    writeData(0x07);

    writeCommand(0x35);
    writeCommand(0x21);

    writeCommand(0x11);
    delay(120);

    writeCommand(0x29);
    delay(20);
    
    digitalWrite(TFT_BL, HIGH); // Turn on backlight
}


void GC9A01A_Display::drawRandomPixels() {
    // Set column and row addresses for filling the screen
    writeCommand(0x2A);
    writeData(0x00); writeData(0x00);
    writeData(0x00); writeData(0xEF);

    writeCommand(0x2B);
    writeData(0x00); writeData(0x00);
    writeData(0x00); writeData(0xEF);

    writeCommand(0x2C); // Start memory write

    digitalWrite(TFT_DC, HIGH);  // Data mode
    digitalWrite(TFT_CS, LOW);   // Select display

    const int bufferSize = 240; // Adjust buffer size as needed
    uint8_t color_data[bufferSize * 2]; // Buffer for multiple pixel colors

    for (uint32_t i = 0; i < 240 * 240; i += bufferSize) {
        for (int j = 0; j < bufferSize; ++j) {
            uint16_t random_color = rand() % 0xFFFF; // Generate a random 16-bit color
            color_data[j * 2] = random_color >> 8;
            color_data[j * 2 + 1] = random_color & 0xFF;
        }
        spi_dma.transferData(color_data, bufferSize * 2);
    }

    digitalWrite(TFT_CS, HIGH);  // Deselect display
}

void GC9A01A_Display::generateRandomPixelArray() {
    for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
        pixelArray[i] = random(0xFFFF);
    }
}

void GC9A01A_Display::updateDisplay() {
    // Set column and row addresses
    writeCommand(0x2A);
    writeData(0x00); writeData(0x00);
    writeData(0x00); writeData(0xEF);

    writeCommand(0x2B);
    writeData(0x00); writeData(0x00);
    writeData(0x00); writeData(0xEF);

    writeCommand(0x2C); // Start memory write

    digitalWrite(TFT_DC, HIGH);  // Data mode
    digitalWrite(TFT_CS, LOW);   // Select display

    const int bufferSize = 240; // One row at a time
    uint8_t color_data[bufferSize * 2]; // Buffer for pixel data

    // Transfer the pixel array data in rows
    for (uint32_t i = 0; i < 240 * 240; i += bufferSize) {
        // Convert pixel array values to color data bytes
        for (int j = 0; j < bufferSize; j++) {
            uint16_t pixel = pixelArray[i + j];
            color_data[j * 2] = pixel >> 8;        // High byte
            color_data[j * 2 + 1] = pixel & 0xFF;  // Low byte
        }
        spi_dma.transferData(color_data, bufferSize * 2);
    }

    digitalWrite(TFT_CS, HIGH);  // Deselect display
}

void GC9A01A_Display::clearDisplay(uint16_t color) {
    // Fill the pixel array with the specified color
    for (uint32_t i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
        pixelArray[i] = color;
    }
}

void GC9A01A_Display::drawImage(const ImageData* image, uint16_t x, uint16_t y) {
    // For each row of the image
    for (uint16_t yOffset = 0; yOffset < image->height; yOffset++) {
        // Calculate the starting position in the pixel array for this row
        uint32_t displayOffset = (y + yOffset) * DISPLAY_WIDTH + x;
        
        // For each pixel in the row
        for (uint16_t xOffset = 0; xOffset < image->width; xOffset++) {
            // Only draw the pixel if it's within the display bounds
            if ((x + xOffset) < DISPLAY_WIDTH && (y + yOffset) < DISPLAY_HEIGHT) {
                // Swap bytes for correct RGB565 format
                uint16_t pixel = image->data[yOffset * image->width + xOffset];
                pixel = (pixel << 8) | (pixel >> 8);  // Swap high and low bytes
                pixelArray[displayOffset + xOffset] = pixel;
            }
        }
    }
}

void GC9A01A_Display::drawTransparentImage(const TransparentImageData* image, uint16_t x, uint16_t y, uint8_t opacity) {
    // For each row of the image
    for (uint16_t yOffset = 0; yOffset < image->height; yOffset++) {
        // Calculate the starting position in the pixel array for this row
        uint32_t displayOffset = (y + yOffset) * DISPLAY_WIDTH + x;
        
        // For each pixel in the row
        for (uint16_t xOffset = 0; xOffset < image->width; xOffset++) {
            // Only draw the pixel if it's within the display bounds
            if ((x + xOffset) < DISPLAY_WIDTH && (y + yOffset) < DISPLAY_HEIGHT) {
                uint32_t pixelIndex = yOffset * image->width + xOffset;
                uint16_t newPixel = image->data[pixelIndex];
                newPixel = (newPixel << 8) | (newPixel >> 8);  // Swap bytes
                
                // Calculate combined alpha
                uint8_t alpha = (uint16_t)(image->alpha[pixelIndex] * opacity) / 255;
                
                if (alpha > 0) {
                    if (alpha == 255) {
                        // Fully opaque - direct copy
                        pixelArray[displayOffset + xOffset] = newPixel;
                    } else {
                        // Semi-transparent - blend
                        uint16_t bgPixel = pixelArray[displayOffset + xOffset];
                        
                        // Extract RGB components (5-6-5 format)
                        uint8_t newR = (newPixel >> 11) & 0x1F;
                        uint8_t newG = (newPixel >> 5) & 0x3F;
                        uint8_t newB = newPixel & 0x1F;
                        
                        uint8_t bgR = (bgPixel >> 11) & 0x1F;
                        uint8_t bgG = (bgPixel >> 5) & 0x3F;
                        uint8_t bgB = bgPixel & 0x1F;
                        
                        // Simple linear interpolation
                        uint8_t blendR = ((newR * alpha) + (bgR * (255 - alpha))) / 255;
                        uint8_t blendG = ((newG * alpha) + (bgG * (255 - alpha))) / 255;
                        uint8_t blendB = ((newB * alpha) + (bgB * (255 - alpha))) / 255;
                        
                        // Combine back into RGB565
                        uint16_t blendedPixel = (blendR << 11) | (blendG << 5) | blendB;
                        pixelArray[displayOffset + xOffset] = blendedPixel;
                    }
                }
            }
        }
    }
}

// Font data organized as [character][row] for ASCII 32-122
const uint8_t font5x7[123][8] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // 32 Space
    {0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x04, 0x00},   // 33 !
    {0x0A, 0x0A, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00},   // 34 "
    {0x0A, 0x0A, 0x1F, 0x0A, 0x1F, 0x0A, 0x0A, 0x00},   // 35 #
    {0x04, 0x0F, 0x14, 0x0E, 0x05, 0x1E, 0x04, 0x00},   // 36 $
    {0x18, 0x19, 0x02, 0x04, 0x08, 0x13, 0x03, 0x00},   // 37 %
    {0x0C, 0x12, 0x14, 0x08, 0x15, 0x12, 0x0D, 0x00},   // 38 &
    {0x0C, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00},   // 39 '
    {0x02, 0x04, 0x08, 0x08, 0x08, 0x04, 0x02, 0x00},   // 40 (
    {0x08, 0x04, 0x02, 0x02, 0x02, 0x04, 0x08, 0x00},   // 41 )
    {0x04, 0x15, 0x0E, 0x1F, 0x0E, 0x15, 0x04, 0x00},   // 42 *
    {0x00, 0x04, 0x04, 0x1F, 0x04, 0x04, 0x00, 0x00},   // 43 +
    {0x00, 0x00, 0x00, 0x00, 0x0C, 0x04, 0x08, 0x00},   // 44 ,
    {0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00},   // 45 -
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // 46 .
    {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x00, 0x00},   // 47 /
    {0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E, 0x00},   // 48 0
    {0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E, 0x00},   // 49 1
    {0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F, 0x00},   // 50 2
    {0x0E, 0x11, 0x01, 0x06, 0x01, 0x11, 0x0E, 0x00},   // 51 3
    {0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02, 0x00},   // 52 4
    {0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E, 0x00},   // 53 5
    {0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E, 0x00},   // 54 6
    {0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08, 0x00},   // 55 7
    {0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E, 0x00},   // 56 8
    {0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C, 0x00},   // 57 9
    {0x00, 0x0C, 0x0C, 0x00, 0x0C, 0x0C, 0x00, 0x00},   // 58 :
    {0x00, 0x0C, 0x0C, 0x00, 0x0C, 0x04, 0x08, 0x00},   // 59 ;
    {0x02, 0x04, 0x08, 0x10, 0x08, 0x04, 0x02, 0x00},   // 60 <
    {0x00, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x00, 0x00},   // 61 =
    {0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x00},   // 62 >
    {0x0E, 0x11, 0x01, 0x02, 0x04, 0x00, 0x04, 0x00},   // 63 ?
    {0x0E, 0x11, 0x17, 0x15, 0x17, 0x10, 0x0F, 0x00},   // 64 @
    {0x04, 0x0A, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x00},   // 65 A
    {0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E, 0x00},   // 66 B
    {0x0E, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0E, 0x00},   // 67 C
    {0x1E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1E, 0x00},   // 68 D
    {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F, 0x00},   // 69 E
    {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10, 0x00},   // 70 F
    {0x0E, 0x11, 0x10, 0x17, 0x11, 0x11, 0x0E, 0x00},   // 71 G
    {0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11, 0x00},   // 72 H
    {0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E, 0x00},   // 73 I
    {0x07, 0x02, 0x02, 0x02, 0x02, 0x12, 0x0C, 0x00},   // 74 J
    {0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11, 0x00},   // 75 K
    {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x00},   // 76 L
    {0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11, 0x00},   // 77 M
    {0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x11, 0x00},   // 78 N
    {0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E, 0x00},   // 79 O
    {0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10, 0x00},   // 80 P
    {0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D, 0x00},   // 81 Q
    {0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11, 0x00},   // 82 R
    {0x0E, 0x11, 0x10, 0x0E, 0x01, 0x11, 0x0E, 0x00},   // 83 S
    {0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00},   // 84 T
    {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E, 0x00},   // 85 U
    {0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04, 0x00},   // 86 V
    {0x11, 0x11, 0x11, 0x15, 0x15, 0x1B, 0x11, 0x00},   // 87 W
    {0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11, 0x00},   // 88 X
    {0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x04, 0x00},   // 89 Y
    {0x1F, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1F, 0x00},   // 90 Z
    {0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0E, 0x00},   // 91 [
    {0x00, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00},   // 92 backslash
    {0x0E, 0x02, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x00},   // 93 ]
    {0x04, 0x0A, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00},   // 94 ^
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00},   // 95 _
    {0x08, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00},   // 96 `
    {0x00, 0x00, 0x0E, 0x01, 0x0F, 0x11, 0x0F, 0x00},   // 97 a
    {0x10, 0x10, 0x16, 0x19, 0x11, 0x11, 0x1E, 0x00},   // 98 b
    {0x00, 0x00, 0x0E, 0x10, 0x10, 0x11, 0x0E, 0x00},   // 99 c
    {0x01, 0x01, 0x0D, 0x13, 0x11, 0x11, 0x0F, 0x00},   // 100 d
    {0x00, 0x00, 0x0E, 0x11, 0x1F, 0x10, 0x0E, 0x00},   // 101 e
    {0x06, 0x09, 0x08, 0x1C, 0x08, 0x08, 0x08, 0x00},   // 102 f
    {0x00, 0x0F, 0x11, 0x11, 0x0F, 0x01, 0x0E, 0x00},   // 103 g
    {0x10, 0x10, 0x16, 0x19, 0x11, 0x11, 0x11, 0x00},   // 104 h
    {0x04, 0x00, 0x0C, 0x04, 0x04, 0x04, 0x0E, 0x00},   // 105 i
    {0x02, 0x00, 0x06, 0x02, 0x02, 0x12, 0x0C, 0x00},   // 106 j
    {0x10, 0x10, 0x12, 0x14, 0x18, 0x14, 0x12, 0x00},   // 107 k
    {0x0C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E, 0x00},   // 108 l
    {0x00, 0x00, 0x1A, 0x15, 0x15, 0x11, 0x11, 0x00},   // 109 m
    {0x00, 0x00, 0x16, 0x19, 0x11, 0x11, 0x11, 0x00},   // 110 n
    {0x00, 0x00, 0x0E, 0x11, 0x11, 0x11, 0x0E, 0x00},   // 111 o
    {0x00, 0x00, 0x1E, 0x11, 0x1E, 0x10, 0x10, 0x00},   // 112 p
    {0x00, 0x00, 0x0D, 0x13, 0x0F, 0x01, 0x01, 0x00},   // 113 q
    {0x00, 0x00, 0x16, 0x19, 0x10, 0x10, 0x10, 0x00},   // 114 r
    {0x00, 0x00, 0x0E, 0x10, 0x0E, 0x01, 0x1E, 0x00},   // 115 s
    {0x08, 0x08, 0x1C, 0x08, 0x08, 0x09, 0x06, 0x00},   // 116 t
    {0x00, 0x00, 0x11, 0x11, 0x11, 0x13, 0x0D, 0x00},   // 117 u
    {0x00, 0x00, 0x11, 0x11, 0x11, 0x0A, 0x04, 0x00},   // 118 v
    {0x00, 0x00, 0x11, 0x11, 0x15, 0x15, 0x0A, 0x00},   // 119 w
    {0x00, 0x00, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x00},   // 120 x
    {0x00, 0x00, 0x11, 0x11, 0x0F, 0x01, 0x0E, 0x00},   // 121 y
    {0x00, 0x00, 0x1F, 0x02, 0x04, 0x08, 0x1F, 0x00}    // 122 z
};

void GC9A01A_Display::drawChar(uint16_t x, uint16_t y, char c, uint16_t color) {
    // Skip if character is not printable
    if(c < 32 || c > 122) return;
    
    // Get character index (subtract 32 since our array starts at space)
    int charIndex = c - 32;
    
    for(int row = 0; row < 8; row++) {
        uint8_t line = font5x7[charIndex][row];
        for(int col = 0; col < 5; col++) {
            if(line & (1 << (4 - col))) {  // Changed to read bits from left to right
                // Scale and draw the pixel
                for(uint8_t sx = 0; sx < textSize; sx++) {
                    for(uint8_t sy = 0; sy < textSize; sy++) {
                        int px = x + (col * textSize) + sx;
                        int py = y + (row * textSize) + sy;
                        if(px < DISPLAY_WIDTH && py < DISPLAY_HEIGHT) {
                            pixelArray[py * DISPLAY_WIDTH + px] = color;
                        }
                    }
                }
            }
        }
    }
}

void GC9A01A_Display::drawText(uint16_t x, uint16_t y, const char* text, uint16_t color, uint8_t size) {
    setTextSize(size);
    uint16_t cursorX = x;
    uint16_t cursorY = y;
    
    while(*text) {
        drawChar(cursorX, cursorY, *text++, color);
        cursorX += 6 * textSize;  // 5 pixels + 1 space between characters
        
        // Wrap text if it exceeds screen width
        if(cursorX > (DISPLAY_WIDTH - 6 * textSize)) {
            cursorX = x;
            cursorY += 8 * textSize;  // 7 pixels + 1 space between lines
        }
    }
}

void GC9A01A_Display::setTextSize(uint8_t size) {
    textSize = size > 0 ? size : 1;
}

// Add these new function implementations
void GC9A01A_Display::drawText(uint16_t x, uint16_t y, int number, uint16_t color, uint8_t size) {
    char buffer[12];  // Buffer for int conversion
    sprintf(buffer, "%d", number);
    drawText(x, y, buffer, color, size);
}

void GC9A01A_Display::drawText(uint16_t x, uint16_t y, float number, uint16_t color, uint8_t size) {
    char buffer[16];  // Increased buffer size to accommodate more digits
    dtostrf(number, 7, 5, buffer);  // Format: 7 total width, 5 decimal places
    drawText(x, y, buffer, color, size);
}





















