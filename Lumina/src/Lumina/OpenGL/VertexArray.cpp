#include "VertexArray.h"
#include "GLUtils.h"

namespace GL
{
    VertexArray::VertexArray() : m_IsBound(false), m_VertexCount(0), m_IndexCount(0), m_VertexArrayID(0) {}

    VertexArray::VertexArray(const VertexArray& other)
        : m_IsBound(other.m_IsBound), m_VertexCount(other.m_VertexCount), 
        m_IndexCount(other.m_IndexCount), m_VertexArrayID(other.m_VertexArrayID) {}

    VertexArray::VertexArray(VertexArray&& other) noexcept
        : m_IsBound(other.m_IsBound), m_VertexCount(other.m_VertexCount), 
        m_IndexCount(other.m_IndexCount), m_VertexArrayID(other.m_VertexArrayID)
    {
        other.m_VertexArrayID = 0;
        other.m_IsBound = false;
        other.m_VertexCount = 0;
        other.m_IndexCount = 0;
    }

    VertexArray& VertexArray::operator=(const VertexArray& other)
    {
        if (this != &other)
        {
            Destroy();

            m_IsBound = other.m_IsBound;
            m_VertexCount = other.m_VertexCount;
            m_IndexCount = other.m_IndexCount;
            m_VertexArrayID = other.m_VertexArrayID;
        }
        return *this;
    }

    void VertexArray::Destroy()
    {
        if (m_VertexArrayID != 0)
        {
            GLCALL(glDeleteVertexArrays(1, &m_VertexArrayID));
            m_VertexArrayID = 0;
        }
    }

    void VertexArray::Bind()
    {
        if (m_VertexArrayID == 0)
        {
            std::cerr << "[Error] Attempting to bind an ungenerated vertex array.\n";
            return;
        }

        GLCALL(glBindVertexArray(m_VertexArrayID));
        m_IsBound = true;
    }

    void VertexArray::Unbind()
    {
        GLCALL(glBindVertexArray(0));
        m_IsBound = false;
    }

    void VertexArray::ApplyAttributes(VertexAttributes& attributes)
    {
        Destroy(); 

        m_VertexCount = attributes.GetVertexCount();
        m_IndexCount = attributes.GetIndexCount(); 
        
        glGenVertexArrays(1, &m_VertexArrayID);
        glBindVertexArray(m_VertexArrayID);

        for (auto& attribute : attributes)
        {
                attribute.Bind();
                glVertexAttribPointer(attribute.GetLocation(), attribute.GetStride(), GL_FLOAT, GL_FALSE, attribute.GetStride() * sizeof(float), nullptr);
                glEnableVertexAttribArray(attribute.GetLocation());
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attributes.GetIndexBufferID());

        Unbind();
    }

    void VertexArray::DrawSequence(GLenum mode)
    {
        if (m_IsBound)
        {
            glDrawArrays(mode, 0, m_VertexCount);
        }
        else
        {
            std::cerr << "Vertex array is not bound!";
        }
    }

    void VertexArray::DrawIndexed(GLenum mode)
    {
        if (m_IsBound)
        {
            glDrawElements(mode, m_IndexCount, GL_UNSIGNED_INT, 0);
        }
        else
        {
            std::cerr << "Vertex array is not bound!";
        }
    }
}