#include "FrameBuffer.h"

#include <iostream>

namespace GL
{
    FrameBuffer::FrameBuffer()
    {
        glGenFramebuffers(1, &m_FrameBufferID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Error: Framebuffer is not complete!" << std::endl;
        }
    }

    void FrameBuffer::Destroy()
    {
        glDeleteFramebuffers(1, &m_FrameBufferID);
    }

    void FrameBuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
    }

    void FrameBuffer::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::AttachTexture(unsigned int textureID)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
    }
}
