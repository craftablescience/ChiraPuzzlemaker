#pragma once

#include "../../external/ChiraEngine/src/loader/image.h"

class vtfImage : public abstractImage {
public:
    vtfImage(const std::string& filepath, unsigned int* width, unsigned int* height, int* glFormat, bool vFlip = false, int currentFrame = 0, int face = 0);
    ~vtfImage();
};
