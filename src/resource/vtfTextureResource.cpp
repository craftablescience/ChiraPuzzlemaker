#include "vtfTextureResource.h"

VTFTextureResource::VTFTextureResource(const std::string& identifier_, bool vFlip_) : TextureResource(identifier_, vFlip_) {}

void VTFTextureResource::compile(const unsigned char* buffer, std::size_t bufferLen) {
    this->file = std::make_unique<VTFImage>(buffer, bufferLen);
    // Casts suck
    this->width = static_cast<int>(assert_cast<VTFImage*>(this->file.get())->getWidth());
    this->height = static_cast<int>(assert_cast<VTFImage*>(this->file.get())->getHeight());
    this->bitDepth = 0;
}
