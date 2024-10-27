// SPI_DMA.h
#ifndef SPI_DMA_H
#define SPI_DMA_H

#include <Arduino.h>
#include "driver/spi_master.h"

// Define TFT Pins (You may adjust these according to your wiring)
#define TFT_MOSI 38
#define TFT_SCK  39
#define TFT_CS   40

class SPI_DMA {
public:
    SPI_DMA();
    ~SPI_DMA();

    void initSPI();
    void transferData(const uint8_t *data, size_t len);

private:
    spi_device_handle_t spi; // SPI device handle
};

#endif // SPI_DMA_H
