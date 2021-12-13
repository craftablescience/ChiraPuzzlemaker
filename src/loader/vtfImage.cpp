#include "vtfImage.h"

#include <core/engine.h>
#include <i18n/translationManager.h>
#include <fmt/core.h>

using namespace chira;

vtfImage::vtfImage(const unsigned char* buffer, std::size_t bufferLen, bool vFlip, int currentFrame, int face) {
    // todo: use vFlip parameter
    VTFLib::CVTFFile tmpFile;
    if (!tmpFile.Load(buffer, bufferLen - 1))
        logger::log(ERR, "VTF Image", TR("error.vtfimage.load_from_buffer"));
    this->vtf.Create(tmpFile.GetWidth(), tmpFile.GetHeight());
    VTFLib::CVTFFile::ConvertToRGBA8888(
            tmpFile.GetData(currentFrame, 0, face, 0),
            this->vtf.GetData(0, 0, 0, 0),
            tmpFile.GetWidth(),
            tmpFile.GetHeight(),
            tmpFile.GetFormat());
}

vtfImage::vtfImage(const unsigned char* buffer, std::size_t bufferLen, unsigned int* width, unsigned int* height, int* glFormat, bool vFlip, int currentFrame, int face) : vtfImage(buffer, bufferLen, vFlip, currentFrame, face) {
    *width = this->vtf.GetWidth();
    *height = this->vtf.GetHeight();
    *glFormat = this->getGLFormat();
}

vtfImage::vtfImage(const std::string& filepath, bool vFlip, int currentFrame, int face) {
    // todo: use vFlip parameter
    VTFLib::CVTFFile tmpFile;
    if (!tmpFile.Load(filepath.c_str()))
        chira::logger::log(ERR, "VTF Image", fmt::format(TR("error.vtfimage.load_from_file"), filepath));
    this->vtf.Create(tmpFile.GetWidth(), tmpFile.GetHeight());
    VTFLib::CVTFFile::ConvertToRGBA8888(
            tmpFile.GetData(currentFrame, 0, face, 0),
            this->vtf.GetData(0, 0, 0, 0),
            tmpFile.GetWidth(),
            tmpFile.GetHeight(),
            tmpFile.GetFormat());
}

vtfImage::vtfImage(const std::string& filepath, unsigned int* width, unsigned int* height, int* glFormat, bool vFlip, int currentFrame, int face) : vtfImage(filepath, vFlip, currentFrame, face) {
    *width = this->vtf.GetWidth();
    *height = this->vtf.GetHeight();
    *glFormat = this->getGLFormat();
}

vtfImage::~vtfImage() {
    this->vtf.Destroy();
}

unsigned int vtfImage::getWidth() {
    return vtf.GetWidth();
}

unsigned int vtfImage::getHeight() {
    return vtf.GetHeight();
}

int vtfImage::getGLFormat() {
    if (VTFLib::CVTFFile::GetImageFormatInfo(this->vtf.GetFormat()).uiAlphaBitsPerPixel > 0) {
        return GL_RGBA;
    } else {
        return GL_RGB;
    }
}

unsigned char* vtfImage::getData() {
    return reinterpret_cast<unsigned char*>(this->vtf.GetData(0, 0, 0, 0));
}
