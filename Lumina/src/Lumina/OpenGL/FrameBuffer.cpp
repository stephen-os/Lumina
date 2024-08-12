#include "FrameBuffer.h"
#include "GLUtils.h"

#include <iostream>

namespace GL
{
    FrameBuffer::FrameBuffer()
        : m_DepthBufferID(0)
    {
        GLCALL(glGenFramebuffers(1, &m_FrameBufferID));
        GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID));

        // Unbind to ensure no accidental changes
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
        GLCALL(glDeleteFramebuffers(1, &m_FrameBufferID));
        if (m_DepthBufferID)
        {
            GLCALL(glDeleteRenderbuffers(1, &m_DepthBufferID));
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

    void FrameBuffer::AttachTexture(unsigned int textureID)
    {
        GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0));
    }

    void FrameBuffer::AttachDepthBuffer(float width, float height)
    {
        Bind(); 
        GLCALL(glGenRenderbuffers(1, &m_DepthBufferID));
        GLCALL(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBufferID));
        GLCALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height)); // Adjust the size as needed
        GLCALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBufferID));
        Unbind(); 
    }
}
