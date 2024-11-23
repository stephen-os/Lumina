#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "../OpenGL/VertexAttributes.h"
#include "../OpenGL/VertexArray.h"
#include "../OpenGL/InstanceBuffer.h"
#include "../OpenGL/ShaderProgram.h"
#include "../OpenGL/Texture.h"

#include "../Renderer/Transform.h"

#include <vector>

struct CubeData {
    std::vector<float> positions = {
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f 
    };

    std::vector<float> normals = {
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,

        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f
    };

    std::vector<float> texCoords = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0,

        5, 4, 7,
        7, 6, 5,

        3, 2, 6,
        6, 7, 3,

        4, 5, 1,
        1, 0, 4,

        1, 5, 6,
        6, 2, 1,

        4, 0, 3,
        3, 7, 4
    };
};


class Cube
{
public:
    Cube();

    void Draw(GL::ShaderProgram& shader, std::vector<glm::mat4>& instanceTransforms);
private: 
    void InitializeGeometry();
private:
    GL::VertexArray m_VertexArray; 
    GL::VertexAttributes m_Attributes; 
    GL::Texture m_Texture;

    Transform m_Transform;
};
