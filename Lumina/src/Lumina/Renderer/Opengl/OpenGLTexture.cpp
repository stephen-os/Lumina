#include "OpenGLTexture.h"
#include "RendererDebug.h"
#include <stb_image.h>
#include <iostream>

namespace Lumina
{
    OpenGLTexture::OpenGLTexture(std::string& source)
    {
        // Create texture using DSA
        GLCALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_BufferID));

        int channels;
        int width, height;
        unsigned char* data = stbi_load(source.c_str(), &width, &height, &channels, 0);
        if (!data)
        {
            std::cerr << "Failed to load OpenGLTexture: " << source << std::endl;
        }

        m_Width = width;
        m_Height = height;
        m_Path = source;

        GLenum format = (channels == 4) ? GL_RGBA : (channels == 3) ? GL_RGB : GL_RED;
        GLenum internalFormat = GL_RGBA8;  // Could be more specific like GL_RGBA8 if needed

        // Set texture parameters using DSA
        GLCALL(glTextureParameteri(m_BufferID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCALL(glTextureParameteri(m_BufferID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCALL(glTextureParameteri(m_BufferID, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCALL(glTextureParameteri(m_BufferID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

        // Allocate and upload texture data using DSA
        GLCALL(glTextureStorage2D(m_BufferID, 1, internalFormat, m_Width, m_Height));
        GLCALL(glTextureSubImage2D(m_BufferID, 0, 0, 0, m_Width, m_Height, format, GL_UNSIGNED_BYTE, data));

        // Generate mipmaps using DSA
        GLCALL(glGenerateTextureMipmap(m_BufferID));

        stbi_image_free(data);
    }

    OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height)
    {
        // Create texture using DSA
        GLCALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_BufferID));

        m_Width = width;
        m_Height = height;

        // Allocate texture storage using DSA
        GLCALL(glTextureStorage2D(m_BufferID, 1, GL_RGBA8, width, height));

        // Set texture parameters using DSA
        GLCALL(glTextureParameteri(m_BufferID, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCALL(glTextureParameteri(m_BufferID, GL_TEXTURE_WRAP_T, GL_REPEAT));
        GLCALL(glTextureParameteri(m_BufferID, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCALL(glTextureParameteri(m_BufferID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    }

    OpenGLTexture::~OpenGLTexture()
    {
        GLCALL(glDeleteTextures(1, &m_BufferID));
    }

    void OpenGLTexture::Bind(uint32_t slot) const
    {
        // Bind texture to texture unit using DSA
        GLCALL(glBindTextureUnit(slot, m_BufferID));
        // m_Slot = slot;
    }

    void OpenGLTexture::Unbind() const
    {
        // In OpenGL 4.5+ with DSA, explicit unbinding is less necessary, but can still be done
        // GLCALL(glBindTextureUnit(m_Slot, 0));
    }

    bool OpenGLTexture::SetResolution(int width, int height)
    {
        if (width <= 0 || height <= 0)
        {
            std::cerr << "Invalid resolution: " << width << "x" << height << std::endl;
            return false;
        }

        m_Width = width;
        m_Height = height;

        // Reallocate texture storage using DSA
        GLCALL(glDeleteTextures(1, &m_BufferID));
        GLCALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_BufferID));
        GLCALL(glTextureStorage2D(m_BufferID, 1, GL_RGBA8, m_Width, m_Height));

        // Set texture parameters using DSA
        GLCALL(glTextureParameteri(m_BufferID, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCALL(glTextureParameteri(m_BufferID, GL_TEXTURE_WRAP_T, GL_REPEAT));
        GLCALL(glTextureParameteri(m_BufferID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCALL(glTextureParameteri(m_BufferID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        return true;
    }

    void OpenGLTexture::SetData(void* data, uint32_t size)
    {
        uint32_t expectedSize = m_Width * m_Height * 4; // Assuming 4 channels (RGBA)
        if (size != expectedSize)
        {
            std::cerr << "OpenGLTexture::SetData - Data size mismatch. Expected: "
                << expectedSize << ", got: " << size << std::endl;
            return;
        }

        // Update texture data using DSA
        GLCALL(glTextureSubImage2D(m_BufferID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data));
    }
}