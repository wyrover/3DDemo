#pragma once

namespace ImageProcessing
{

struct Bitmap decode_bitmap_from_pcx_memory(_In_reads_(size) const uint8_t* pcx_memory, size_t size);

}

