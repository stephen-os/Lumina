#include "Buffer.h"

#include "RendererDebug.h"

namespace Lumina
{
    // Vertex Buffer

    VertexBuffer::VertexBuffer(uint32_t size)
    {
        GLCALL(glCreateBuffers(1, &m_BufferID));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
        GLCALL(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    }

    VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
    {
        GLCALL(glCreateBuffers(1, &m_BufferID));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
        GLCALL(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
    }

    VertexBuffer::~VertexBuffer()
    {
        GLCALL(glDeleteBuffers(1, &m_BufferID));
    }

    void VertexBuffer::Bind() const
    {
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
    }

    void VertexBuffer::Unbind() const
    {
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    void VertexBuffer::SetData(const void* data, uint32_t size)
    {
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
        GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
    }

    // Index Buffer

    IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count)
    {
        GLCALL(glCreateBuffers(1, &m_BufferID));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
        GLCALL(glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
    }

    IndexBuffer::~IndexBuffer()
    {
        GLCALL(glDeleteBuffers(1, &m_BufferID));
    }

    void IndexBuffer::Bind() const
    {
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID));
    }

    void IndexBuffer::Unbind() const
    {
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
}