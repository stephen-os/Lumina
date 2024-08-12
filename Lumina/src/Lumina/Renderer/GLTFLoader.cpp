// GLTFLoader.cpp
#include "GLTFLoader.h"
#include <iostream>

Mesh GLTFLoader::loadFromFile(const std::string& filename) {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);

    if (!warn.empty()) {
        std::cout << "GLTF Warning: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << "GLTF Error: " << err << std::endl;
    }

    if (!ret) {
        std::cerr << "Failed to load glTF file: " << filename << std::endl;
        return Mesh(nullptr, nullptr, nullptr, 0, 0);
    }

    if (model.meshes.empty()) {
        std::cerr << "No meshes found in the glTF file." << std::endl;
        return Mesh(nullptr, nullptr, nullptr, 0, 0);
    }

    return processMesh(model, model.meshes[0]);
}

Mesh GLTFLoader::processMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh) {
    float* vertices = nullptr;
    float* normals = nullptr;
    unsigned int* indices = nullptr;
    size_t vertexCount = 0;
    size_t indexCount = 0;

    for (const auto& primitive : mesh.primitives) {
        const auto& posAccessor = model.accessors[primitive.attributes.at("POSITION")];
        const auto& posBufferView = model.bufferViews[posAccessor.bufferView];
        const auto& posBuffer = model.buffers[posBufferView.buffer];

        vertexCount = posAccessor.count;
        vertices = new float[vertexCount * 3];
        memcpy(vertices, &posBuffer.data[posBufferView.byteOffset + posAccessor.byteOffset], vertexCount * 3 * sizeof(float));

        if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) {
            const auto& normalAccessor = model.accessors[primitive.attributes.at("NORMAL")];
            const auto& normalBufferView = model.bufferViews[normalAccessor.bufferView];
            const auto& normalBuffer = model.buffers[normalBufferView.buffer];

            normals = new float[vertexCount * 3];
            memcpy(normals, &normalBuffer.data[normalBufferView.byteOffset + normalAccessor.byteOffset], vertexCount * 3 * sizeof(float));
        }

        const auto& indicesAccessor = model.accessors[primitive.indices];
        const auto& indicesBufferView = model.bufferViews[indicesAccessor.bufferView];
        const auto& indicesBuffer = model.buffers[indicesBufferView.buffer];

        indexCount = indicesAccessor.count;
        indices = new unsigned int[indexCount];

        if (indicesAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
            const uint16_t* indicesData = reinterpret_cast<const uint16_t*>(&indicesBuffer.data[indicesBufferView.byteOffset + indicesAccessor.byteOffset]);
            for (size_t i = 0; i < indexCount; ++i) {
                indices[i] = static_cast<unsigned int>(indicesData[i]);
            }
        }
        else if (indicesAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
            memcpy(indices, &indicesBuffer.data[indicesBufferView.byteOffset + indicesAccessor.byteOffset], indexCount * sizeof(unsigned int));
        }

        // We're only processing the first primitive for simplicity
        break;
    }

    return Mesh(vertices, normals, indices, vertexCount, indexCount);
}