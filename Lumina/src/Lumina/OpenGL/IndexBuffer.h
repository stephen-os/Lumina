#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <glad/glad.h>

namespace GL
{
	class IndexBuffer
	{
	public:
		IndexBuffer(); 
		IndexBuffer(const unsigned int* indices, unsigned int count, GLenum usage = GL_STATIC_DRAW);

		IndexBuffer(const IndexBuffer& other);
		IndexBuffer(IndexBuffer&& other) noexcept;
		IndexBuffer& operator=(const IndexBuffer& other);

		~IndexBuffer() = default;

		void Destroy();
		void Bind();
		void Unbind();

		unsigned int GetIndexCount() const { return m_Count; };
		unsigned int GetID() const { return m_IndexBufferID; }; 
	
		void SetData(const unsigned int* indices, unsigned int count, GLenum usage = GL_STATIC_DRAW);
	private:
		int m_Count; 
		unsigned int m_IndexBufferID;
	};
}

#endif 