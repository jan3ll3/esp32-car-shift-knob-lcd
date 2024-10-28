#ifndef IMAGES_H
#define IMAGES_H

#include "../ImageData.h"
#include "../TransparentImageData.h"

// Include all image files
#include "bmw.h"
#include "transparent_bmw.h"  // Your new transparent image

// Regular image declarations
extern const ImageData bmw;

// Transparent image declarations
extern const TransparentImageData transparent_bmw;

// Create arrays for regular and transparent images
static const ImageData* const IMAGES[] = {
    &bmw
};

enum ImageIndex {
    BMW,
    IMAGE_COUNT
};

static const TransparentImageData* const TRANSPARENT_IMAGES[] = {
    &transparent_bmw
};

enum TransparentImageIndex {
    TRANSPARENT_BMW,
    TRANSPARENT_IMAGE_COUNT
};

#endif // IMAGES_H
