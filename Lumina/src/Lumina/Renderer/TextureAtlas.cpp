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
    if (!m_Texture.SetData(filepath)) {
        std::cerr << "[Texture Atlas] Failed to load texture atlas: " << filepath << std::endl;
        return false;
    }

    // Load texture dimensions in pixels
    int texturePixelWidth = m_Texture.GetWidth();  // Assuming m_Texture has a GetWidth() method
    int texturePixelHeight = m_Texture.GetHeight(); // Assuming m_Texture has a GetHeight() method

    m_GridWidth = gridWidth;
    m_GridHeight = gridHeight;

    // Calculate the width and height of each cell in pixels
    int cellWidth = texturePixelWidth / m_GridWidth;
    int cellHeight = texturePixelHeight / m_GridHeight;

    m_TexWidth = static_cast<float>(cellWidth) / static_cast<float>(texturePixelWidth);
    m_TexHeight = static_cast<float>(cellHeight) / static_cast<float>(texturePixelHeight);

    m_TexCoords.clear();
    for (int y = 0; y < m_GridHeight; y++) {
        for (int x = 0; x < m_GridWidth; x++) {
            // Calculate pixel boundaries
            int uMinPixel = x * cellWidth;
            int vMinPixel = y * cellHeight;
            int uMaxPixel = uMinPixel + cellWidth;
            int vMaxPixel = vMinPixel + cellHeight;

            // Normalize to [0, 1] texture coordinates
            float uMin = static_cast<float>(uMinPixel) / static_cast<float>(texturePixelWidth);
            float vMin = static_cast<float>(vMinPixel) / static_cast<float>(texturePixelHeight);
            float uMax = static_cast<float>(uMaxPixel) / static_cast<float>(texturePixelWidth);
            float vMax = static_cast<float>(vMaxPixel) / static_cast<float>(texturePixelHeight);

            // Add normalized coordinates to the vector
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