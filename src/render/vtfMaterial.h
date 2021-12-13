#pragma once

#include <render/material.h>
#include "vtfTexture.h"

using namespace chira;

class vtfMaterial : public material {
public:
    explicit vtfMaterial(const std::string& identifier_) : material(identifier_) {}
    void compile(const nlohmann::json& properties) override;
    void use() override;
private:
    sharedPointer<vtfTexture> vtfTex;
};
