#include "vtfTexture.h"

#include "../../external/ChiraEngine/src/core/engine.h"

vtfTexture::vtfTexture(const std::string& file) : texture2d(getTexture(file), 7, 7, 7, 7) {}

abstractImage* vtfTexture::getTexture(const std::string& file) {
    if (!this->vtf) {
        this->vtf = new vtfImage{file};
    }
    return this->vtf;
}
