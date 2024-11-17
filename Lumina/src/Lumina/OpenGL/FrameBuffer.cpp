#include "FrameBuffer.h"
#include "GLUtils.h"

#include <iostream>

namespace GL
{
    FrameBuffer::FrameBuffer() : m_FrameBufferID(0)
    {
        GLCALL(glGenFramebuffers(1, &m_FrameBufferID));
        GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID));

        Unbind();

        GLenum status;
        GLCALL(status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "Error: Framebuffer is not complete!!!" << std::endl;
        }

        GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    FrameBuffer::~FrameBuffer() 
    {
        Destroy(); 
    }

    void FrameBuffer::Destroy()
    {
        if (m_FrameBufferID != 0)
        {
            GLCALL(glDeleteFramebuffers(1, &m_FrameBufferID));
            m_FrameBufferID = 0; 
        }
    }

    void FrameBuffer::Bind() const
    {
        GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID));
    }

    void FrameBuffer::Unbind() const
    {
        GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void FrameBuffer::AttachTexture(unsigned int id)
    {
        Bind();
        GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0));
        Unbind(); 
    }

    void FrameBuffer::AttachDepthBuffer(unsigned int id)
    {
        Bind(); 
        GLCALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, id));
        Unbind(); 
    }
}
