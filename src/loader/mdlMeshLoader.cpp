#include "mdlMeshLoader.h"

#include "../../external/ChiraEngine/src/core/engine.h"
#include "../utility/mdlHeaders.h"
#include "../../external/ChiraEngine/src/resource/stringResource.h"
#include <iostream>

struct MDL {
    studiohdr_t m_header{};
    mstudiotexture_t m_textureData{};
    std::vector<mstudiotexture_t> m_textures;
    MDL(studiohdr_t header, mstudiotexture_t textureData, const std::vector<mstudiotexture_t>& texture) {
        m_header = header;
        m_textureData = textureData;
        m_textures = texture;
    }
};

void mdlMeshLoader::loadMesh(const std::string& provider, const std::string& name, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) {
    auto rawMDL = resourceManager::getResourceWithoutCaching<stringResource>(provider, name + ".mdl");
    std::unique_ptr<stringResource> rawVTX;
    if (!(rawVTX = resourceManager::getResourceWithoutCaching<stringResource>(provider, name + ".vtx"))) {
        if (!(rawVTX = resourceManager::getResourceWithoutCaching<stringResource>(provider, name + ".dx90.vtx"))) {
            chira::logger::log(ERR, "MDL", "Unable to find VTX model for " + name);
        }
    }
    //std::vector<unsigned char> rawMDL2;
    //char* rawMDL2 = new char[mainlineValue];

    std::fstream fs;
    studiohdr_t model{};
    VTX_FileHeader_t vertexData{};
    fs.open(rawMDL->getString(), std::ios::in | std::ios::binary);
    if (fs.is_open()) {
        /*
        a lot of the logs are to read the current position, they're handy to see what is going on.
        */

        fs.read(reinterpret_cast<char*>(&model), sizeof(studiohdr_t)); //getting model, works fine.
        //fs.seekg(model.texture_offset + sizeof(studiohdr_t));
        std::vector<mstudiotexture_t> textures; //getting texture, questionable, i have no way to verify if this works.
        for (int i = 0;  i < model.texture_count; i++) {
            std::size_t cto = (int)model.texture_offset + (int)(i * (int)sizeof(mstudiotexture_t));
            chira::logger::log(INFO, "Trico number ", std::to_string(model.texture_offset + i * (int)sizeof(mstudiotexture_t)));
            fs.seekg((long long) cto, std::ios::beg);
            mstudiotexture_t texture{};
            fs.read(reinterpret_cast<char*>(&texture), sizeof(mstudiotexture_t));
            fs.seekg(((long long) cto) + (texture.name_offset), std::ios::beg);
            chira::logger::log(INFO, "Trico texture offset name", std::to_string(texture.name_offset));
            std::string mdlName;
            std::getline(fs, mdlName, '\0');

            chira::logger::log(INFO, "Trico texture name_offset: ", std::to_string(texture.name_offset));
            chira::logger::log(INFO, "Trico texture name after offset name grab: ", mdlName);
            textures.push_back(texture);
        }
        fs.close();
    } 

    fs.open(rawVTX->getString(), std::ios::in | std::ios::binary);

    if (fs.is_open()) {

        fs.read(reinterpret_cast<char*>(&vertexData), sizeof(VTX_FileHeader_t));
        fs.close();
    }

    //std::cout << model.name;
    chira::logger::log(INFO, "Trico Model Name: ", model.name);
    chira::logger::log(INFO, "Trico Model ID: ", std::to_string(model.id));
    chira::logger::log(INFO, "Trico Model Version: ", std::to_string(model.version));
    chira::logger::log(INFO, "Trico Model texture_count: ", std::to_string(model.texture_count));
    chira::logger::log(INFO, "Trico Model texture_offset: ", std::to_string(model.texture_offset));
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
