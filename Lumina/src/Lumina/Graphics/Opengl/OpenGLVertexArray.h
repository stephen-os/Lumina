#pragma once

#include <vector>

#include "../Buffer.h"
#include "../VertexArray.h"

#include <glad/glad.h>

#include "../../Core/Aliases.h"

namespace Lumina
{
    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();

        void Bind() const override;
        void Unbind() const override;

        void SetVertexBuffer(Shared<VertexBuffer> vertexBuffer) override;
        void SetIndexBuffer(Shared<IndexBuffer> indexBuffer) override;

        Shared<VertexBuffer> GetVertexBuffer() override { return m_VertexBuffer; }
        Shared<IndexBuffer> GetIndexBuffer() override { return m_IndexBuffer; }

    private:
        uint32_t m_BufferID = 0;

        uint32_t m_VertexBufferIndex = 0;
        Shared<IndexBuffer> m_IndexBuffer;
        Shared<VertexBuffer> m_VertexBuffer;
    };
}