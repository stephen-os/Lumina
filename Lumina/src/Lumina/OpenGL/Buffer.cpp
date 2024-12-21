#include "Buffer.h"
#include "GLUtils.h"

#include <iostream>

namespace GL
{
    Buffer::Buffer()
        : m_Name("Unnamed"), m_Type(0), m_Count(0), m_Stride(0), m_Size(0), m_IsInstance(false), m_ID(0) {}

    Buffer::~Buffer()
    {
        Delete();
    }

    // Copy constructor: Copies member variables and OpenGL state.
    Buffer::Buffer(const Buffer& other)
        : m_Name(other.m_Name), m_Type(other.m_Type), m_Count(other.m_Count),
        m_Stride(other.m_Stride), m_Size(other.m_Size), m_IsInstance(other.m_IsInstance), m_ID(other.m_ID) {}

    // Move constructor: Transfers ownership of OpenGL resources.
    Buffer::Buffer(Buffer&& other) noexcept
        : m_Name(std::move(other.m_Name)), m_Type(other.m_Type), m_Count(other.m_Count),
        m_Stride(other.m_Stride), m_Size(other.m_Size), m_IsInstance(other.m_IsInstance), m_ID(other.m_ID)
    {
        other.m_Name = "Unnamed";
        other.m_Type = 0;
        other.m_Count = 0;
        other.m_Stride = 0;
        other.m_Size = 0;
        other.m_IsInstance = false;
        other.m_ID = 0;
    }

    // Copy assignment operator: Handles self-assignment and duplicates member data.
    Buffer& Buffer::operator=(const Buffer& other)
    {
        if (this != &other)
        {
            m_Name = other.m_Name;
            m_Type = other.m_Type;
            m_Count = other.m_Count;
            m_Stride = other.m_Stride;
            m_Size = other.m_Size;
            m_IsInstance = other.m_IsInstance;
            m_ID = other.m_ID;
        }
        return *this;
    }

    // Initializes the buffer by generating an OpenGL buffer ID.
    void Buffer::Init()
    {
        if (m_ID == 0)
        {
            GLCALL(glGenBuffers(1, &m_ID));
        }
    }

    // Deletes the OpenGL buffer if it exists, freeing GPU memory.
    void Buffer::Delete()
    {
        if (m_ID != 0)
        {
            GLCALL(glDeleteBuffers(1, &m_ID));
            m_ID = 0;
        }
    }

    // Binds the buffer to the specified OpenGL target.
    void Buffer::Bind() const
    {
        if (m_ID != 0)
        {
            GLCALL(glBindBuffer(m_Type, m_ID));
        }
    }

    // Unbinds the buffer from its target.
    void Buffer::Unbind() const
    {
        GLCALL(glBindBuffer(m_Type, 0));
    }

    // Sets buffer data and uploads it to the GPU.
    void Buffer::SetData(GLuint type, const void* data, uint32_t count, uint32_t stride, bool isInstance, GLenum usage)
    {
        m_Type = type;
        m_Count = count;
        m_Stride = stride;
        m_Size = 4; // Assume 4 bytes 
        m_IsInstance = isInstance;

        Init();
        Bind();

        GLCALL(glBufferData(type, count * m_Size, data, usage));
        
        Unbind();
    }



    void Buffer::UpdateBuffer(const void* data, int count, GLenum usage)
    {
        if (m_ID == 0)
        {
            std::cerr << "Buffer not initialized. Call Init() before updating data.\n";
            return;
        }

        Bind();

        glBufferData(m_Type, count * m_Size, data, usage);

        Unbind();
    }
}

