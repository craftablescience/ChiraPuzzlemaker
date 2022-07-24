#pragma once

#include <core/Logger.h>
#include <render/Texture.h>
#include <resource/vtfTextureResource.h>

class TextureVTF : public chira::Texture {
public:
    explicit TextureVTF(const std::string& identifier_, bool cacheTexture = true)
        : Texture(identifier_, cacheTexture) {}
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
