#include "DepthBuffer.h"
#include "GLUtils.h"
#include <iostream>

namespace GL
{
    DepthBuffer::DepthBuffer() : m_Width(0), m_Height(0), m_DepthBufferID(0) 
    {
        GLCALL(glGenRenderbuffers(1, &m_DepthBufferID));
        GLCALL(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBufferID));
        GLCALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_Width, m_Height));
    }

    DepthBuffer::~DepthBuffer()
    {
        Destroy();
    }

    void DepthBuffer::Destroy()
    {
        if (m_DepthBufferID != 0)
        {
            GLDESTROY("DepthBuffer", m_DepthBufferID);
            GLCALL(glDeleteRenderbuffers(1, &m_DepthBufferID));
            m_DepthBufferID = 0; 
        }
    }

    bool DepthBuffer::SetData(int width, int height)
    {
        if (m_Width == width && m_Height == height)
        {
            return false;
        }

        m_Width = width;
        m_Height = height;
        
        Bind(); 
        GLCALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_Width, m_Height));
        Unbind();

        return true; 
    }

    void DepthBuffer::Bind() const
    {
        GLCALL(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBufferID));
    }

    void DepthBuffer::Unbind() const
    {
        GLCALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    }
}