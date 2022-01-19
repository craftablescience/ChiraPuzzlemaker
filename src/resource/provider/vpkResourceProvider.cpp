#include "vpkResourceProvider.h"

#include <algorithm>
#include <hook/steamAPI.h>
#include <resource/resource.h>
#include <resource/provider/filesystemResourceProvider.h>
#include <utility/string/stringStrip.h>

using namespace chira;

VPKResourceProvider::VPKResourceProvider(unsigned int appID, const std::string& relativePath)
    : AbstractResourceProvider(SOURCE_PROVIDER_NAME)
    , absolutePath(calculateAbsolutePath(appID, relativePath))
    , vpk(this->absolutePath) {}

bool VPKResourceProvider::hasResource(const std::string& name) const {
    return !this->vpk.findEntry(name).fileName.empty();
}

void VPKResourceProvider::compileResource(const std::string& name, chira::Resource* resource) const {
    auto entry = this->vpk.findEntry(name);
    std::vector<byte> output;
    this->vpk.readEntry(entry, output);
    output.push_back('\0');
    resource->compile(reinterpret_cast<const unsigned char*>(&output[0]), output.size());
}

std::string VPKResourceProvider::calculateAbsolutePath(unsigned int appID, const std::string& relativePath) {
    std::string path = SteamAPI::Apps::getAppInstallPath(appID);
    FilesystemResourceProvider::nixifyPath(path);
    stripRightModify(path, '/');
    std::string relPath = relativePath;
    FilesystemResourceProvider::nixifyPath(relPath);
    stripLeftModify(relPath, '/');
    return path + '/' + relPath;
}

FilesystemResourceProvider* VPKResourceProvider::getFilesystemResourceProvider(unsigned int appID, const std::string& relativePath) {
    std::string path = SteamAPI::Apps::getAppInstallPath(appID);
    FilesystemResourceProvider::nixifyPath(path);
    stripRightModify(path, '/');
    std::string relPath = relativePath;
    FilesystemResourceProvider::nixifyPath(relPath);
    stripLeftModify(relPath, '/');
    return new FilesystemResourceProvider{path + '/' + relPath, true, SOURCE_PROVIDER_NAME};
}
