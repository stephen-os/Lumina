#include "Texture.h"

#include "OpenGL/OpenGLTexture.h"

#include "../Core/API.h"

namespace Lumina
{
    Shared<Texture> Texture::Create(std::string& source)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::OPENGL: return MakeShared<OpenGLTexture>(source);
            case API::VULKAN: return nullptr;
            default: return nullptr;
        }
    }

    Shared<Texture> Texture::Create(uint32_t width, uint32_t height)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::OPENGL: return MakeShared<OpenGLTexture>(width, height);
            case API::VULKAN: return nullptr;
            default: return nullptr;
        }
    }
}
