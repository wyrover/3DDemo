#pragma once

#include "Bitmap.h"

namespace ImageProcessing
{

Bitmap decode_bitmap_from_tga_memory(_In_reads_(size) const uint8_t* tga_memory, size_t size);
std::vector<uint8_t> encode_tga_from_bitmap(const Bitmap& bitmap);

}

