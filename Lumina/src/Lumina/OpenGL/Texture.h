#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

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

		bool SetData(int width, int height);

		unsigned int GetID() { return m_TextureID; };
	private:
		int m_Width;
		int m_Height; 
		unsigned int m_TextureID;
	};
}

#endif 