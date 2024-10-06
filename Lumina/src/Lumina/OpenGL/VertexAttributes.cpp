#include "VertexAttributes.h"

namespace GL
{
    void VertexAttributes::AddVertices(const std::string& name, int count, int components, const float* floats, GLenum usage)
    {
        if (m_VertexCount >= 0 && count != m_VertexCount)
        {
            throw std::runtime_error("Attributes must have same number of vertices.");
        }

        if (m_AttributeCount == m_MaxAttributeCount)
        {
            throw std::runtime_error("Vertex buffer (" + name + ") exceeds the allowed number of buffers for this attributes container.");
        }

        m_VertexCount = count; 
        m_Attributes.emplace_back(name, count, components, floats, usage);
    }

    void VertexAttributes::AddIndices(const unsigned int* indices, const unsigned int count, GLenum usage)
    {
        m_IndexBuffer.emplace(indices, count, usage);
    }
}