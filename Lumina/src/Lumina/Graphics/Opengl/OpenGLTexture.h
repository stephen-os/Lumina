#pragma once

#include "../Texture.h"

#include <glad/glad.h>

#include <string>

namespace Lumina
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(std::string& source);
		OpenGLTexture(uint32_t width, uint32_t height); 
		~OpenGLTexture();

		void Bind(uint32_t slot = 0) const override;
		void Unbind() const override;

		bool SetResolution(int width, int height) override;

		void SetData(void* data, uint32_t size); 

		uint32_t GetID() const override { return m_BufferID; }
		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }
		std::string GetPath() const override { return m_Path; }
	private:
		std::string m_Path; 
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		uint32_t m_BufferID = 0;
	};
}