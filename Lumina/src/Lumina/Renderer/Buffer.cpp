#include "Buffer.h"

#include "OpenGL/OpenGLBuffer.h"
#include "Vulkan/VulkanBuffer.h"

#include "../Core/API.h"

namespace Lumina
{
    Shared<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::OPENGL: return MakeShared<OpenGLVertexBuffer>(size); 
            case API::VULKAN: return MakeShared<VulkanVertexBuffer>(size);
            default: return nullptr; 
        }
    }

    Shared<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
        case API::OPENGL: return MakeShared<OpenGLVertexBuffer>(vertices, size);
        case API::VULKAN: return MakeShared<VulkanVertexBuffer>(vertices, size);
        default: return nullptr;
        }
    }

    Shared<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (RendererAPI::GetAPI())
        {
        case API::OPENGL: return MakeShared<OpenGLIndexBuffer>(indices, count);
        case API::VULKAN: return MakeShared<VulkanIndexBuffer>(indices, count);
        default: return nullptr;
        }
    }
}
