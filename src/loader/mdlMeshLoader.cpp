#include "mdlMeshLoader.h"

#include "../../external/ChiraEngine/src/loader/abstractMeshLoader.h"
#include "../../external/ChiraEngine/src/core/engine.h"

struct MDL {
    studiohdr_t header;
    MDL(byte* bytes) {
        memcpy(bytes, &header, sizeof(studiohdr_t));
    }
};

mdlLoader::mdlLoader(const std::string& filepath) {
    // keep in mind that this just returns the absolute path to the file, not the file itself
    std::string rawMDL = virtualFileSystem::getMeshPath(filepath);
    byte sos = (rawMDL[0, 408]);
    MDL model(&sos);
    engine::logInfo("Trico", model.header.name);
}
