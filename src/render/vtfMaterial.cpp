#include "vtfMaterial.h"

#include <resource/resource.h>

void vtfMaterial::compile(const nlohmann::json& properties) {
    material::compile(properties);
    this->vtfTex = resource::getResource<vtfTexture>(properties["dependencies"]["texture"]);
    this->vtfTex->setTextureUnit(GL_TEXTURE0);
}

void vtfMaterial::use() {
    this->vtfTex->use();
    material::use();
}
