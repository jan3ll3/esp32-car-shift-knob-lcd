#ifndef IMAGE_DATA_H
#define IMAGE_DATA_H

#include <Arduino.h>

struct ImageData {
    const uint16_t width;
    const uint16_t height;
    const uint16_t* const data;
};

#endif // IMAGE_DATA_H
