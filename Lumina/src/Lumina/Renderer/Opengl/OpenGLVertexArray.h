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

        void AddVertexBuffer(Shared<VertexBuffer> vertexBuffer) override;
        void SetIndexBuffer(Shared<IndexBuffer> indexBuffer) override;

        void DrawIndexed() override;

        std::vector<Shared<VertexBuffer>>::iterator begin() override { return m_Vertices.begin(); }
        std::vector<Shared<VertexBuffer>>::iterator end() override { return m_Vertices.end(); }
        std::vector<Shared<VertexBuffer>>::const_iterator begin() const override { return m_Vertices.begin(); }
        std::vector<Shared<VertexBuffer>>::const_iterator end() const override { return m_Vertices.end(); }
    private:
        uint32_t m_BufferID = 0;

        uint32_t m_VertexBufferIndex = 0;
        Shared<IndexBuffer> m_IndexBuffer;
        std::vector<Shared<VertexBuffer>> m_Vertices;
    };
}