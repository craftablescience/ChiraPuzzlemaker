#include "ImageVTF.h"

#include <glad/gl.h>
#include <i18n/TranslationManager.h>

using namespace chira;

ImageVTF::~ImageVTF() {
    this->vtf.Destroy();
}

void ImageVTF::compile(const byte buffer[], std::size_t bufferLen) {
    // todo: use vFlip parameter
    VTFLib::CVTFFile tmpFile;
    if (!tmpFile.Load(buffer, bufferLen - 1))
        Logger::log(LogType::LOG_ERROR, "VTF Image", TR("error.vtfimage.load_from_buffer"));
    this->vtf.Create(tmpFile.GetWidth(), tmpFile.GetHeight());
    VTFLib::CVTFFile::ConvertToRGBA8888(
            tmpFile.GetData(0, 0, 0, 0),
            this->vtf.GetData(0, 0, 0, 0),
            tmpFile.GetWidth(),
            tmpFile.GetHeight(),
            tmpFile.GetFormat());

    this->width = static_cast<int>(this->vtf.GetWidth());
    this->height = static_cast<int>(this->vtf.GetHeight());
    this->bitDepth = 0;
}

int ImageVTF::getGLFormat() const {
    if (VTFLib::CVTFFile::GetImageFormatInfo(this->vtf.GetFormat()).uiAlphaBitsPerPixel > 0) {
        return GL_RGBA;
    } else {
        return GL_RGB;
    }
}

byte* ImageVTF::getVTFData() const {
    return reinterpret_cast<byte*>(this->vtf.GetData(0, 0, 0, 0));
}
