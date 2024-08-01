#include "VertexAttributes.h"

namespace GL
{
    VertexAttribute& VertexAttributes::AddAttribute(const std::string& name, int vertices, int components, const std::vector<float>& floats, GLenum usage)
    {
        if (this->m_Vertices >= 0 && vertices != this->m_Vertices)
        {
            throw std::runtime_error("Attributes must have same number of vertices.");
        }
        this->m_Vertices = vertices;
        m_Attributes.emplace_back(VertexAttribute{ name, vertices, components, floats, usage });
    }

    void VertexAttributes::AddIndices(const std::vector<unsigned int>& ints, GLenum usage)
    {
        m_IndexCount = ints.size();
        glGenBuffers(1, &m_IndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ints.size() * sizeof(unsigned int), ints.data(), usage);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void VertexAttributes::Destroy()
    {
        for (auto& attribute : m_Attributes)
        {
            attribute.Destroy();
        }
        if (m_IndexBuffer)
        {
            glDeleteBuffers(1, &m_IndexBuffer);
        }
    }
}