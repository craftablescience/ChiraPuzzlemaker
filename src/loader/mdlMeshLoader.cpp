#include "mdlMeshLoader.h"

#include "../../external/ChiraEngine/src/loader/abstractMeshLoader.h"
#include "../../external/ChiraEngine/src/core/engine.h"
#include <iostream>
#include <fstream>
#include "../../external/ChiraEngine/src/third_party/openalsoft/common/alfstream.h"


struct MDL {
    studiohdr_t header;
    MDL(byte* bytes) {
        memcpy(bytes, &header, sizeof(studiohdr_t));
    }
};

mdlLoader::mdlLoader(const std::string& filepath) {
    // keep in mind that this just returns the absolute path to the file, not the file itself
    std::string rawMDL = virtualFileSystem::getMeshPath(filepath);
    char rawMDL2[408];
    //std::ifstream myFile(rawMDL, std::ios::in | std::ios::binary);
    std::ifstream infile;
    infile.open(rawMDL, std::ios::binary);
    infile.read(rawMDL2,408);
    rawMDL = rawMDL2;
    engine::logInfo("Trico Premodel", rawMDL);
    // rawMDL = myFile.;
    if (rawMDL.length() >= 408) {
        byte sos = rawMDL[0, 408];
        MDL model(&sos);
        engine::logInfo("Trico success", model.header.name);
    } else {
        engine::logInfo("Trico fail", rawMDL);
    }
}
