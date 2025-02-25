#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

#include <string>

namespace Lumina
{
	class Texture
	{
	public:
		Texture(); 
		~Texture();

		void Reset();

		void Bind(uint32_t slot = 0) const;
		void Unbind() const;

		bool SetData(std::string source);
		bool SetResolution(int width, int height); 

		uint32_t GetID() { return m_BufferID; };
		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
	private:
		int m_Slot;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_BufferID;
	};
}

#endif 