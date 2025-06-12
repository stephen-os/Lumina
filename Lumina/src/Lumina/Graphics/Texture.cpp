#include "Texture.h"

#include "OpenGL/OpenGLTexture.h"

#include "../Core/API.h"

namespace Lumina
{
    std::shared_ptr<Texture> Texture::Create(std::string& source)
    {
        switch (RendererAPI::GetAPI())
        {
        case API::OPENGL: return std::make_shared<OpenGLTexture>(source);
        default: return nullptr;
        }
    }

    std::shared_ptr<Texture> Texture::Create(uint32_t width, uint32_t height)
    {
        switch (RendererAPI::GetAPI())
        {
        case API::OPENGL: return std::make_shared<OpenGLTexture>(width, height);
        default: return nullptr;
        }
    }
}
