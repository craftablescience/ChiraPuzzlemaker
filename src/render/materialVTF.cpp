#include "materialVTF.h"

#include <resource/resource.h>

void MaterialVTF::compile(const nlohmann::json& properties) {
    MaterialUntextured::compile(properties);
    this->vtfTex = Resource::getResource<TextureVTF>(properties["dependencies"]["texture"]);
    this->vtfTex->setTextureUnit(GL_TEXTURE0);
}

void MaterialVTF::use() const {
    this->vtfTex->use();
    MaterialUntextured::use();
}
