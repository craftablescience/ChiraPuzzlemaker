#include "mdlMeshLoader.h"

#include "../../external/ChiraEngine/src/loader/abstractMeshLoader.h"
#include "../../external/ChiraEngine/src/core/engine.h"
#include <iostream>
#include <fstream>
#include "../../external/ChiraEngine/src/third_party/openalsoft/common/alfstream.h"


struct MDL {
    studiohdr_t header;
    MDL(char* bytes) {
        memcpy(bytes, &header, sizeof(studiohdr_t));
    }
    static char* bufferator(std::string filepath) {

        std::ifstream is(filepath, std::ifstream::binary);
        if (is) {
            // get length of file:
            is.seekg(0, is.end);
            int length = is.tellg();
            is.seekg(0, is.beg);

            char buffer = char(length);

            std::cout << "Reading " << length << " characters... ";
            // read data as a block:
            is.read(&buffer, length);
            
            if (is)
                std::cout << "all characters read successfully.";
            else
                std::cout << "error: only " << is.gcount() << " could be read";
            is.close();

            // ...buffer contains the entire file...
            MDL* medel = new MDL(&buffer);
            std::cout << medel->header.name;
            return &buffer;
        }
        return 0;
    }
};



mdlLoader::mdlLoader(const std::string& filepath) {
    // keep in mind that this just returns the absolute path to the file, not the file itself
    const int mainlineValue = 408;
    std::string rawMDL = virtualFileSystem::getMeshPath(filepath);
    //std::vector<unsigned char> rawMDL2;
    //char* rawMDL2 = new char[mainlineValue];

   

    char* rawMDL2 = MDL::bufferator(rawMDL);

    //engine::logInfo("Trico Premodel", rawMDL2);
    if (sizeof(rawMDL2) >= mainlineValue) {
        char* sos = &rawMDL2[0, mainlineValue];
        MDL model(sos);
        //engine::logInfo("Trico success", model.header.name);
    } else {
       // engine::logInfo("Trico fail", rawMDL2);
    }



}


