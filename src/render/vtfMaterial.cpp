#include "vtfMaterial.h"
#include "../../external/ChiraEngine/src/resource/resourceManager.h"

void vtfMaterial::compile(const nlohmann::json& properties) {
    material::compile(properties);
    this->vtfTex = resourceManager::getResource<vtfTexture>(this->provider, properties["dependencies"]["texture"]);
    this->vtfTex->setTextureUnit(GL_TEXTURE0);
}

void vtfMaterial::use() {
    this->vtfTex->use();
    material::use();
}

void vtfMaterial::release() const {
    this->vtfTex->release();
    material::release();
}
