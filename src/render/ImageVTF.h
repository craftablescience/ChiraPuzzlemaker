#pragma once

#include <VTFFile.h>
#include <render/texture/Image.h>

class ImageVTF : public chira::Image {
public:
    explicit ImageVTF(std::string identifier_, bool vFlip_ = true) : Image(std::move(identifier_), vFlip_) {}
    ~ImageVTF() override;
    void compile(const byte buffer[], std::size_t bufferLen) override;
    [[nodiscard]] int getGLFormat() const;
    [[nodiscard]] byte* getVTFData() const;
private:
    VTFLib::CVTFFile vtf{};
};
