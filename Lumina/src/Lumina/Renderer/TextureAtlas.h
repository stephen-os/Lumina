#pragma once

#include "../Core/Aliases.h"

#include "Texture.h"

#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace Lumina
{
	class TextureAtlas
	{
	public:
		TextureAtlas(int width, int height); 
		TextureAtlas(std::string& source, int width, int height);
		~TextureAtlas() = default;

		void Resize(int width, int height);
		void SetTexture(std::string& source); 

		int GetWidth() const { return m_GridWidth; }
		int GetHeight() const { return m_GridHeight; }

		Shared<Texture>& GetTexture() { return m_Texture; }
		uint32_t GetTextureID() { return m_Texture->GetID(); }

		glm::vec4 GetTextureCoords(int index) const;
		glm::vec2 GetOffset(int index) const;
		glm::vec2 GetPosition(int index) const; 
	private:
		Shared<Texture> m_Texture;
		int m_GridWidth;					// Number of textures along the width
		int m_GridHeight;					// Number of textures along the height
		float m_TexWidth;					// Width of a single texture in UV space
		float m_TexHeight;					// Height of a single texture in UV space
		std::vector<glm::vec4> m_TexCoords; // Precomputed texture coordinates
	};
}