#pragma once

#include <memory>
#include "../../external/ChiraEngine/src/render/texture2d.h"
#include "../loader/vtfImage.h"

class vtfTexture : public texture2d {
public:
    explicit vtfTexture(const std::string& file) : texture2d(nullptr, 0, 0, 0, 0) {
        this->file = std::make_unique<vtfImage>(file);
        this->width = (int) this->getWidth();
        this->height = (int) this->getHeight();
        this->bitDepth = 0;
        this->format = this->getFormat();
    }
    unsigned int getWidth() {
        return ((vtfImage*) this->file.get())->getHeight();
    }
    unsigned int getHeight() {
        return ((vtfImage*) this->file.get())->getWidth();
    }
    int getFormat() {
        return ((vtfImage*) this->file.get())->getGLFormat();
    }
};
