#include "textureVTF.h"

using namespace chira;

void TextureVTF::compile(const nlohmann::json& properties) {
    this->format = getFormatFromString(getPropertyOrDefault<std::string>(properties["properties"], "format", std::string("RGBA")));
    this->wrapModeS = getWrapModeFromString(getPropertyOrDefault<std::string>(properties["properties"], "wrap_mode_s", "REPEAT"));
    this->wrapModeT = getWrapModeFromString(getPropertyOrDefault<std::string>(properties["properties"], "wrap_mode_t", "REPEAT"));
    this->filterMode = getFilterModeFromString(getPropertyOrDefault<std::string>(properties["properties"], "filter_mode", "LINEAR"));
    this->mipmaps = getPropertyOrDefault<bool>(properties["properties"], "mipmaps", true);
    auto vtf = Resource::getResource<VTFTextureResource>(properties["dependencies"]["image"], getPropertyOrDefault<bool>(properties["properties"], "vertical_flip", true));

    if (this->activeTextureUnit == -1)
        glActiveTexture(GL_TEXTURE0);
    else
        glActiveTexture(this->activeTextureUnit);
    glBindTexture(GL_TEXTURE_2D, this->handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapModeS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapModeT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMode);

    if (vtf.get() && vtf->getData()) {
        glTexImage2D(GL_TEXTURE_2D, 0, this->format, vtf->getWidth(), vtf->getHeight(), 0, this->format, GL_UNSIGNED_BYTE, vtf->getData());
        if (this->mipmaps)
            glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        Logger::log(LogType::ERROR, "VTF Texture", TR("error.opengl.texture_compile"));
    }

    if (this->cache)
        this->file = vtf.castDynamic<TextureResource>();
}
