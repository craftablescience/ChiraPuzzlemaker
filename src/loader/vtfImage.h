#pragma once

#include "../../external/ChiraEngine/src/loader/image.h"
#include "../../external/VTFLib/src/VTFLib.h"

class vtfImage : public abstractImage {
public:
    vtfImage(const std::string& filepath, unsigned int* width, unsigned int* height, int* glFormat, bool vFlip = false, int currentFrame = 0, int face = 0);
    explicit vtfImage(const std::string& filepath, bool vFlip = false, int currentFrame = 0, int face = 0);
    ~vtfImage();
    unsigned int getWidth();
    unsigned int getHeight();
    int getGLFormat();

    VTFLib::CVTFFile vtf{};
};
