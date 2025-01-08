#include "Renderer.h"

#include <glad/glad.h>

namespace Lumina 
{
    void Renderer::Init()
    {
        m_FrameBuffer = Lumina::CreateRef<FrameBuffer>();
    }

    void Renderer::Begin()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Renderer::End()
    {
        glDisable(GL_DEPTH_TEST);
    }

    void Renderer::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::ClearColor(float r, float g, float b, float a)
    {

        glClearColor(r, g, b, a);
    }

    void Renderer::Enable(RenderState param)
    {
        uint32_t state = ConvertToGLEnum(param);
        glEnable(state); 
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        glViewport(0, 0, width, height);
        m_FrameBuffer->Resize(width, height);
    }

    void Renderer::Draw(const Ref<VertexArray>& vertexArray)
    {
        m_FrameBuffer->Bind();

        vertexArray->Bind();

        const auto& attributes = vertexArray->GetAttributes();
        const auto& indexBuffer = attributes.GetIndexBuffer();
        if (indexBuffer)
        {
            glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
        }

        vertexArray->Unbind();

        m_FrameBuffer->Unbind(); 

        // ToDo DrawArrays. 
    }

}