#pragma once

namespace ImageProcessing
{

#pragma pack(push)
#pragma pack(1)
struct Color_rgb
{
    // Empty default constructor is used to prevent zero-init when creating a std::vector<ColorRGB>.
    // These vectors are used as write targets for reads from disk, where zero-init would be inefficient.
    Color_rgb() {}

    uint8_t red;
    uint8_t green;
    uint8_t blue;
};
#pragma pack(pop)

struct Bitmap
{
    Bitmap();

// http://stackoverflow.com/questions/70013/how-to-detect-if-im-compiling-code-with-visual-studio-2008
#if _MSC_VER <= 1800
    // Add default and move constructors as VS2010/2012/2013 doesn't default define a move constructor.
    Bitmap(Bitmap&& other);
    Bitmap& operator=(Bitmap&& other) NOEXCEPT;
#else
#error This compiler may autodefine the default move constructor.
#endif

    std::vector<Color_rgb> bitmap;
    unsigned int xsize;
    unsigned int ysize;
    bool filtered;
};

}

