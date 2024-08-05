#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <glad/glad.h>

namespace GL
{
	class IndexBuffer
	{
	public:
		IndexBuffer(const unsigned int* indices, unsigned int count, GLenum usage = GL_STATIC_DRAW);
		~IndexBuffer();

		void Bind();
		void Unbind();

		unsigned int GetIndexBufferID() const { return m_IndexBufferID; }; 
		unsigned int GetIndexCount() const { return m_Count; };
	private:
		unsigned int m_IndexBufferID;
		unsigned int m_Count; 
	};
}

#endif 
