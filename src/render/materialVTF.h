#pragma once

#include <render/material/materialUntextured.h>
#include "textureVTF.h"

class MaterialVTF : public chira::MaterialUntextured {
public:
    explicit MaterialVTF(const std::string& identifier_) : MaterialUntextured(identifier_) {}
    void compile(const nlohmann::json& properties) override;
    void use() const override;
protected:
    chira::SharedPointer<TextureVTF> vtfTex;
private:
    REGISTER_MATERIAL_TYPE(MaterialVTF);
};
