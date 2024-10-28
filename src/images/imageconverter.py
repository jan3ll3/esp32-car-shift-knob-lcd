from PIL import Image
import os
import sys

def rgb_to_rgb565(r, g, b):
    # First create RGB565 value
    rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
    # Swap bytes
    msb = (rgb565 >> 8) & 0xFF
    lsb = rgb565 & 0xFF
    return (lsb << 8) | msb

def convert_image_to_rgb565(input_path, output_path):
    # Open and convert image to RGB mode
    img = Image.open(input_path).convert('RGB')
    width, height = img.size
    
    base_name = os.path.splitext(os.path.basename(output_path))[0]
    
    header = f"""// Auto-generated file from {os.path.basename(input_path)}
#ifndef {base_name.upper()}_H
#define {base_name.upper()}_H

#include "../ImageData.h"

// Image data array
static const uint16_t {base_name}_data[{width * height}] PROGMEM = {{"""

    # Convert pixels to RGB565 format
    pixels = []
    for y in range(height):
        for x in range(width):
            r, g, b = img.getpixel((x, y))
            rgb565 = rgb_to_rgb565(r, g, b)
            pixels.append(f"0x{rgb565:04X}")
    
    pixels_per_line = 16
    pixel_lines = [pixels[i:i + pixels_per_line] for i in range(0, len(pixels), pixels_per_line)]
    pixels_str = ',\n    '.join([', '.join(line) for line in pixel_lines])
    
    footer = f"""
}};

// Image object
static const ImageData {base_name} = {{
    {width},    // width
    {height},   // height
    {base_name}_data  // data
}};

#endif // {base_name.upper()}_H
"""
    
    with open(output_path, 'w') as f:
        f.write(header + "\n    " + pixels_str + footer)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 imageconverter.py input_image.png output_file.h")
        sys.exit(1)
    
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    convert_image_to_rgb565(input_file, output_file)
