#include "InstanceBuffer.h"

#include <iostream>

namespace GL
{
    InstanceBuffer::InstanceBuffer()
        : m_Name("unamed"), m_Location(0), m_InstanceBufferID(0), m_InstanceCount(0)
    {
        glGenBuffers(1, &m_InstanceBufferID);
    }

    InstanceBuffer::~InstanceBuffer()
    {
        Destroy();
    }

    void InstanceBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_InstanceBufferID);
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
        m_Location = location; 

        Bind();
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, size, type, GL_FALSE, stride, reinterpret_cast<const void*>(offset));
        glVertexAttribDivisor(location, divisor);
        Unbind();
    }

    void InstanceBuffer::Destroy()
    {
        if (m_InstanceBufferID != 0)
        {
            glDeleteBuffers(1, &m_InstanceBufferID);
            m_InstanceBufferID = 0;
        }
    }
}