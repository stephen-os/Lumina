#include "OpenGLFrameBuffer.h"
#include "RendererDebug.h"

#include <glad/glad.h>
#include <iostream>

namespace Lumina
{
    OpenGLFrameBuffer::OpenGLFrameBuffer()
    {
        m_Width = 720;
        m_Height = 1080;

        GLCALL(glGenFramebuffers(1, &m_BufferID));
        Bind(); // Bind to set up the OpenGLFrameBuffer

        // Create color attachment (Texture)
        GLCALL(glGenTextures(1, &m_ColorAttachment));
        GLCALL(glBindTexture(GL_TEXTURE_2D, m_ColorAttachment));
        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCALL(glGenerateMipmap(GL_TEXTURE_2D));

        GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0));

        // Create depth attachment (optional)
        GLCALL(glGenRenderbuffers(1, &m_DepthAttachment));
        GLCALL(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment));
        GLCALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height));
        GLCALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment));

        // Check if OpenGLFrameBuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "[Error] OpenGLFrameBuffer is not complete!" << std::endl;
        }

        Unbind(); // Unbind the OpenGLFrameBuffer
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        GLCALL(glDeleteFramebuffers(1, &m_BufferID));
        GLCALL(glDeleteTextures(1, &m_ColorAttachment));
        if (m_DepthAttachment) {
            GLCALL(glDeleteRenderbuffers(1, &m_DepthAttachment));
        }
    }

    void OpenGLFrameBuffer::Bind() const
    {
        GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_BufferID));
    }

    void OpenGLFrameBuffer::Unbind() const
    {
        GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0)); // Bind default OpenGLFrameBuffer (screen)
    }

    void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
    {
        m_Width = width;
        m_Height = height;

        Bind();

        // Resize the color attachment (texture)
        GLCALL(glBindTexture(GL_TEXTURE_2D, m_ColorAttachment));
        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
        GLCALL(glGenerateMipmap(GL_TEXTURE_2D));

        // Resize the depth attachment (renderbuffer)
        GLCALL(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment));
        GLCALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height));
    }

    void OpenGLFrameBuffer::ReadPixels(int x, int y, uint32_t width, uint32_t height, void* data) const
    {
        Bind(); 
        GLCALL(glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data));
        Unbind(); 
    }
}