#include "VertexAttribute.h"

#include <iostream>

namespace GL
{
    VertexAttribute::VertexAttribute(const std::string& name, int vertices, int components, const std::vector<float>& floats, GLenum usage)
    {
        m_Name = name;
        m_Vertices = vertices;
        m_Components = components;
        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
        glBufferData(GL_ARRAY_BUFFER, floats.size() * sizeof(float), floats.data(), usage);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexAttribute::Update(const std::vector<float>& floats)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, floats.size() * sizeof(float), floats.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexAttribute::Destroy()
    {
        glDeleteBuffers(1, &m_Buffer);
    }
}