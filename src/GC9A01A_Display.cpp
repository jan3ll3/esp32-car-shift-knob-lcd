#include "GC9A01A_Display.h"

GC9A01A_Display::GC9A01A_Display() {
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

void GC9A01A_Display::fillScreen(uint16_t color) {
    uint8_t color_data[480]; // Buffer for color data

    // Prepare buffer with the color in RGB565 format
    for (int i = 0; i < 480; i += 2) {
        color_data[i] = color >> 8;
        color_data[i + 1] = color & 0xFF;
    }

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

    // Transfer the color data in chunks to fill the screen
    for (uint32_t i = 0; i < 240 * 240 * 2; i += 480) {
        spi_dma.transferData(color_data, 480);
    }

    digitalWrite(TFT_CS, HIGH);  // Deselect display
}
