#include "RenderCommands.h"

#include "Opengl/OpenGLRenderCommands.h"

#include "../Core/API.h"

namespace Lumina
{
    void RenderCommands::SetViewport(int x, int y, int width, int height) 
    {
        switch (RendererAPI::GetAPI())
        {
        case API::OPENGL: OpenGLRenderCommands::SetViewport(x, y, width, height); break; 
        default: break; 
        }
        
    }

    void RenderCommands::Clear() 
    {
        switch (RendererAPI::GetAPI())
        {
        case API::OPENGL: OpenGLRenderCommands::Clear(); break;
        default: break;
        }
    }

    void RenderCommands::EnableDepthTest() 
    {
        switch (RendererAPI::GetAPI())
        {
        case API::OPENGL: OpenGLRenderCommands::EnableDepthTest(); break;
        default: break;
        }
    }

    void RenderCommands::DrawLines(const std::shared_ptr<VertexArray>& vao, uint32_t count) 
    {
        vao->Bind();

        switch (RendererAPI::GetAPI())
        {
        case API::OPENGL: OpenGLRenderCommands::DrawLines(count); break;
        default: break;
        }

        vao->Unbind(); 
    }

    void RenderCommands::DrawLineStrips(const std::shared_ptr<VertexArray>& vao, uint32_t count)
    {
        vao->Bind();

        switch (RendererAPI::GetAPI())
        {
        case API::OPENGL: OpenGLRenderCommands::DrawLineStrips(count); break;
        default: break;
        }

        vao->Unbind();
    }

    void RenderCommands::DrawTriangles(const std::shared_ptr<VertexArray>& vao)
    {
        vao->Bind();
        auto ib = vao->GetIndexBuffer();

        switch (RendererAPI::GetAPI())
        {
        case API::OPENGL: OpenGLRenderCommands::DrawTriangles(ib->GetCount()); break;
        default: break;
        }

        vao->Unbind(); 
    }
}
