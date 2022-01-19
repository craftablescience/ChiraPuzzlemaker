#pragma once

#include <loader/image/vtfImage.h>
#include <resource/textureResource.h>

class VTFTextureResource : public chira::TextureResource {
public:
    explicit VTFTextureResource(const std::string& identifier_, bool vFlip_ = true);
    void compile(const unsigned char buffer[], std::size_t bufferLen) override;
};
