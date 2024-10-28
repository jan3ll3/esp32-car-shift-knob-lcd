#ifndef TRANSPARENT_IMAGE_DATA_H
#define TRANSPARENT_IMAGE_DATA_H

#include <Arduino.h>

struct TransparentImageData {
    const uint16_t width;
    const uint16_t height;
    const uint16_t* data;
    const uint8_t* alpha;
};

#endif // TRANSPARENT_IMAGE_DATA_H
