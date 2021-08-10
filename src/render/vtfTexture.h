#pragma once

#include "../../external/ChiraEngine/src/render/texture2d.h"
#include "../loader/vtfImage.h"

class vtfTexture : public texture2d {
public:
    explicit vtfTexture(const std::string& file);
    unsigned int getWidth() {
        return this->vtf.vtf.GetWidth();
    }
    unsigned int getHeight() {
        return this->vtf.vtf.GetWidth();
    }
private:
    vtfImage* vtf;
    abstractImage* getTexture(const std::string& file);
};
