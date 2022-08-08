#pragma once

#include <render/material/MaterialUntextured.h>
#include "TextureVTF.h"

class MaterialVTF : public chira::IMaterial {
public:
    explicit MaterialVTF(std::string identifier_) : IMaterial(std::move(identifier_)) {}
    void compile(const nlohmann::json& properties) override;
    void use() const override;
protected:
    chira::SharedPointer<TextureVTF> vtfTex;
private:
    CHIRA_REGISTER_MATERIAL_TYPE(MaterialVTF); // NOLINT(cert-err58-cpp)
};
