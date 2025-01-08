#pragma once

#include "Texture.h"

#include <vector>
#include <string>

#include <glm/glm.hpp>

namespace Lumina
{ 
    class TextureAtlas 
    {
    public:
        TextureAtlas();
        ~TextureAtlas() = default;

        void CreateAtlas(const std::string& path, int width, int height);

        void Bind() const;
        void Unbind() const;

        int GetGridWidth() const { return m_GridWidth; }
        int GetGridHeight() const { return m_GridHeight; }

        unsigned int GetTextureID() { return m_Texture.GetID(); }
        glm::vec4 GetTexCoords(int index) const;
        glm::vec2 GetOffset(int index) const;
    private:
        Texture m_Texture;
        int m_GridWidth;                    // Number of textures along the width
        int m_GridHeight;                   // Number of textures along the height
        float m_TexWidth;                   // Width of a single texture in UV space
        float m_TexHeight;                  // Height of a single texture in UV space
        std::vector<glm::vec4> m_TexCoords; // Precomputed texture coordinates
    };
}