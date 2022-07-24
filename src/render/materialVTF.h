#pragma once

#include <render/material/MaterialUntextured.h>
#include "textureVTF.h"

class MaterialVTF : public chira::MaterialUntextured {
public:
    explicit MaterialVTF(const std::string& identifier_) : MaterialUntextured(identifier_) {}
    void compile(const nlohmann::json& properties) override;
    void use() const override;
protected:
    chira::SharedPointer<TextureVTF> vtfTex;
private:
    CHIRA_REGISTER_MATERIAL_TYPE(MaterialVTF); // NOLINT(cert-err58-cpp)
};
