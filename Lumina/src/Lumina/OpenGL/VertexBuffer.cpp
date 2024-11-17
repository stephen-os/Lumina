#include "VertexBuffer.h"
#include "GLUtils.h"

#include <iostream>

namespace GL
{
    VertexBuffer::VertexBuffer() : m_Name("Unnamed"), m_Location(-1), m_Count(0), m_Stride(0), m_VertexBufferID(0) {}

    VertexBuffer::VertexBuffer(const std::string& name, int location, const float* data, int count, int stride, GLenum usage)
        : m_Name(name), m_Location(location), m_Count(count), m_Stride(stride)
    {
        GLCALL(glGenBuffers(1, &m_VertexBufferID));
        Bind();
        GLCALL(glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, usage));
        Unbind();
    }

    VertexBuffer::VertexBuffer(const VertexBuffer& other)
        : m_Name(other.m_Name), m_Location(other.m_Location), m_Count(other.m_Count),
        m_Stride(other.m_Stride), m_VertexBufferID(other.m_VertexBufferID) {}

    VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept 
        : m_Name(std::move(other.m_Name)), m_Location(other.m_Location), m_Count(other.m_Count), 
        m_Stride(other.m_Stride), m_VertexBufferID(other.m_VertexBufferID)
    {
        other.m_Location = 0;
        other.m_Count = 0;
        other.m_Stride = 0;
        other.m_VertexBufferID = 0;
    }

    VertexBuffer& VertexBuffer::operator=(const VertexBuffer& other)
    {
        if (this != &other)
        {
            m_Name = other.m_Name;
            m_Location = other.m_Location;
            m_Count = other.m_Count;
            m_Stride = other.m_Stride;
            m_VertexBufferID = other.m_VertexBufferID; 
        }
        return *this;
    }

    void VertexBuffer::Destroy()
    {
        if (m_VertexBufferID != 0)
        {
            GLDESTROY("VertexBuffer", m_VertexBufferID);
            GLCALL(glDeleteBuffers(1, &m_VertexBufferID));
            m_VertexBufferID = 0;
        }
    }
    
    void VertexBuffer::Bind() const
    {
        if (m_VertexBufferID != 0)
        {
            GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));
        }
    }

    void VertexBuffer::Unbind() const
    {
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    void VertexBuffer::SetData(int location, const float* data, int count, int stride, GLenum usage)
    {
        if (m_VertexBufferID == 0)
        {
            GLCALL(glGenBuffers(1, &m_VertexBufferID));
        }

        m_Location = location; 
        m_Count = count;
        m_Stride = stride; 
        Bind();
        GLCALL(glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, usage));
        Unbind();
    }
}