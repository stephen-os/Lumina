#include "VertexAttributes.h"

namespace GL
{
    VertexAttributes::VertexAttributes(const VertexAttributes& other)
        : m_VertexCount(other.m_VertexCount), m_MaxAttributeCount(other.m_MaxAttributeCount),
        m_IndexBuffer(other.m_IndexBuffer)
    {
        for (const auto& attribute : other.m_Attributes)
        {
            m_Attributes.emplace_back(attribute);
        }
    }

    VertexAttributes::VertexAttributes(VertexAttributes&& other) noexcept
        : m_VertexCount(other.m_VertexCount),
        m_MaxAttributeCount(other.m_MaxAttributeCount),
        m_IndexBuffer(std::move(other.m_IndexBuffer))
    {
        m_Attributes = std::move(other.m_Attributes);

        other.m_VertexCount = -1;
        other.m_MaxAttributeCount = 0;
    }

    VertexAttributes& VertexAttributes::operator=(const VertexAttributes& other)
    {
        if (this != &other)
        {
            Destroy(); 

            m_VertexCount = other.m_VertexCount;
            m_MaxAttributeCount = other.m_MaxAttributeCount;

            m_IndexBuffer = other.m_IndexBuffer;

            m_Attributes.reserve(other.m_Attributes.size());
            for (const auto& attribute : other.m_Attributes)
            {
                m_Attributes.emplace_back(attribute);
            }
        }
        return *this;
    }

    void VertexAttributes::Destroy()
    {
        m_IndexBuffer.Destroy(); 
        
        for (auto& attribute : m_Attributes)
        {
            attribute.Destroy();
        }
        m_Attributes.clear(); 
    }

    void VertexAttributes::AddVertices(const std::string& name, int location, const float* data, int count, int stride, GLenum usage)
    {
        if (m_Attributes.size() >= m_MaxAttributeCount)
        {
            std::cerr << "[ERROR] Failed to add vertex buffer \"" << name << "\".\n";
            std::cerr << "Exceeded maximum number of allowed buffers (" << m_MaxAttributeCount << ").\n";
            std::cerr << "Current buffer count: " << m_Attributes.size() << ".\n";
            return;
        }

        m_VertexCount = count; 
        m_Attributes.emplace_back(name, location, data, count, stride, usage);
    }

    void VertexAttributes::AddIndices(const unsigned int* data, const int count, GLenum usage)
    {
        m_IndexBuffer.SetData(data, count, usage);
    }
}