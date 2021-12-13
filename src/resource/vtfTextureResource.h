#pragma once

#include <resource/textureResource.h>
#include <loader/vtfImage.h>

using namespace chira;

class vtfTextureResource : public textureResource {
public:
    explicit vtfTextureResource(const std::string& identifier_, bool vFlip_ = true);
    void compile(const unsigned char buffer[], std::size_t bufferLen) override;
};
