#pragma once

#include <render/texture.h>
#include <utility/logger.h>
#include <resource/vtfTextureResource.h>
#include <i18n/translationManager.h>

using namespace chira;

class vtfTexture : public texture {
public:
    explicit vtfTexture(const std::string& identifier_, bool cacheTexture = true) : texture(identifier_, cacheTexture) {}
    void compile(const nlohmann::json& properties) override;
    unsigned int getWidth() {
        return dynamic_cast<vtfImage*>(this->file.get())->getHeight();
    }
    unsigned int getHeight() {
        return dynamic_cast<vtfImage*>(this->file.get())->getWidth();
    }
    int getFormat() {
        return dynamic_cast<vtfImage*>(this->file.get())->getGLFormat();
    }
};
