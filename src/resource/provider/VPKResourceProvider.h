#pragma once

#include <vpktool/VPK.h>
#include <resource/provider/IResourceProvider.h>

const std::string SOURCE_PROVIDER_NAME = "source"; // NOLINT(cert-err58-cpp)

namespace chira {
    class FilesystemResourceProvider;
}

class VPKResourceProvider : public chira::IResourceProvider {
public:
    explicit VPKResourceProvider(unsigned int appID, const std::string& relativePath);
    [[nodiscard]] bool hasResource(std::string_view name) const override;
    void compileResource(std::string_view name, chira::Resource* resource) const override;
    [[nodiscard]] std::string getPath() const {
        return this->absolutePath;
    }
    [[nodiscard]] const vpktool::VPK& getVPK() const {
        return this->vpk;
    }
    [[nodiscard]] static std::string calculateAbsolutePath(unsigned int appID, const std::string& relativePath);
    [[nodiscard]] static chira::FilesystemResourceProvider* getFilesystemResourceProvider(unsigned int appID, const std::string& relativePath);
private:
    std::string absolutePath;
    vpktool::VPK vpk;
};
