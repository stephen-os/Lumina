// FrameBuffer.cpp

#include "FrameBuffer.h"
#include "GLUtils.h"

#include <iostream>

namespace GL
{
    FrameBuffer::FrameBuffer()
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
        GLCALL(glDeleteFramebuffers(1, &m_FrameBufferID));
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
}
