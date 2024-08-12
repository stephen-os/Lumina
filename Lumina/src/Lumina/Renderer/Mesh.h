#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "../OpenGL/ShaderProgram.h"
#include "../OpenGL/VertexAttributes.h"
#include "../OpenGL/VertexArray.h"

class Mesh 
{
public:
    Mesh(float* vertices, float* normals, unsigned int* indices, size_t vertexCount, size_t indicesCount);
    ~Mesh(); 

    const float* GetVertices() const { return m_Vertices; };
    const float* GetNormals() const { return m_Normals; };
    const unsigned int* GetIndices() const { return m_Indices; };

    size_t GetVertexCount() const { return m_VertexCount; };
    size_t GetIndexCount() const { return m_IndexCount; };

    void Draw() const;
    void AttachShader(GL::ShaderProgram& shader);
private:
    float* m_Vertices; 
    float* m_Normals;
    unsigned int* m_Indices;

    size_t m_VertexCount; 
    size_t m_IndexCount; 
    
    GL::ShaderProgram* m_Shader = nullptr;
    GL::VertexAttributes* m_Attributes = nullptr; 
    GL::VertexArray* m_VertexArray = nullptr; 
};