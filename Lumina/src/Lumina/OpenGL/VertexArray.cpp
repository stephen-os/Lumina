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
            if (buffer.IsInstance())
			{
                std::cout << "Instance buffer: " << buffer.GetName() << std::endl;
                buffer.Bind();
                const GLsizei instanceSize = buffer.GetSize();
                for (uint32_t i = 0; i < buffer.GetCount(); ++i)
                {
                    int instanceLocation = location + i;
                    glEnableVertexAttribArray(instanceLocation);
                    glVertexAttribPointer(instanceLocation, buffer.GetStride(), GL_FLOAT, GL_FALSE, instanceSize, (void*)(sizeof(float) * i * buffer.GetStride()));
                    glVertexAttribDivisor(instanceLocation, 1);
                }
                buffer.Unbind();
			}
            else
            {
                buffer.Bind();
                glVertexAttribPointer(location, buffer.GetStride(), GL_FLOAT, GL_FALSE, buffer.GetStride() * sizeof(float), nullptr);
                glEnableVertexAttribArray(location);
                buffer.Unbind();
            }
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attributes.GetIndexBufferID());

        /*
        glBindBuffer(GL_ARRAY_BUFFER, attributes.GetBufferID(3));
        const int instanceSize = sizeof(glm::mat4);

        std::cout << "Instance size: " << instanceSize << std::endl;

        for (int i = 0; i < 4; ++i)
        {
            int instanceLocation = 3 + i;
            glEnableVertexAttribArray(instanceLocation);
            glVertexAttribPointer(instanceLocation, 4, GL_FLOAT, GL_FALSE, instanceSize, (void*)(sizeof(float) * i * 4));
            glVertexAttribDivisor(instanceLocation, 1);
        }
        */
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