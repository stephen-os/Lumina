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

		bool HasTexture() const { return m_HasTexture; }
		void RemoveTexture();

		Shared<Texture>& GetTexture() { return m_Texture; }
		glm::vec4 GetTextureCoords(int index) const;
		glm::vec2 GetOffset(int index) const;
		glm::vec2 GetPosition(int index) const; 
	private:
		Shared<Texture> m_Texture = nullptr;	// Shared pointer to Texture
		int m_GridWidth = 1;					// Number of textures along the width
		int m_GridHeight = 1;					// Number of textures along the height
		float m_TexWidth = 1.0f;				// Width of a single texture in UV space
		float m_TexHeight = 1.0f;				// Height of a single texture in UV space
		bool m_HasTexture = false;				// Flag to keep track when atlas has texture or not
		std::vector<glm::vec4> m_TexCoords;		// Precomputed texture coordinates
	};
}