#pragma once

#include <string>
#include "../../external/VTFLib/src/VTFLib.h"
#undef ERROR
#include <loader/image/IImage.h>

class VTFImage : public chira::IImage {
public:
    VTFImage(const unsigned char* buffer, std::size_t bufferLen, bool vFlip = false, int currentFrame = 0, int face = 0);
    VTFImage(const unsigned char* buffer, std::size_t bufferLen, unsigned int* width, unsigned int* height, int* glFormat, bool vFlip = false, int currentFrame = 0, int face = 0);
    explicit VTFImage(const std::string& filepath, bool vFlip = false, int currentFrame = 0, int face = 0);
    VTFImage(const std::string& filepath, unsigned int* width, unsigned int* height, int* glFormat, bool vFlip = false, int currentFrame = 0, int face = 0);
    ~VTFImage() override;
    [[nodiscard]] unsigned int getWidth() const;
    [[nodiscard]] unsigned int getHeight() const;
    [[nodiscard]] int getGLFormat() const;
    [[nodiscard]] unsigned char* getData() const override;
private:
    VTFLib::CVTFFile vtf{};
};
