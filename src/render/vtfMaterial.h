#pragma once

#include "../../external/ChiraEngine/src/render/material.h"
#include "vtfTexture.h"

class vtfMaterial : public material {
public:
    vtfMaterial(const std::string& provider_, const std::string& name_) : material(provider_, name_) {}
    void compile(const nlohmann::json& properties) override;
    void use() override;
    void release() const override;
private:
    vtfTexture* vtfTex = nullptr;
};
