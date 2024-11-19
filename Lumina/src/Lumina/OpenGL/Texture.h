#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

#include <string>

namespace GL
{
	class Texture
	{
	public:
		Texture();
		~Texture();

		void Destroy(); 
		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		bool SetData(std::string source);
		void SetSlot(int slot); 
		bool SetResolution(int width, int height); 

		unsigned int GetID() { return m_TextureID; };
	private:
		int m_Slot;
		int m_Width;
		int m_Height; 
		unsigned int m_TextureID;
	};
}

#endif 