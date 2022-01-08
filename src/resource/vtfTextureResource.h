#pragma once

#include <loader/vtfImage.h>
#undef ERROR
#include <resource/textureResource.h>

using namespace chira;

class VTFTextureResource : public TextureResource {
public:
    explicit VTFTextureResource(const std::string& identifier_, bool vFlip_ = true);
    void compile(const unsigned char buffer[], std::size_t bufferLen) override;
};
