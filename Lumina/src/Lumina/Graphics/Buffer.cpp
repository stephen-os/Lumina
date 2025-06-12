#include "Buffer.h"

#include "OpenGL/OpenGLBuffer.h"

#include "../Core/API.h"

namespace Lumina
{
    std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
        case API::OPENGL: return std::make_shared<OpenGLVertexBuffer>(size);
            default: return nullptr; 
        }
    }

    std::shared_ptr<VertexBuffer> VertexBuffer::Create(const void* vertices, uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
        case API::OPENGL: return std::make_shared<OpenGLVertexBuffer>(vertices, size);
        default: return nullptr;
        }
    }

    std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (RendererAPI::GetAPI())
        {
        case API::OPENGL: return std::make_shared<OpenGLIndexBuffer>(indices, count);
        default: return nullptr;
        }
    }
}
