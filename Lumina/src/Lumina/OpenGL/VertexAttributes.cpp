#include "VertexAttributes.h"

namespace GL
{
    VertexAttributes::VertexAttributes(const VertexAttributes& other)
        : m_IndexBuffer(other.m_IndexBuffer)
    {
        for (const auto& attribute : other.m_Attributes)
        {
            m_Attributes.emplace_back(attribute.first, attribute.second);
        }
    }

    VertexAttributes::VertexAttributes(VertexAttributes&& other) noexcept
        : m_IndexBuffer(std::move(other.m_IndexBuffer)),
        m_Attributes(std::move(other.m_Attributes))
    {
    }

    VertexAttributes& VertexAttributes::operator=(const VertexAttributes& other)
    {
        if (this != &other)
        {
            Destroy();
            m_IndexBuffer = other.m_IndexBuffer;
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

    void VertexAttributes::AddBuffer(const std::string& name, int location, const float* data, int size, int count, int stride, bool isInstance, GLenum usage)
    {
        Buffer buffer;
        buffer.Init();
        buffer.SetName(name);
        buffer.SetData(GL_ARRAY_BUFFER, data, size, count, stride, isInstance, usage);
        m_Attributes.emplace_back(location, std::move(buffer));

        // Enable the vertex attribute array
        // glEnableVertexAttribArray(location);
        // glVertexAttribPointer(location, stride / sizeof(float), GL_FLOAT, GL_FALSE, stride, nullptr);
    }

    void VertexAttributes::UpdateBuffer(int location, const void* data, int count)
    {
        for (auto& attribute : m_Attributes)
        {
            if (attribute.first == static_cast<GLuint>(location))
            {
                Buffer& buffer = attribute.second;
                buffer.Bind();
                buffer.UpdateBuffer(data, count);
                buffer.Unbind();
                return;
            }
        }

        std::cerr << "UpdateBuffer: Buffer with location " << location << " not found.\n";
    }

    GLuint VertexAttributes::GetBufferID(int location)
    {
        for (const auto& attribute : m_Attributes)
        {
            if (attribute.first == static_cast<GLuint>(location))
            {
                return attribute.second.GetID();
            }
        }

        std::cerr << "GetBufferID: Buffer with location " << location << " not found.\n";
        return 0; // Return 0 if the buffer is not found
    }

    void VertexAttributes::AddIndices(const unsigned int* data, const int count, GLenum usage)
    {
        m_IndexBuffer.Init();
        m_IndexBuffer.SetData(GL_ELEMENT_ARRAY_BUFFER, data, sizeof(unsigned int), count, sizeof(unsigned int), usage);
    }
}