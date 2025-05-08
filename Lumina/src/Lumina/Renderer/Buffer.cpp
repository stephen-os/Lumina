#include "Buffer.h"

#include "OpenGL/OpenGLBuffer.h"

#include "../Core/API.h"

namespace Lumina
{
    Shared<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::OPENGL: return MakeShared<OpenGLVertexBuffer>(size); 
            default: return nullptr; 
        }
    }

    Shared<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
        case API::OPENGL: return MakeShared<OpenGLVertexBuffer>(vertices, size);
        default: return nullptr;
        }
    }

    Shared<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (RendererAPI::GetAPI())
        {
        case API::OPENGL: return MakeShared<OpenGLIndexBuffer>(indices, count);
        default: return nullptr;
        }
    }
}
