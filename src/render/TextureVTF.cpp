#include "TextureVTF.h"

#include <i18n/TranslationManager.h>

using namespace chira;

void TextureVTF::compile(const nlohmann::json& properties) {
    Serialize::fromJSON(this, properties);

    auto vtf = Resource::getResource<ImageVTF>(this->filePath, this->verticalFlip);

    glGenTextures(1, &this->handle);

    if (this->activeTextureUnit == -1) {
        glActiveTexture(GL_TEXTURE0);
    } else {
        glActiveTexture(this->activeTextureUnit);
    }
    glBindTexture(GL_TEXTURE_2D, this->handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapModeS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapModeT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMode);

    if (vtf->getVTFData()) {
        glTexImage2D(GL_TEXTURE_2D, 0, vtf->getGLFormat(), vtf->getWidth(), vtf->getHeight(), 0, vtf->getGLFormat(), GL_UNSIGNED_BYTE, vtf->getVTFData());
        if (this->mipmaps) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    } else {
        Logger::log(LogType::LOG_ERROR, "TextureVTF", TR("error.opengl.texture_compile"));
    }
}

void TextureVTF::use() {
    if (this->handle == 0) return;
    if (this->activeTextureUnit == -1) {
        glActiveTexture(GL_TEXTURE0);
    } else {
        glActiveTexture(this->activeTextureUnit);
    }
    glBindTexture(GL_TEXTURE_2D, this->handle);
}

void TextureVTF::setFilterMode(std::string filterModeStr_) {
    this->filterModeStr = std::move(filterModeStr_);
    this->filterMode = ITexture::getFilterModeFromString(this->filterModeStr);
}

void TextureVTF::setWrapModeS(std::string wrapModeSStr_) {
    this->wrapModeSStr = std::move(wrapModeSStr_);
    this->wrapModeS = ITexture::getWrapModeFromString(this->wrapModeSStr);
}

void TextureVTF::setWrapModeT(std::string wrapModeTStr_) {
    this->wrapModeTStr = std::move(wrapModeTStr_);
    this->wrapModeT = ITexture::getWrapModeFromString(this->wrapModeTStr);
}
