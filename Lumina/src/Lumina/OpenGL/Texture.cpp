#include "Texture.h"
#include "GLUtils.h"

#include <iostream>

namespace GL
{
    Texture::Texture()
    {
        GLCALL(glGenTextures(1, &m_TextureID));
        GLCALL(glBindTexture(GL_TEXTURE_2D, m_TextureID));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
    }

    Texture::~Texture()
    {
        GLCALL(glDeleteTextures(1, &m_TextureID));
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

    void Texture::SetData(float width, float height)
    {
        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)width, (GLsizei)height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
    }
}