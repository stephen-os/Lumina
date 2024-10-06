#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "../OpenGL/ShaderProgram.h"
#include "../OpenGL/VertexAttributes.h"
#include "../OpenGL/VertexArray.h"

#include <optional>
#include <string>

class Mesh 
{
public:
    Mesh(float* vertices, float* normals, unsigned int* indices, size_t vertexCount, size_t indicesCount);
    Mesh(const std::string& filename); 
    ~Mesh() {};

    size_t GetVertexCount() const { return m_VertexCount; };
    size_t GetIndexCount() const { return m_IndexCount; };

    void Draw();
    void AttachShader(GL::ShaderProgram& shader);
private:
    void LoadGLTF(const std::string& filename); 
private:
    size_t m_VertexCount; 
    size_t m_IndexCount; 
    
    GL::VertexAttributes m_Attributes = GL::VertexAttributes(2);
    std::optional<GL::VertexArray> m_VertexArray; 
};