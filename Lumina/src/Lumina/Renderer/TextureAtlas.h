#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include "../OpenGL/Texture.h"

#include <vector>
#include <string>

#include <glm/glm.hpp>

class TextureAtlas {
public:
    TextureAtlas();
    ~TextureAtlas();

    void Destroy(); 

    bool SetData(const std::string& filepath, int gridWidth, int gridHeight);

    void Bind() const;
    void Unbind() const;

    int GetGridWidth() const { return m_GridWidth; }
	int GetGridHeight() const { return m_GridHeight; }

    unsigned int GetTextureID() { return m_Texture.GetID(); }
    glm::vec4 GetTexCoords(int index) const;
    glm::vec2 GetOffset(int index) const;
private:
    GL::Texture m_Texture;
    int m_GridWidth;                    // Number of textures along the width
    int m_GridHeight;                   // Number of textures along the height
    float m_TexWidth;                   // Width of a single texture in UV space
    float m_TexHeight;                  // Height of a single texture in UV space
    std::vector<glm::vec4> m_TexCoords; // Precomputed texture coordinates
};

#endif // TEXTURE_ATLAS_H
