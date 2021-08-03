#include "vtfImage.h"
#include "../../external/VTFLib/src/VTFLib.h"
#include "../../external/ChiraEngine/src/core/engine.h"

vtfImage::vtfImage(const std::string& filepath, unsigned int* width, unsigned int* height, int* glFormat, bool vFlip, int currentFrame, int face) : abstractImage() {
    VTFLib::CVTFFile vtf;
    if (!vtf.Load(filepath.c_str())) {
        engine::logError("VTFImage", "Could not load VTF at " + filepath);
    }
    *width = vtf.GetWidth();
    *height = vtf.GetHeight();
    if (VTFLib::CVTFFile::GetImageFormatInfo(vtf.GetFormat()).uiAlphaBitsPerPixel > 0) {
        *glFormat = GL_RGBA8;
    } else {
        *glFormat = GL_RGB8;
    }
    this->data = reinterpret_cast<unsigned char*>(vtf.GetData(currentFrame, 0, face, 0));
    if (!this->data) {
        engine::logWarning("VTFImage", "VTF at " + filepath + " has no data");
        return;
    }
}

vtfImage::~vtfImage() {
    delete this->data;
}
