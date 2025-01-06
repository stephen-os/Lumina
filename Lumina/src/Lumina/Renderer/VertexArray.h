#pragma once

#include <vector>

#include "Buffer.h"

#include <glad/glad.h>

#include "../base.h"

namespace Lumina
{
    class VertexAttributes
    {
    public:
        VertexAttributes();
        ~VertexAttributes();

        void AddVertexBuffer(Ref<VertexBuffer> vertexBuffer);
        void SetIndexBuffer(Ref<IndexBuffer> indexBuffer);

        const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

        std::vector<Ref<VertexBuffer>>::iterator begin() { return m_Vertices.begin(); }
        std::vector<Ref<VertexBuffer>>::iterator end() { return m_Vertices.end(); }
        std::vector<Ref<VertexBuffer>>::const_iterator begin() const { return m_Vertices.begin(); }
        std::vector<Ref<VertexBuffer>>::const_iterator end() const { return m_Vertices.end(); }
    private:
        uint32_t m_VertexBufferIndex = 0;
        Ref<IndexBuffer> m_IndexBuffer;
        std::vector<Ref<VertexBuffer>> m_Vertices;
    };

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        void Unbind() const;

        void AddVertexBuffer(Ref<VertexBuffer> vertexBuffer);
        void SetIndexBuffer(Ref<IndexBuffer> indexBuffer);

        const VertexAttributes& GetAttributes() const { return m_Attributes; }
    private:
        uint32_t m_BufferID;
        VertexAttributes m_Attributes;
    };
}