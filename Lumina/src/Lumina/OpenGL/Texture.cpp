#include "Texture.h"
#include <stb_image.h>
#include <iostream>

namespace GL {

    Texture::Texture()
        : m_Slot(0), m_Width(0), m_Height(0), m_TextureID(0)
    {
        glGenTextures(1, &m_TextureID);
    }

    Texture::~Texture()
    {
        Destroy();
    }

    void Texture::Destroy()
    {
        if (m_TextureID != 0)
        {
            glDeleteTextures(1, &m_TextureID);
            m_TextureID = 0;
        }
    }

    void Texture::Bind(unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
    }

    void Texture::Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    bool Texture::SetData(std::string source)
    {
        Bind(m_Slot);

        int channels;
        unsigned char* data = stbi_load(source.c_str(), &m_Width, &m_Height, &channels, 0);
        if (!data)
        {
            std::cerr << "Failed to load texture: " << source << std::endl;
            return false;
        }

        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Unbind();
        return true;
    }

    void Texture::SetSlot(int slot)
    {
        m_Slot = slot;
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

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Unbind();
        return true;
    }

}