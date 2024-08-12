// GLTFLoader.h
#pragma once
#include <string>
#include <vector>
#include "Mesh.h"

#include <tiny_gltf.h>

class GLTFLoader {
public:
    static Mesh loadFromFile(const std::string& filename);
private:
    static Mesh processMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh);
};