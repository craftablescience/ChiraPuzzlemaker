#pragma once

#include <glm/ext/vector_float3.hpp>
#include <string>
#include <vector>
#include "../../external/ChiraEngine/src/utility/vertex.h"
#include "../../external/ChiraEngine/src/loader/abstractMeshLoader.h"

class mdlMeshLoader : public abstractMeshLoader {
public:
	void loadMesh(const std::string& provider, const std::string& name, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) override;
};
