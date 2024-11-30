#include "VertexAttributes.h"

namespace GL
{
    VertexAttributes::VertexAttributes(const VertexAttributes& other)
        : m_IndexBuffer(other.m_IndexBuffer),
        m_InstanceBuffer(other.m_InstanceBuffer)
    {
        for (const auto& attribute : other.m_Attributes)
        {
            m_Attributes.emplace_back(attribute.first, attribute.second);
        }
    }

    VertexAttributes::VertexAttributes(VertexAttributes&& other) noexcept
        : m_IndexBuffer(std::move(other.m_IndexBuffer)),
        m_InstanceBuffer(std::move(other.m_InstanceBuffer)),
        m_Attributes(std::move(other.m_Attributes))
    {
    }

    VertexAttributes& VertexAttributes::operator=(const VertexAttributes& other)
    {
        if (this != &other)
        {
            Destroy();
            m_IndexBuffer = other.m_IndexBuffer;
            m_InstanceBuffer = other.m_InstanceBuffer;
            m_Attributes = other.m_Attributes;
        }
        return *this;
    }

    void VertexAttributes::Destroy()
    {
        m_IndexBuffer.Delete();
        for (auto& attribute : m_Attributes)
        {
            attribute.second.Delete();
        }
        m_Attributes.clear();
    }

    void VertexAttributes::AddVertices(const std::string& name, int location, const float* data, int count, int stride, GLenum usage)
    {
        Buffer buffer;
        buffer.Init();
        buffer.SetData(GL_ARRAY_BUFFER, data, count, stride, usage);
        m_Attributes.emplace_back(location, std::move(buffer));

        // Enable the vertex attribute array
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, stride / sizeof(float), GL_FLOAT, GL_FALSE, stride, nullptr);
    }

    void VertexAttributes::AddInstances(const std::string& name, int location, int size, unsigned int type, int stride, unsigned int divisor, int offset)
    {
        m_InstanceBuffer.SetName(name);
        m_InstanceBuffer.SetInstanceAttribute(location, size, type, stride, divisor, offset);
    }

    void VertexAttributes::AddIndices(const unsigned int* data, const int count, GLenum usage)
    {
        m_IndexBuffer.Init();
        m_IndexBuffer.SetData(GL_ELEMENT_ARRAY_BUFFER, data, count, sizeof(unsigned int), usage);
    }

    void VertexAttributes::UpdateInstanceBufferData(const std::vector<glm::mat4>& data)
    {
        m_InstanceBuffer.UpdateBufferData(data);
    }
}