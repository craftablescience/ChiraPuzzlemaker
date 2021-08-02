#include "vtfImage.h"
#include "../../external/VTFLib/src/VTFLib.h"
#include "../../external/ChiraEngine/src/core/engine.h"

vtfImage::vtfImage(const std::string& filepath, int* width, int* height, int* fileChannels, int desiredChannels, bool vflip):
    image(filepath, width, height, fileChannels, desiredChannels, vflip) {

    VTFLib::CVTFFile vtf;
    vlUInt w = vtf.GetWidth();
    vlUInt h = vtf.GetHeight();
    VTFImageFormat srcformat = vtf.GetFormat();
    VTFImageFormat dstformat;

    int glFormat;
    if (VTFLib::CVTFFile::GetImageFormatInfo(srcformat).uiAlphaBitsPerPixel > 0) {
        dstformat = IMAGE_FORMAT_RGBA8888;
        glFormat = GL_RGBA8;
    } else {
        dstformat = IMAGE_FORMAT_RGB888;
        glFormat = GL_RGB8;
    }

    int currentFrame = 0;
    const vlByte* frame = vtf.GetData(currentFrame, 0, 0, 0);

    if (!frame) {
        engine::logWarning("VTFImage", "VTF at " + filepath + " has no data");
        return;
    }
}
