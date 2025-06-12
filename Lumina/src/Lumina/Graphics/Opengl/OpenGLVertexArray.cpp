#include "OpenGLVertexArray.h"
#include "RendererDebug.h"

#include <glad/glad.h>

#include "../../Core/Assert.h"
#include "../../Core/Log.h"

#include "../BufferLayout.h"

namespace Lumina 
{

    OpenGLVertexArray::OpenGLVertexArray()
    {
        GLCALL(glCreateVertexArrays(1, &m_BufferID));
        LUMINA_ASSERT(m_BufferID != 0, "Failed to create OpenGL Vertex Array Object.");
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        LUMINA_ASSERT(m_BufferID != 0, "Trying to delete an invalid Vertext Array Object.");
        GLCALL(glDeleteVertexArrays(1, &m_BufferID));
    }

    void OpenGLVertexArray::Bind() const
    {
        LUMINA_ASSERT(m_BufferID != 0, "Cannot bind VAO with invalid ID.");
        GLCALL(glBindVertexArray(m_BufferID));

        if (m_VertexBuffer)
            m_VertexBuffer->Bind();
        if (m_IndexBuffer)
            m_IndexBuffer->Bind();
    }

    void OpenGLVertexArray::Unbind() const
    {
        GLCALL(glBindVertexArray(0));

        if (m_VertexBuffer)
            m_VertexBuffer->Unbind();
        if (m_IndexBuffer)
            m_IndexBuffer->Unbind();
    }

    void OpenGLVertexArray::SetVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
    {
        LUMINA_ASSERT(vertexBuffer != nullptr, "VertexBuffer passed to AddVertexBuffer is null.");

        Bind();
        vertexBuffer->Bind();
        
        const auto& layout = vertexBuffer->GetLayout();
        LUMINA_ASSERT(layout.GetElements().size() > 0, "VertexBuffer has no layout elements defined.");

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
				LUMINA_LOG_ERROR("Unknown BufferDataType: {0}", element.Name);
            }
        }

        m_VertexBuffer.reset(); 
        m_VertexBuffer = vertexBuffer;
    }

    void OpenGLVertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
    {
        LUMINA_ASSERT(indexBuffer != nullptr, "IndexBuffer passed to SetIndexBuffer is null.");

        Bind();
        indexBuffer->Bind();
        m_IndexBuffer = indexBuffer;
    }
}