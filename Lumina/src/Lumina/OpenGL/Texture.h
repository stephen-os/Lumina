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

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;
		void SetData(float width, float height);

		unsigned int GetID() { return m_TextureID; };
	private:
		unsigned int m_TextureID;
	};
}

#endif 