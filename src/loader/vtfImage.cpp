#include "vtfImage.h"
#include "../../external/ChiraEngine/src/core/engine.h"

vtfImage::vtfImage(const std::string& filepath, unsigned int* width, unsigned int* height, int* glFormat, bool vFlip, int currentFrame, int face) : abstractImage() {
    if (!this->vtf.Load(filepath.c_str())) {
        engine::logError("VTFImage", "Could not load VTF at " + filepath);
    }
    *width = this->vtf.GetWidth();
    *height = this->vtf.GetHeight();
    if (VTFLib::CVTFFile::GetImageFormatInfo(this->vtf.GetFormat()).uiAlphaBitsPerPixel > 0) {
        *glFormat = GL_RGBA8;
    } else {
        *glFormat = GL_RGB8;
    }
    this->data = reinterpret_cast<unsigned char*>(this->vtf.GetData(currentFrame, 0, face, 0));
    if (!this->data) {
        engine::logWarning("VTFImage", "VTF at " + filepath + " has no data");
        return;
    }
}

vtfImage::vtfImage(const std::string& filepath, bool vFlip, int currentFrame, int face) {
    if (!this->vtf.Load(filepath.c_str())) {
        engine::logError("VTFImage", "Could not load VTF at " + filepath);
    }
    this->data = reinterpret_cast<unsigned char*>(this->vtf.GetData(currentFrame, 0, face, 0));
    if (!this->data) {
        engine::logWarning("VTFImage", "VTF at " + filepath + " has no data");
        return;
    }
}

vtfImage::~vtfImage() {
    delete this->data;
}

unsigned int vtfImage::getWidth() {
    return vtf.GetWidth();
}

unsigned int vtfImage::getHeight() {
    return vtf.GetHeight();
}

int vtfImage::getGLFormat() {
    if (VTFLib::CVTFFile::GetImageFormatInfo(this->vtf.GetFormat()).uiAlphaBitsPerPixel > 0) {
        return GL_RGBA8;
    } else {
        return GL_RGB8;
    }
}
