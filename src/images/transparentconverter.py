from PIL import Image
import os
import sys
import re

def rgb_to_rgb565(r, g, b, a=255):
    # First create RGB565 value
    rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
    # Swap bytes
    msb = (rgb565 >> 8) & 0xFF
    lsb = rgb565 & 0xFF
    return (lsb << 8) | msb

def update_images_h(image_name):
    # Change the path to look in the current directory
    images_h_path = "images.h"
    
    with open(images_h_path, 'r') as f:
        content = f.read()
    
    # Add include if not present
    include_line = f'#include "{image_name}.h"'
    if include_line not in content:
        # Find the last #include line
        last_include_pos = content.rfind('#include')
        last_include_end = content.find('\n', last_include_pos) + 1
        content = content[:last_include_end] + include_line + '\n' + content[last_include_end:]
    
    # Add external declaration if not present
    extern_line = f'extern const TransparentImageData {image_name};'
    if extern_line not in content:
        # Find the section for transparent image declarations
        transparent_section = content.find('// Create arrays for regular and transparent images')
        if transparent_section != -1:
            insert_pos = content.rfind('\n', 0, transparent_section) + 1
            content = content[:insert_pos] + extern_line + '\n' + content[insert_pos:]
    
    # Update TRANSPARENT_IMAGES array
    array_start = content.find('static const TransparentImageData* const TRANSPARENT_IMAGES[] = {')
    if array_start != -1:
        array_end = content.find('};', array_start)
        current_array = content[array_start:array_end]
        
        if f'&{image_name}' not in current_array:
            # If array is empty (only contains comment)
            if '// Empty for now' in current_array:
                new_array = current_array.replace('// Empty for now', f'    &{image_name}')
            else:
                # Add new image to array
                new_array = current_array.rstrip() + f',\n    &{image_name}'
            content = content[:array_start] + new_array + content[array_end:]
    
    # Update enum
    enum_start = content.find('enum TransparentImageIndex {')
    if enum_start != -1:
        enum_end = content.find('};', enum_start)
        current_enum = content[enum_start:enum_end]
        
        # Extract existing enum values
        enum_values = re.findall(r'(\w+)\s*,', current_enum)
        
        # Create new enum entry
        enum_name = image_name.upper()
        if enum_name not in enum_values:
            # Find the position of TRANSPARENT_IMAGE_COUNT
            count_pos = current_enum.find('TRANSPARENT_IMAGE_COUNT')
            if count_pos != -1:
                # Insert new enum value before TRANSPARENT_IMAGE_COUNT
                new_enum = current_enum[:count_pos].rstrip() + f',\n    {enum_name},\n    '
                new_enum += 'TRANSPARENT_IMAGE_COUNT'
                content = content[:enum_start] + new_enum + content[enum_end:]
    
    # Write updated content back to file
    with open(images_h_path, 'w') as f:
        f.write(content)

def convert_transparent_image(input_path, output_path):
    # Open and convert image to RGBA mode
    img = Image.open(input_path)
    
    # Print image info for debugging
    print(f"Original image mode: {img.mode}")
    print(f"Transparency info: {img.info.get('transparency')}")
    
    img = img.convert('RGBA')
    width, height = img.size
    
    print(f"Image size: {width}x{height}")
    print("Checking for transparency...")
    
    # Check if image has any transparent pixels
    has_transparency = False
    for y in range(height):
        for x in range(width):
            if img.getpixel((x, y))[3] < 255:
                has_transparency = True
                print(f"Found transparent pixels")
                break
        if has_transparency:
            break
    
    if not has_transparency:
        print("WARNING: No transparent pixels found in image!")
    
    base_name = os.path.splitext(os.path.basename(output_path))[0]
    
    header = f"""// Auto-generated file from {os.path.basename(input_path)}
#ifndef {base_name.upper()}_H
#define {base_name.upper()}_H

#include "../TransparentImageData.h"

// Image data array
static const uint16_t {base_name}_data[{width * height}] PROGMEM = {{"""

    # Convert pixels to RGB565 format and collect alpha values
    pixels = []
    alphas = []
    
    for y in range(height):
        for x in range(width):
            r, g, b, a = img.getpixel((x, y))
            rgb565 = rgb_to_rgb565(r, g, b)
            pixels.append(f"0x{rgb565:04X}")
            # Make sure alpha is properly captured
            alphas.append(str(a))
    
    # Format pixel data
    pixels_per_line = 16
    pixel_lines = [pixels[i:i + pixels_per_line] for i in range(0, len(pixels), pixels_per_line)]
    pixels_str = ',\n    '.join([', '.join(line) for line in pixel_lines])
    
    # Add alpha data
    alpha_header = f"""
}};

// Alpha channel array (0 = transparent, 255 = opaque)
static const uint8_t {base_name}_alpha[{width * height}] PROGMEM = {{"""
    
    alpha_lines = [alphas[i:i + pixels_per_line] for i in range(0, len(alphas), pixels_per_line)]
    alpha_str = ',\n    '.join([', '.join(line) for line in alpha_lines])
    
    footer = f"""
}};

// Transparent image object
static const TransparentImageData {base_name} = {{
    {width},    // width
    {height},   // height
    {base_name}_data,  // color data
    {base_name}_alpha  // alpha data
}};

#endif // {base_name.upper()}_H
"""
    
    # Write everything to file
    with open(output_path, 'w') as f:
        f.write(header + "\n    " + pixels_str + alpha_header + "\n    " + alpha_str + footer)
    
    # Update images.h
    update_images_h(base_name)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 transparentconverter.py input_image.png output_file.h")
        sys.exit(1)
    
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    convert_transparent_image(input_file, output_file)
