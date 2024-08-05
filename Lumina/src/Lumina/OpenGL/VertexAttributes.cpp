#include "VertexAttributes.h"

namespace GL
{
    void VertexAttributes::AddVertices(const std::string& name, int count, int components, const float* floats, GLenum usage)
    {
        if (m_VertexCount >= 0 && count != this->m_VertexCount)
        {
            throw std::runtime_error("Attributes must have same number of vertices.");
        }
        m_VertexCount = count; 
        m_Attributes.emplace_back(std::make_unique<VertexBuffer>(name, count, components, floats, usage));
    }

    void VertexAttributes::AddIndices(const unsigned int* indices, const unsigned int count, GLenum usage)
    {
        m_IndexBuffer = new IndexBuffer(indices, count, usage);
    }
}