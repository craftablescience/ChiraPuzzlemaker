#pragma once

#include "../../external/ChiraEngine/src/render/texture2d.h"
#include "../../external/ChiraEngine/src/utility/logger.h"
#include "../loader/vtfImage.h"

using namespace chira;

class vtfTexture : public texture2d {
public:
    explicit vtfTexture(const std::string& identifier_, bool vFlip_ = true, int wrapModeU = GL_REPEAT, int wrapModeV = GL_REPEAT, int filterMode = GL_LINEAR) : texture2d(identifier_, 0, vFlip_, wrapModeU, wrapModeV, filterMode) {}
    void compile(unsigned char* buffer, std::size_t bufferLen) override {
        this->file = std::make_unique<vtfImage>(buffer, bufferLen);
        this->width = (int) this->getWidth();
        this->height = (int) this->getHeight();
        this->bitDepth = 0;
        this->format = this->getFormat();

        if (this->activeTextureUnit == -1) {
            glActiveTexture(GL_TEXTURE0);
        } else {
            glActiveTexture(this->activeTextureUnit);
        }
        glBindTexture(GL_TEXTURE_2D, this->handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapModeU);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapModeV);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMode);

        if (this->file && this->file->getData()) {
            glTexImage2D(GL_TEXTURE_2D, 0, this->format, this->width, this->height, 0, this->format, GL_UNSIGNED_BYTE, this->file->getData());
            if (this->mipmaps) {
                glGenerateMipmap(GL_TEXTURE_2D);
            }
        } else {
            // todo(i18n)
            logger::log(ERR, "Texture2D", "Texture failed to compile. Missing image data");
        }
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
