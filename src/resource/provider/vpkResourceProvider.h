#pragma once

#include <resource/provider/abstractResourceProvider.h>
#include <loader/vpk/vpk.h>

const std::string SOURCE_PROVIDER_NAME = "source"; // NOLINT(cert-err58-cpp)

namespace chira {
    class FilesystemResourceProvider;
}

class VPKResourceProvider : public chira::AbstractResourceProvider {
public:
    explicit VPKResourceProvider(unsigned int appID, const std::string& relativePath);
    [[nodiscard]] bool hasResource(const std::string& name) const override;
    void compileResource(const std::string& name, chira::Resource* resource) const override;
    [[nodiscard]] std::string getPath() const {
        return this->absolutePath;
    }
    const VPK& getVPK() const {
        return this->vpk;
    }
    [[nodiscard]] static std::string calculateAbsolutePath(unsigned int appID, const std::string& relativePath);
    [[nodiscard]] static chira::FilesystemResourceProvider* getFilesystemResourceProvider(unsigned int appID, const std::string& relativePath);
private:
    std::string absolutePath;
    VPK vpk;
};
