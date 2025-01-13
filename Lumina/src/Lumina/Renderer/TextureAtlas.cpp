#include "TextureAtlas.h"
#include <iostream>

namespace Lumina
{
    TextureAtlas::TextureAtlas()
        : m_GridWidth(1), m_GridHeight(1), m_TexWidth(0.0f), m_TexHeight(0.0f) {}

    void TextureAtlas::CreateAtlas(const std::string& path, int width, int height)
    { 
        m_GridWidth = width;
        m_GridHeight = height;

        if (!m_Texture.SetData(path)) 
        {
            std::cerr << "[Texture Atlas] Failed to load texture atlas: " << path << std::endl;
            return;
        }

        int texturePixelWidth = m_Texture.GetWidth();
        int texturePixelHeight = m_Texture.GetHeight();

        m_TexWidth = 1.0f / static_cast<float>(m_GridWidth);
        m_TexHeight = 1.0f / static_cast<float>(m_GridHeight);

        m_TexCoords.clear();

        m_TexCoords.reserve(m_GridWidth * m_GridHeight);
        for (int y = 0; y < m_GridHeight; ++y) 
        {
            for (int x = 0; x < m_GridWidth; ++x) 
            {
                float uMin = x * m_TexWidth;
                float vMin = y * m_TexHeight;
                float uMax = uMin + m_TexWidth;
                float vMax = vMin + m_TexHeight;

                m_TexCoords.emplace_back(uMin, vMin, uMax, vMax);
            }
        }
    }

    void TextureAtlas::Bind() const 
    {
        m_Texture.Bind();
    }

    void TextureAtlas::Unbind() const 
    {
        m_Texture.Unbind();
    }

    glm::vec4 TextureAtlas::GetTexCoords(int index) const 
    {
        if (index < 0 || index >= static_cast<int>(m_TexCoords.size())) 
        {
            std::cerr << "[Texture Atlas] Invalid texture index: " << index << std::endl;
            return glm::vec4(0.0f);
        }
        return m_TexCoords[index];
    }

    glm::vec2 TextureAtlas::GetOffset(int index) const 
    {
        int row = index / m_GridWidth;
        int col = index % m_GridWidth;

        float xOffset = col * m_TexWidth;
        float yOffset = row * m_TexHeight;

        return glm::vec2(xOffset, yOffset);
    }
}