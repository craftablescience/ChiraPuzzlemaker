#include "mdlMeshLoader.h"

#include "../../external/ChiraEngine/src/loader/abstractMeshLoader.h"
#include "../../external/ChiraEngine/src/core/engine.h"
#include <iostream>
#include <fstream>
#include "../../external/ChiraEngine/src/third_party/openalsoft/common/alfstream.h"

//: public abstractMeshLoader
struct MDL {
    studiohdr_t m_header;
    mstudiotexture_t m_textureData;
    std::vector<mstudiotexture_t> m_textures;
    MDL(studiohdr_t header, mstudiotexture_t textureData, std::vector<mstudiotexture_t> texture) {
        m_header = header;
        m_textureData = textureData;
        m_textures = texture;
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

    studiohdr_t model{};
    VTX_FileHeader_t vertexData{};

    fs.open(rawMDL,std::ios::in | std::ios::binary);

    if (fs.is_open()) {

        /*
        a lot of the logs are to read the current position, they're handy to see what is going on.
        */

        fs.read(reinterpret_cast<char*>(&model), sizeof(studiohdr_t)); //getting model, works fine.
        //fs.seekg(model.texture_offset + sizeof(studiohdr_t));
        std::vector<mstudiotexture_t> textures; //getting texture, questionable, i have no way to verify if this works.
        for (int i = 0;  i < model.texture_count; i++) {
            std::size_t cto = (int)model.texture_offset + (int)(i * (int)sizeof(mstudiotexture_t));
            engine::logInfo("Trico number ", std::to_string(model.texture_offset + i * (int)sizeof(mstudiotexture_t)));
            fs.seekg(cto,std::ios::beg);
            mstudiotexture_t texture;
            fs.read(reinterpret_cast<char*>(&texture), sizeof(mstudiotexture_t));
            fs.seekg(cto + (texture.name_offset), std::ios::beg);
            engine::logInfo("Trico texture offset name", std::to_string(texture.name_offset));
            std::string name = "";
            char ch;
            std::getline(fs, name, '\0');


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

void mdlLoader::loadMesh(const std::string& filepath, std::vector<vertex>* vertices, std::vector<unsigned int>* indices)
{
}


