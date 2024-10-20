#include "IndexBuffer.h"
#include "GLUtils.h"

#include <glad/glad.h>

#include "iostream"

namespace GL
{
	IndexBuffer::IndexBuffer(const unsigned int* indices, unsigned int count, GLenum usage) : m_Count(count)
	{
		if (m_IndexBufferID == 0)
		{
			GLCALL(glGenBuffers(1, &m_IndexBufferID));
			GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID));
			GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, usage));
			GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}
	}

	void IndexBuffer::Destroy()
	{
		if (m_IndexBufferID != 0)
		{
			GLDESTROY("IndexBuffer", m_IndexBufferID);
			GLCALL(glDeleteBuffers(1, &m_IndexBufferID));
			m_IndexBufferID = 0;
		}
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