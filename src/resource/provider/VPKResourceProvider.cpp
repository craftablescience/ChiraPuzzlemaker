#include "VPKResourceProvider.h"

#include <algorithm>
#include <hook/SteamAPI.h>
#include <resource/Resource.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <utility/String.h>

using namespace chira;

VPKResourceProvider::VPKResourceProvider(unsigned int appID, const std::string& relativePath)
    : IResourceProvider(SOURCE_PROVIDER_NAME)
    , absolutePath(calculateAbsolutePath(appID, relativePath))
    , vpk(this->absolutePath) {}

bool VPKResourceProvider::hasResource(std::string_view name) const {
    return !this->vpk.findEntry(std::string{name}).fileName.empty();
}

void VPKResourceProvider::compileResource(std::string_view name, chira::Resource* resource) const {
    auto entry = this->vpk.findEntry(std::string{name});
    std::vector<byte> output;
    this->vpk.readEntry(entry, output);
    output.push_back('\0');
    resource->compile(reinterpret_cast<const unsigned char*>(&output[0]), output.size());
}

std::string VPKResourceProvider::calculateAbsolutePath(unsigned int appID, const std::string& relativePath) {
    std::string path = SteamAPI::Apps::getAppInstallPath(appID);
    FilesystemResourceProvider::nixifyPath(path);
    path = String::stripRight(path, '/');
    std::string relPath = relativePath;
    FilesystemResourceProvider::nixifyPath(relPath);
    relPath = String::stripLeft(relPath, '/');
    return path + '/' + relPath;
}

FilesystemResourceProvider* VPKResourceProvider::getFilesystemResourceProvider(unsigned int appID, const std::string& relativePath) {
    return new FilesystemResourceProvider{VPKResourceProvider::calculateAbsolutePath(appID, relativePath), true, SOURCE_PROVIDER_NAME};
}
