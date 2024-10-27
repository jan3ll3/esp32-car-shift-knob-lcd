// SPI_DMA.cpp
#include "SPI_DMA.h"

SPI_DMA::SPI_DMA() : spi(nullptr) {}

SPI_DMA::~SPI_DMA() {
    if (spi) {
        spi_bus_remove_device(spi);
        spi_bus_free(SPI3_HOST); // Use SPI3_HOST for ESP32-S3
    }
}

void SPI_DMA::initSPI() {
    // SPI bus configuration
    spi_bus_config_t buscfg = {
        .mosi_io_num = TFT_MOSI,
        .miso_io_num = -1,
        .sclk_io_num = TFT_SCK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 240 * 240 * 2 // Maximum transfer size for a full frame
    };

    // Initialize the SPI bus
    spi_bus_initialize(SPI3_HOST, &buscfg, SPI_DMA_CH_AUTO);

    // SPI device configuration
    spi_device_interface_config_t devcfg = {
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
        .mode = 0,                    // SPI mode 0
        .duty_cycle_pos = 128,        // 50% duty cycle
        .cs_ena_pretrans = 0,
        .cs_ena_posttrans = 0,
        .clock_speed_hz = 80 * 1000 * 1000, // 80MHz
        .input_delay_ns = 0,
        .spics_io_num = TFT_CS,
        .flags = 0,
        .queue_size = 1,
        .pre_cb = nullptr,
        .post_cb = nullptr
    };

    // Attach the SPI device to the SPI bus
    spi_bus_add_device(SPI3_HOST, &devcfg, &spi);
}

void SPI_DMA::transferData(const uint8_t *data, size_t len) {
    spi_transaction_t trans = {};
    trans.length = len * 8;   // Length in bits
    trans.tx_buffer = data;   // Pointer to the data to send

    // Perform the SPI transaction
    spi_device_transmit(spi, &trans);
}
