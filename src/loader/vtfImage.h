#pragma once

#include "../../external/ChiraEngine/src/loader/image.h"

class vtfImage : public image {
public:
    vtfImage(const std::string& filepath, int* width, int* height, int* fileChannels, int desiredChannels, bool vflip = true);
};
