#include "VertexAttributes.h"

#include <iostream>

namespace GL
{
    void VertexAttributes::Destroy()
    {
        if (m_IndexBuffer)
        {
            m_IndexBuffer->Destroy();
            m_IndexBuffer.reset(); 
        }
        
        for (auto& attribute : m_Attributes)
        {
            attribute.Destroy();
        }
        m_Attributes.clear(); 
    }

    void VertexAttributes::AddVertices(const std::string& name, int count, int components, const float* floats, GLenum usage)
    {
        if (m_VertexCount >= 0 && count != m_VertexCount)
        {
            std::cerr << "[ERROR] Failed to add vertices for attribute \"" << name << "\".\n";
            std::cerr << "Expected vertex count: " << m_VertexCount << ", but received: " << count << ".\n";
            std::cerr << "Ensure all attributes have the same number of vertices.\n";
            return; 
        }

        if (m_Attributes.size() >= m_MaxAttributeCount)
        {
            std::cerr << "[ERROR] Failed to add vertex buffer \"" << name << "\".\n";
            std::cerr << "Exceeded maximum number of allowed buffers (" << m_MaxAttributeCount << ").\n";
            std::cerr << "Current buffer count: " << m_Attributes.size() << ".\n";
            return;
        }

        m_VertexCount = count; 
        m_Attributes.emplace_back(name, count, components, floats, usage);
    }

    void VertexAttributes::AddIndices(const unsigned int* indices, const unsigned int count, GLenum usage)
    {
        m_IndexBuffer.emplace(indices, count, usage);
    }
}