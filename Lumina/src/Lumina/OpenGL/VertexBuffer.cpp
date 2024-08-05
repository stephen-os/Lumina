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
		GLCALL(glGenBuffers(1, &m_BufferID));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
		GLCALL(glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), floats, GL_STATIC_DRAW));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	VertexBuffer::~VertexBuffer()
	{
		GLCALL(glDeleteBuffers(1, &m_BufferID));
	}

	void VertexBuffer::Bind()
	{
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
	}

	void VertexBuffer::Unbind()
	{
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}