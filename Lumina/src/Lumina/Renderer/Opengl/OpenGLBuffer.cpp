#include "OpenGLBuffer.h"

#include "RendererDebug.h"
#include "../../Core/Assert.h"

namespace Lumina
{
    // Vertex Buffer

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
    {
        GLCALL(glCreateBuffers(1, &m_BufferID));
        LUMINA_ASSERT(m_BufferID != 0, "Failed to create vertex buffer!");

        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
        GLCALL(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
    {
        GLCALL(glCreateBuffers(1, &m_BufferID));
        LUMINA_ASSERT(m_BufferID != 0, "Failed to create vertex buffer!");

        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
        GLCALL(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        GLCALL(glDeleteBuffers(1, &m_BufferID));
    }

    void OpenGLVertexBuffer::Bind() const
    {
        LUMINA_ASSERT(m_BufferID != 0, "Trying to bind an invalid vertex buffer!");
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
    {
        LUMINA_ASSERT(data != nullptr, "VertexBuffer::SetData called with null data!");
        LUMINA_ASSERT(size > 0, "VertexBuffer::SetData called with zero size!");

        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
        GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
    }

    // Index Buffer

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count)
    {
        LUMINA_ASSERT(indices != nullptr, "Null index data passed to IndexBuffer!");
        LUMINA_ASSERT(count > 0, "Index buffer count is zero!");

        GLCALL(glCreateBuffers(1, &m_BufferID));
        LUMINA_ASSERT(m_BufferID != 0, "Failed to create index buffer!");

        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
        GLCALL(glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        GLCALL(glDeleteBuffers(1, &m_BufferID));
    }

    void OpenGLIndexBuffer::Bind() const
    {
        LUMINA_ASSERT(m_BufferID != 0, "Trying to bind an invalid index buffer!");
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID));
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
}