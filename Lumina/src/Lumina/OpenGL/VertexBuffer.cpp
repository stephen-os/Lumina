#include "VertexBuffer.h"

#include "GLUtils.h"

#include <iostream>

namespace GL
{
	VertexBuffer::VertexBuffer(const std::string& name, int count, int stride, const float* floats, GLenum usage) :
		m_Name(name),
		m_Count(count),
		m_Stride(stride)
	{
		GLCALL(glGenBuffers(1, &m_VertexBufferID));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));
		GLCALL(glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), floats, usage));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void VertexBuffer::Destroy()
	{
		if (m_VertexBufferID != 0)
		{
			GLDESTROY("VertexBuffer", m_VertexBufferID);
			GLCALL(glDeleteBuffers(1, &m_VertexBufferID));
			m_VertexBufferID = 0; 
		}
	}

	void VertexBuffer::Bind()
	{
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));
	}

	void VertexBuffer::Unbind()
	{
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}