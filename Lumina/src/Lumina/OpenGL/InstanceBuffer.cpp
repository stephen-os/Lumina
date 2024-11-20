#include "InstanceBuffer.h"
#include <iostream>

namespace GL
{
    InstanceBuffer::InstanceBuffer()
        : m_BufferID(0), m_InstanceCount(0)
    {
        glGenBuffers(1, &m_BufferID);
    }

    InstanceBuffer::~InstanceBuffer()
    {
        Destroy();
    }

    void InstanceBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
    }

    void InstanceBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void InstanceBuffer::UpdateBufferData(const std::vector<glm::mat4>& data, GLenum usage)
    {
        Bind();
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::mat4), data.data(), usage);
        m_InstanceCount = static_cast<GLsizei>(data.size());
        Unbind();
    }

    void InstanceBuffer::SetInstanceAttribute(GLuint location, GLint size, GLenum type, GLsizei stride, GLuint divisor, GLsizei offset)
    {
        Bind();
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, size, type, GL_FALSE, stride, reinterpret_cast<const void*>(offset));
        glVertexAttribDivisor(location, divisor);
        Unbind();
    }

    void InstanceBuffer::Destroy()
    {
        if (m_BufferID != 0)
        {
            glDeleteBuffers(1, &m_BufferID);
            m_BufferID = 0;
        }
    }
}