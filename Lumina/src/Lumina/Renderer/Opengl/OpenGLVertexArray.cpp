#include "OpenGLVertexArray.h"
#include "RendererDebug.h"

#include <iostream>

#include <glad/glad.h>
#include "../../Core/Aliases.h"

#include "../BufferLayout.h"

namespace Lumina 
{

    OpenGLVertexArray::OpenGLVertexArray()
    {
        GLCALL(glCreateVertexArrays(1, &m_BufferID));
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        GLCALL(glDeleteVertexArrays(1, &m_BufferID));
    }

    void OpenGLVertexArray::Bind() const
    {
        GLCALL(glBindVertexArray(m_BufferID));
    }

    void OpenGLVertexArray::Unbind() const
    {
        GLCALL(glBindVertexArray(0));
    }

    void OpenGLVertexArray::AddVertexBuffer(Shared<VertexBuffer> vertexBuffer)
    {
        Bind();
        vertexBuffer->Bind();
        
        const auto& layout = vertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            switch (element.Type)
            {
            case BufferDataType::Float:
            {
                GLCALL(glEnableVertexAttribArray(m_VertexBufferIndex));
                GLCALL(glVertexAttribPointer(
                    m_VertexBufferIndex,
                    element.GetComponentCount(),
                    GL_FLOAT,
                    element.Normalized ? GL_TRUE : GL_FALSE,
                    layout.GetStride(),
                    (const void*)element.Offset));
                m_VertexBufferIndex++;
                break;
            }
            case BufferDataType::Float2:
            {

                GLCALL(glEnableVertexAttribArray(m_VertexBufferIndex));
                GLCALL(glVertexAttribPointer(
                    m_VertexBufferIndex,
                    element.GetComponentCount(),
                    GL_FLOAT,
                    element.Normalized ? GL_TRUE : GL_FALSE,
                    layout.GetStride(),
                    (const void*)element.Offset));
                m_VertexBufferIndex++;
                break;
            }
            case BufferDataType::Float3:
            {
                GLCALL(glEnableVertexAttribArray(m_VertexBufferIndex));
                GLCALL(glVertexAttribPointer(
                    m_VertexBufferIndex,
                    element.GetComponentCount(),
                    GL_FLOAT,
                    element.Normalized ? GL_TRUE : GL_FALSE,
                    layout.GetStride(),
                    (const void*)element.Offset));
                m_VertexBufferIndex++;
                break;
            }
            case BufferDataType::Float4:
            {
                GLCALL(glEnableVertexAttribArray(m_VertexBufferIndex));
                GLCALL(glVertexAttribPointer(m_VertexBufferIndex,
                    element.GetComponentCount(),
                    GL_FLOAT,
                    element.Normalized ? GL_TRUE : GL_FALSE,
                    layout.GetStride(),
                    (const void*)element.Offset));
                m_VertexBufferIndex++;
                break;
            }
            case BufferDataType::Int:
            case BufferDataType::Int2:
            case BufferDataType::Int3:
            case BufferDataType::Int4:
            case BufferDataType::Bool:
            {
                GLCALL(glEnableVertexAttribArray(m_VertexBufferIndex));
                GLCALL(glVertexAttribIPointer(m_VertexBufferIndex,
                    element.GetComponentCount(),
                    CalculateDataTypeSize(element.Type),
                    layout.GetStride(),
                    (const void*)element.Offset));
                m_VertexBufferIndex++;
                break;
            }
            case BufferDataType::Mat3:
            case BufferDataType::Mat4:
            {
                uint8_t count = element.GetComponentCount();
                for (uint8_t i = 0; i < count; i++)
                {
                    GLCALL(glEnableVertexAttribArray(m_VertexBufferIndex));
                    GLCALL(glVertexAttribPointer(m_VertexBufferIndex,
                        count,
                        CalculateDataTypeSize(element.Type),
                        element.Normalized ? GL_TRUE : GL_FALSE,
                        layout.GetStride(),
                        (const void*)(element.Offset + sizeof(float) * count * i)));
                    GLCALL(glVertexAttribDivisor(m_VertexBufferIndex, 1));
                    m_VertexBufferIndex++;
                }
                break;
            }
            default:
                // TODO: Logging system 
                std::cout << "Unknown BufferDataType\n";
            }
        }

        m_Vertices.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(Shared<IndexBuffer> indexBuffer)
    {
        Bind();
        indexBuffer->Bind();
        m_IndexBuffer = indexBuffer;
    }

    void OpenGLVertexArray::DrawIndexed()
    {
        for (auto& vertice : m_Vertices)
        {
            vertice->Bind();
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
            vertice->Unbind();
        }
    }
}