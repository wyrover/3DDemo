#ifndef TARGA_H
#define TARGA_H

#include "Bitmap.h"

bool TGADecodeRGB(const char* szFileName, struct Bitmap* spr);
Bitmap decode_bitmap_from_tga_memory(_In_count_(size) const uint8_t* tga_memory, size_t size);

#endif

