#include "mdlMeshLoader.h"

#include "../../external/ChiraEngine/src/loader/abstractMeshLoader.h"
#include "../../external/ChiraEngine/src/core/engine.h"
#include <iostream>
#include <fstream>
#include "../../external/ChiraEngine/src/third_party/openalsoft/common/alfstream.h"


struct MDL {
    studiohdr_t m_header;

    MDL(studiohdr_t header) {
        m_header = header;
    }
};



mdlLoader::mdlLoader(const std::string& filepath) {
    // keep in mind that this just returns the absolute path to the file, not the file itself
    const int mainlineValue = 408;
    std::string rawMDL = virtualFileSystem::getMeshPath(filepath);
    std::string rawvtx = virtualFileSystem::getMeshPath("info_catboy_start.vtx");
    //std::vector<unsigned char> rawMDL2;
    //char* rawMDL2 = new char[mainlineValue];

    std::fstream fs;

    studiohdr_t model;
    VTX_FileHeader_t vertexData;

    fs.open(rawMDL,std::ios::in | std::ios::binary);

    if (fs.is_open()) {

        /*
        a lot of the logs are to read the current position, they're handy to see what is going on.
        */

        engine::logInfo("Trico pre read", std::to_string(fs.tellg()));
        fs.read(reinterpret_cast<char*>(&model), sizeof(studiohdr_t)); //getting model, works fine.
        engine::logInfo("Trico post read", std::to_string(fs.tellg()));
        fs.seekg((int)fs.tellg() + model.texture_offset);
        engine::logInfo("Trico post offset", std::to_string(fs.tellg()));
        std::vector<mstudiotexture_t> textures; //getting texture, questionable, i have no way to verify if this works.
        for (int i = 0;  i < model.texture_count; i++) {
            engine::logInfo("Trico pre texture fetch", std::to_string(fs.tellg()));
            int cto = sizeof(mstudiotexture_t) + fs.tellg();
            fs.seekg(cto);
            engine::logInfo("Trico post texture fetch", std::to_string(fs.tellg()));
            mstudiotexture_t texture;
            fs.read(reinterpret_cast<char*>(&texture), sizeof(mstudiotexture_t));

            engine::logInfo("Trico current texture offset", std::to_string(fs.tellg()));
            engine::logInfo("Trico offset texture offset", std::to_string(texture.name_offset));
            fs.seekg(texture.name_offset + fs.tellg());
            engine::logInfo("Trico current texture offset result", std::to_string(fs.tellg()));
            std::string name;

            char ch;
            int iterations = 0;
            while ((ch = fs.get()) != '\0' && iterations < 100) {
                
                name += ch;
                iterations++;
                engine::logInfo("Trico current texture offset result", std::to_string(iterations));
            }
            engine::logInfo("Trico texture name_offset: ", std::to_string(texture.name_offset));
            engine::logInfo("Trico texture name after offset name grab: ", name);
            textures.push_back(texture);
        }

        fs.close();
    } 

    fs.open(rawvtx, std::ios::in | std::ios::binary);

    if (fs.is_open()) {

        fs.read(reinterpret_cast<char*>(&vertexData), sizeof(VTX_FileHeader_t));
        fs.close();
    }
    
    //std::cout << model.name;
    engine::logInfo("Trico Model Name: ", model.name);
    engine::logInfo("Trico Model ID: ", std::to_string(model.id));
    engine::logInfo("Trico Model Version: ", std::to_string(model.version));
    engine::logInfo("Trico Model texture_count: ", std::to_string(model.texture_count));
    engine::logInfo("Trico Model texture_offset: ", std::to_string(model.texture_offset));
    //studiohdr2index
    
    
    /*
    char* rawMDL2 = MDL::bufferator(rawMDL);

    //engine::logInfo("Trico Premodel", rawMDL2);
    if (sizeof(rawMDL2) >= mainlineValue) {
        char* sos = &rawMDL2[0, mainlineValue];
        MDL model(sos);
        //engine::logInfo("Trico success", model.header.name);
    } else {
       // engine::logInfo("Trico fail", rawMDL2);
    }

    */

}


