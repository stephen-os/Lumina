#include "OpenGLBuffer.h"

#include "RendererDebug.h"

namespace Lumina
{
    // Vertex Buffer

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
    {
        GLCALL(glCreateBuffers(1, &m_BufferID));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
        GLCALL(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
    {
        GLCALL(glCreateBuffers(1, &m_BufferID));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
        GLCALL(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        GLCALL(glDeleteBuffers(1, &m_BufferID));
    }

    void OpenGLVertexBuffer::Bind() const
    {
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
    {
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
        GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
    }

    // Index Buffer

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count)
    {
        GLCALL(glCreateBuffers(1, &m_BufferID));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
        GLCALL(glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        GLCALL(glDeleteBuffers(1, &m_BufferID));
    }

    void OpenGLIndexBuffer::Bind() const
    {
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID));
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
}