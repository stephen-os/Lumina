#include "Texture.h"
#include "GLUtils.h"

#include <iostream>

namespace GL
{
    Texture::Texture() : m_Width(0), m_Height(0), m_TextureID(0)
    {
        GLCALL(glGenTextures(1, &m_TextureID));
        GLCALL(glBindTexture(GL_TEXTURE_2D, m_TextureID));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
    }

    Texture::~Texture()
    {
        Destroy(); 
    }

    void Texture::Destroy()
    {
        if (m_TextureID != 0)
        {
            GLCALL(glDeleteTextures(1, &m_TextureID));
            m_TextureID = 0;
        }
    }

    void Texture::Bind(unsigned int slot) const
    {
        GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
        GLCALL(glBindTexture(GL_TEXTURE_2D, m_TextureID));
    }

    void Texture::Unbind() const
    {
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
    }

    bool Texture::SetData(int width, int height)
    {
        if (m_Width == width && m_Height == height)
        {
            return false;
        }

        m_Width = width;
        m_Height = height;

        Bind();
        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
        Unbind();

        return true; 
    }
}