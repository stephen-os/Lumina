#include "VertexArray.h"
#include "GLUtils.h"

namespace GL
{
    VertexArray::VertexArray(VertexAttributes attributes, ShaderProgram& shader)
        : m_Attributes(attributes),
          m_IsBound(false)
    {
        glGenVertexArrays(1, &m_VertexArrayID);
        glBindVertexArray(m_VertexArrayID);

        for (auto& attribute : attributes)
        {
            GLint location = shader.GetAttributeLocation(attribute.GetBufferName());
            if (location < 0)
            {
                std::cout << attribute.GetBufferName() << " is not used in the shader." << std::endl;
            }
            else
            {
                attribute.Bind();
                glVertexAttribPointer(location, attribute.GetStride(), GL_FLOAT, GL_FALSE, attribute.GetStride() * sizeof(float), nullptr);
                glEnableVertexAttribArray(location);
            }
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attributes.GetIndexBufferID());

        Unbind();
    }

    void VertexArray::Destroy()
    {
        m_Attributes.Destroy(); 

        if (m_VertexArrayID != 0)
        {
            GLDESTROY("VertexArray", m_VertexArrayID);
            GLCALL(glDeleteVertexArrays(1, &m_VertexArrayID));
            m_VertexArrayID = 0;
        }
    }

    void VertexArray::Bind()
    {
        GLCALL(glBindVertexArray(m_VertexArrayID));
        m_IsBound = true;
    }

    void VertexArray::Unbind()
    {
        GLCALL(glBindVertexArray(0));
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