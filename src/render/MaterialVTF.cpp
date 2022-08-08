#include "MaterialVTF.h"

#include <glad/gl.h>
#include <resource/Resource.h>

void MaterialVTF::compile(const nlohmann::json& properties) {
    IMaterial::compile(properties);
    this->vtfTex = Resource::getResource<TextureVTF>(properties["image"]);
    this->vtfTex->setTextureUnit(GL_TEXTURE0);
}

void MaterialVTF::use() const {
    this->vtfTex->use();
    IMaterial::use();
}
