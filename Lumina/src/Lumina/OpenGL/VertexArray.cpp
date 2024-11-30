#include "VertexArray.h"
#include "GLUtils.h"

namespace GL
{
    VertexArray::VertexArray()
        : m_IsBound(false), m_IndexCount(0), m_VertexArrayID(0) {}

    VertexArray::VertexArray(const VertexArray& other)
        : m_IsBound(other.m_IsBound),
        m_IndexCount(other.m_IndexCount),
        m_VertexArrayID(other.m_VertexArrayID) {}

    VertexArray::VertexArray(VertexArray&& other) noexcept
        : m_IsBound(other.m_IsBound),
        m_IndexCount(other.m_IndexCount),
        m_VertexArrayID(other.m_VertexArrayID)
    {
        other.m_VertexArrayID = 0;
        other.m_IsBound = false;
        other.m_IndexCount = 0;
    }

    VertexArray& VertexArray::operator=(const VertexArray& other)
    {
        if (this != &other)
        {
            Destroy();

            m_IsBound = other.m_IsBound;
            m_IndexCount = other.m_IndexCount;
            m_VertexArrayID = other.m_VertexArrayID;
        }
        return *this;
    }

    VertexArray::~VertexArray()
    {
        Destroy();
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

        m_IndexCount = attributes.GetIndexCount();
        glGenVertexArrays(1, &m_VertexArrayID);
        glBindVertexArray(m_VertexArrayID);

        for (auto& [location, buffer] : attributes)
        {
            buffer.Bind();
            glVertexAttribPointer(location, buffer.GetStride(), GL_FLOAT, GL_FALSE, buffer.GetStride() * sizeof(float), nullptr);
            glEnableVertexAttribArray(location);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attributes.GetIndexBufferID());

        if (attributes.GetInstanceBufferID() != 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, attributes.GetInstanceBufferID());
            const int instanceSize = sizeof(glm::mat4);

            for (int i = 0; i < 4; ++i)
            {
                int instanceLocation = attributes.GetInstanceBuffer().GetLocation() + i;
                glEnableVertexAttribArray(instanceLocation);
                glVertexAttribPointer(instanceLocation, 4, GL_FLOAT, GL_FALSE, instanceSize, (void*)(sizeof(float) * i * 4));
                glVertexAttribDivisor(instanceLocation, 1);
            }
        }

        Unbind();
    }

    void VertexArray::ApplyAttributesInstanced(VertexAttributes& attributes, GLuint instanceBufferID)
    {
        Destroy();

        m_IndexCount = attributes.GetIndexCount();
        glGenVertexArrays(1, &m_VertexArrayID);
        glBindVertexArray(m_VertexArrayID);

        for (auto& [location, buffer] : attributes)
        {
            buffer.Bind();
            glVertexAttribPointer(location, buffer.GetStride(), GL_FLOAT, GL_FALSE, buffer.GetStride() * sizeof(float), nullptr);
            glEnableVertexAttribArray(location);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attributes.GetIndexBufferID());

        if (instanceBufferID != 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, instanceBufferID);
            const int instanceSize = sizeof(glm::mat4);

            for (int i = 0; i < 4; ++i)
            {
                glEnableVertexAttribArray(i);
                glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, instanceSize, (void*)(sizeof(float) * i * 4));
                glVertexAttribDivisor(i, 1);
            }
        }

        Unbind();
    }

    void VertexArray::DrawIndexed(GLenum mode)
    {
        if (m_IsBound)
        {
            glDrawElements(mode, m_IndexCount, GL_UNSIGNED_INT, nullptr);
        }
        else
        {
            std::cerr << "[Error] VertexArray is not bound.\n";
        }
    }

    void VertexArray::DrawInstanced(GLenum mode, int instanceCount)
    {
        if (m_IsBound)
        {
            glDrawElementsInstanced(mode, m_IndexCount, GL_UNSIGNED_INT, nullptr, instanceCount);
        }
        else
        {
            std::cerr << "[Error] VertexArray is not bound.\n";
        }
    }
}