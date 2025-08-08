from PIL import Image

def rgb888_to_rgb565(r, g, b):
    """Convert 8-bit R,G,B to 16-bit RGB565 format."""
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

# Load the image and convert to RGB
img = Image.open("heartbeat.png").convert("RGB")
width, height = img.size
pixels = list(img.getdata())

# Write header file with auto width and height
with open("heartbeat_array.h", "w") as f:
    f.write(f"const uint16_t heartbeat_image[{height}][{width}] = {{\n")
    for y in range(height):
        f.write("  {")
        for x in range(width):
            r, g, b = pixels[y * width + x]
            color565 = rgb888_to_rgb565(r, g, b)
            f.write(f"0x{color565:04X}, ")
        f.write("},\n")
    f.write("};\n")
