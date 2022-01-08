#pragma once

#include <render/texture.h>
#include <utility/logger.h>
#include <resource/vtfTextureResource.h>
#include <i18n/translationManager.h>

using namespace chira;

class TextureVTF : public Texture {
public:
    explicit TextureVTF(const std::string& identifier_, bool cacheTexture = true) : Texture(identifier_, cacheTexture) {}
    void compile(const nlohmann::json& properties) override;
    [[nodiscard]] unsigned int getWidth() const {
        return assert_cast<VTFImage*>(this->file->getFile())->getHeight();
    }
    [[nodiscard]] unsigned int getHeight() const {
        return assert_cast<VTFImage*>(this->file->getFile())->getWidth();
    }
    [[nodiscard]] int getFormat() const {
        return assert_cast<VTFImage*>(this->file->getFile())->getGLFormat();
    }
};
