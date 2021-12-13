#include "vtfTextureResource.h"

vtfTextureResource::vtfTextureResource(const std::string& identifier_, bool vFlip_) : textureResource(identifier_, vFlip_) {}

void vtfTextureResource::compile(const unsigned char* buffer, std::size_t bufferLen) {
    this->file = std::make_unique<vtfImage>(buffer, bufferLen);
    // Casts suck, couldn't think of a better way off the top of my head
    this->width = static_cast<int>(dynamic_cast<vtfImage*>(this->file.get())->getWidth());
    this->height = static_cast<int>(dynamic_cast<vtfImage*>(this->file.get())->getHeight());
    this->bitDepth = 0;
}
