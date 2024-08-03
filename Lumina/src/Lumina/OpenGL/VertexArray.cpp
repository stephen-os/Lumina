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
            GLint location = program.GetAttributeLocation(attribute.GetName());
            if (location < 0) 
            {
                std::cout << attribute.GetName() << " is not used in the shader." << std::endl;
            }
            else 
            {
                std::cout << attribute.GetName() << ": " << attribute.GetBuffer() << std::endl;
                glBindBuffer(GL_ARRAY_BUFFER, attribute.GetBuffer());
                glVertexAttribPointer(location, attribute.GetComponents(), GL_FLOAT, GL_FALSE, 0, nullptr);
                glEnableVertexAttribArray(location);
            }
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attributes.GetIndexBuffer());

        Unbind();
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

    void VertexArray::Destroy()
    {
        glDeleteVertexArrays(1, &m_VertexArray);
    }

    void VertexArray::DrawSequence(GLenum mode)
    {
        if (m_IsBound)
        {
            glDrawArrays(mode, 0, m_Attributes.VertexCount()); 
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
            glDrawElements(mode, (GLint)m_Attributes.IndexCount(), GL_UNSIGNED_INT, 0);
        }
        else
        {
            std::cerr << "Vertex array is not bound!";
        }
    }
}