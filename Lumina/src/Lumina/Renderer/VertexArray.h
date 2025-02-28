#pragma once

#include <vector>

#include "Buffer.h"

#include <glad/glad.h>

#include "../Core/Aliases.h"

namespace Lumina
{
    class VertexAttributes
    {
    public:
        VertexAttributes();
        ~VertexAttributes();

        void AddVertexBuffer(Shared<VertexBuffer> vertexBuffer);
        void SetIndexBuffer(Shared<IndexBuffer> indexBuffer);

        const Shared<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

        std::vector<Shared<VertexBuffer>>::iterator begin() { return m_Vertices.begin(); }
        std::vector<Shared<VertexBuffer>>::iterator end() { return m_Vertices.end(); }
        std::vector<Shared<VertexBuffer>>::const_iterator begin() const { return m_Vertices.begin(); }
        std::vector<Shared<VertexBuffer>>::const_iterator end() const { return m_Vertices.end(); }
    private:
        uint32_t m_VertexBufferIndex = 0;
        Shared<IndexBuffer> m_IndexBuffer;
        std::vector<Shared<VertexBuffer>> m_Vertices;
    };

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        void Unbind() const;

        void AddVertexBuffer(Shared<VertexBuffer> vertexBuffer);
        void SetIndexBuffer(Shared<IndexBuffer> indexBuffer);

        const VertexAttributes& GetAttributes() const { return m_Attributes; }
    private:
        uint32_t m_BufferID;
        VertexAttributes m_Attributes;
    };
}