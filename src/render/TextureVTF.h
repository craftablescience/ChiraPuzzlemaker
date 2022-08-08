#pragma once

#include <glad/gl.h>

#include <core/Logger.h>
#include <resource/Properties.h>
#include "render/texture/ITexture.h"

#include "ImageVTF.h"

class TextureVTF : public chira::ITexture {
public:
    explicit TextureVTF(std::string identifier_) : ITexture(std::move(identifier_)) {}
    void compile(const nlohmann::json& properties) override;
    void use() override;
private:
    chira::SharedPointer<ImageVTF> image;
    std::string filePath{"file://textures/missing.png"};
    int wrapModeS = GL_REPEAT;
    std::string wrapModeSStr{"REPEAT"};
    int wrapModeT = GL_REPEAT;
    std::string wrapModeTStr{"REPEAT"};
    int filterMode = GL_LINEAR;
    std::string filterModeStr{"LINEAR"};
    bool mipmaps = true;
    bool verticalFlip = true;
private:
    void setFilterMode(std::string filterModeStr_);
    void setWrapModeS(std::string wrapModeSStr_);
    void setWrapModeT(std::string wrapModeTStr_);
public:
    CHIRA_PROPS (
            CHIRA_PROP(TextureVTF, verticalFlip),
            CHIRA_PROP(TextureVTF, mipmaps),
            CHIRA_PROP_NAMED(TextureVTF, filePath, image),
            CHIRA_PROP_NAMED_SET(TextureVTF, wrapModeSStr, wrapModeS, setWrapModeS),
            CHIRA_PROP_NAMED_SET(TextureVTF, wrapModeTStr, wrapModeT, setWrapModeT),
            CHIRA_PROP_NAMED_SET(TextureVTF, filterModeStr, filterMode, setFilterMode)
    );
};
