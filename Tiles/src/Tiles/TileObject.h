#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Lumina/OpenGL/VertexAttributes.h"
#include "Lumina/OpenGL/VertexArray.h"
#include "Lumina/OpenGL/ShaderProgram.h"

#include "Lumina/Renderer/Transform.h"
#include "Lumina/Renderer/TextureAtlas.h"

struct TileData {
    std::vector<float> positions = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
    };

    std::vector<float> normals = {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    std::vector<float> texCoords = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };
};

class TileObject {
public:
    TileObject();

    void Draw(GL::ShaderProgram& shader, int textureIndex);
    GL::VertexAttributes& GetAttributes() { return m_Attributes; }
private:
    void InitializeGeometry();
private:
    GL::VertexArray m_VertexArray;
    GL::VertexAttributes m_Attributes;

    Transform m_Transform;
    TextureAtlas m_TextureAtlas;

    int m_InstanceCount = 0;
};
