#include "Texture.h"
#include "RendererDebug.h"

#include <stb_image.h>
#include <iostream>

namespace Lumina
{
    Texture::Texture()
    {
        GLCALL(glGenTextures(1, &m_BufferID));
    }

    Texture::~Texture()
    {
        GLCALL(glDeleteTextures(1, &m_BufferID));
    }

    void Texture::Bind(uint32_t slot) const
    {
        GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
        GLCALL(glBindTexture(GL_TEXTURE_2D, m_BufferID));
    }

    void Texture::Unbind() const
    {
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
    }

    bool Texture::SetData(std::string source)
    {
        Bind(m_Slot);

        int channels;
        int width, height;
        unsigned char* data = stbi_load(source.c_str(), &width, &height, &channels, 0);
        if (!data)
        {
            std::cerr << "Failed to load texture: " << source << std::endl;
            return false;
        }

        m_Width = width;
        m_Height = height;

        GLenum format = (channels == 4) ? GL_RGBA : (channels == 3) ? GL_RGB : GL_RED;

        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data));
        GLCALL(glGenerateMipmap(GL_TEXTURE_2D));

        stbi_image_free(data);

        Unbind();
        return true;
    }

    bool Texture::SetResolution(int width, int height)
    {
        if (width <= 0 || height <= 0)
        {
            std::cerr << "Invalid resolution: " << width << "x" << height << std::endl;
            return false;
        }

        m_Width = width;
        m_Height = height;

        Bind(m_Slot);

        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        Unbind();
        return true;
    }
}
