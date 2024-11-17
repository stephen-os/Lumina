#include "IndexBuffer.h"
#include "GLUtils.h"

#include <glad/glad.h>

#include <iostream>

namespace GL
{
	IndexBuffer::IndexBuffer() : m_Count(0), m_IndexBufferID(0) {} 

	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count, GLenum usage) : m_Count(count), m_IndexBufferID(0)
	{
		if (m_IndexBufferID == 0)
		{
			GLCALL(glGenBuffers(1, &m_IndexBufferID));
			GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID));
			GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, usage));
			GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}
	}

    IndexBuffer::IndexBuffer(const IndexBuffer& other) 
		: m_Count(other.m_Count), m_IndexBufferID(other.m_IndexBufferID) {}

	IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept 
		: m_Count(other.m_Count), m_IndexBufferID(other.m_IndexBufferID)
	{
		other.m_Count = 0;
		other.m_IndexBufferID = 0;
	}

    IndexBuffer& IndexBuffer::operator=(const IndexBuffer& other) 
    {
		if (this != &other)
		{
			m_Count = other.m_Count;
			m_IndexBufferID = other.m_IndexBufferID;
		}
        return *this;
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

	void IndexBuffer::SetData(const unsigned int* data, unsigned int count, GLenum usage)
	{
		Destroy(); 

		m_Count = count;

		GLCALL(glGenBuffers(1, &m_IndexBufferID));
		Bind();
		GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, usage));
		Unbind();
	}
}