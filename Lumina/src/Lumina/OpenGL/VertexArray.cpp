#include "VertexArray.h"

namespace GL
{
    VertexArray::VertexArray(ShaderProgram& program, VertexAttributes& attributes)
        : m_Attributes(attributes), m_IsBound(false)
    {
        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        for (auto& attribute : m_Attributes)
        {
            GLint location = program.GetAttributeLocation(attribute->GetBufferName());
            if (location < 0)
            {
                std::cout << attribute->GetBufferName() << " is not used in the shader." << std::endl;
            }
            else
            {
                attribute->Bind(); 
                glVertexAttribPointer(location, attribute->GetStride(), GL_FLOAT, GL_FALSE, attribute->GetStride() * sizeof(float), nullptr);
                glEnableVertexAttribArray(location);
            }
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attributes.GetIndexBufferID());

        Unbind();
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_VertexArray);
    }

    void VertexArray::Bind()
    {
        glBindVertexArray(m_VertexArray);
        m_IsBound = true;
    }

    void VertexArray::Unbind()
    {
        glBindVertexArray(0);
        m_IsBound = false;
    }

    void VertexArray::DrawSequence(GLenum mode)
    {
        if (m_IsBound)
        {
            glDrawArrays(mode, 0, m_Attributes.GetVertexCount());
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
            glDrawElements(mode, (GLint)m_Attributes.GetIndexCount(), GL_UNSIGNED_INT, 0);
        }
        else
        {
            std::cerr << "Vertex array is not bound!";
        }
    }
}