#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../OpenGL/ShaderProgram.h"
#include "../OpenGL/VertexAttributes.h"
#include "../OpenGL/VertexArray.h"

#include "Transform.h"

#include <optional>
#include <string>
#include <vector>

class Mesh 
{
public:
    Mesh(float* vertices, float* normals, unsigned int* indices, size_t vertexCount, size_t indicesCount);
    Mesh(const std::string& filename); 
    ~Mesh() {};

    void Draw(GL::ShaderProgram& shader);
    void AttachShader(GL::ShaderProgram& shader);

    void AttachProjection(glm::mat4 projection) { m_Projection = projection; };
    void AttachView(glm::mat4 view) { m_View = view; }
private:
    void LoadGLTF(const std::string& filename); 
private:
    glm::mat4 m_Projection;
    glm::mat4 m_View;
    std::vector<Transform> m_Transforms;

    std::vector<GL::VertexAttributes> m_Attributes;
    std::vector<GL::VertexArray> m_VertexArrays; 
};