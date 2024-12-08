#include "TextureAtlas.h"
#include <iostream>

TextureAtlas::TextureAtlas()
    : m_GridWidth(0), m_GridHeight(0), m_TexWidth(0.0f), m_TexHeight(0.0f) {}

TextureAtlas::~TextureAtlas() {}

void TextureAtlas::Destroy()
{
    m_Texture.Destroy(); 
}

bool TextureAtlas::SetData(const std::string& filepath, int gridWidth, int gridHeight) {
    if (!m_Texture.SetData(filepath)) 
    {
        std::cerr << "[Texture Atlas] Failed to load texture atlas: " << filepath << std::endl;
        return false;
    }

    m_GridWidth = gridWidth;
    m_GridHeight = gridHeight;

    m_TexWidth = 1.0f / static_cast<float>(m_GridWidth);
    m_TexHeight = 1.0f / static_cast<float>(m_GridHeight);

    m_TexCoords.clear();
    for (int y = 0; y < m_GridHeight; y++) 
    {
        for (int x = 0; x < m_GridWidth; x++) 
        {
            float uMin = x * m_TexWidth;
            float vMin = y * m_TexHeight;
            float uMax = uMin + m_TexWidth;
            float vMax = vMin + m_TexHeight;
            m_TexCoords.emplace_back(uMin, vMin, uMax, vMax);
        }
    }

    return true;
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