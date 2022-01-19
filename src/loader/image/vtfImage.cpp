#include "vtfImage.h"

#include <core/engine.h>
#include <i18n/translationManager.h>
#include <fmt/core.h>

using namespace chira;

VTFImage::VTFImage(const unsigned char* buffer, std::size_t bufferLen, bool vFlip, int currentFrame, int face) {
    // todo: use vFlip parameter
    VTFLib::CVTFFile tmpFile;
    if (!tmpFile.Load(buffer, bufferLen - 1))
        Logger::log(LogType::ERROR, "VTF Image", TR("error.vtfimage.load_from_buffer"));
    this->vtf.Create(tmpFile.GetWidth(), tmpFile.GetHeight());
    VTFLib::CVTFFile::ConvertToRGBA8888(
            tmpFile.GetData(currentFrame, 0, face, 0),
            this->vtf.GetData(0, 0, 0, 0),
            tmpFile.GetWidth(),
            tmpFile.GetHeight(),
            tmpFile.GetFormat());
}

VTFImage::VTFImage(const unsigned char* buffer, std::size_t bufferLen, unsigned int* width, unsigned int* height, int* glFormat, bool vFlip, int currentFrame, int face) : VTFImage(buffer, bufferLen, vFlip, currentFrame, face) {
    *width = this->vtf.GetWidth();
    *height = this->vtf.GetHeight();
    *glFormat = this->getGLFormat();
}

VTFImage::VTFImage(const std::string& filepath, bool vFlip, int currentFrame, int face) {
    // todo: use vFlip parameter
    VTFLib::CVTFFile tmpFile;
    if (!tmpFile.Load(filepath.c_str()))
        Logger::log(LogType::ERROR, "VTF Image", TRF("error.vtfimage.load_from_file", filepath));
    this->vtf.Create(tmpFile.GetWidth(), tmpFile.GetHeight());
    VTFLib::CVTFFile::ConvertToRGBA8888(
            tmpFile.GetData(currentFrame, 0, face, 0),
            this->vtf.GetData(0, 0, 0, 0),
            tmpFile.GetWidth(),
            tmpFile.GetHeight(),
            tmpFile.GetFormat());
}

VTFImage::VTFImage(const std::string& filepath, unsigned int* width, unsigned int* height, int* glFormat, bool vFlip, int currentFrame, int face) : VTFImage(filepath, vFlip, currentFrame, face) {
    *width = this->vtf.GetWidth();
    *height = this->vtf.GetHeight();
    *glFormat = this->getGLFormat();
}

VTFImage::~VTFImage() {
    this->vtf.Destroy();
}

unsigned int VTFImage::getWidth() const {
    return vtf.GetWidth();
}

unsigned int VTFImage::getHeight() const {
    return vtf.GetHeight();
}

int VTFImage::getGLFormat() const {
    if (VTFLib::CVTFFile::GetImageFormatInfo(this->vtf.GetFormat()).uiAlphaBitsPerPixel > 0)
        return GL_RGBA;
    else
        return GL_RGB;
}

unsigned char* VTFImage::getData() const {
    return reinterpret_cast<unsigned char*>(this->vtf.GetData(0, 0, 0, 0));
}
