#include "IndexBuffer.h"
#include "GLUtils.h"

#include <glad/glad.h>

namespace GL
{
	IndexBuffer::IndexBuffer(const unsigned int* indices, unsigned int count, GLenum usage) : m_Count(count)
	{
		GLCALL(glGenBuffers(1, &m_IndexBufferID));
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID));
		GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, usage));
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	IndexBuffer::~IndexBuffer()
	{
		GLCALL(glDeleteBuffers(1, &m_IndexBufferID));
	}

	void IndexBuffer::Bind()
	{
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID));
	}

	void IndexBuffer::Unbind()
	{
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}