#include "Renderer.h"

#include <glad/glad.h>
#include <iostream>

#include <stb_image_write.h>

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

        m_FrameBuffer->Bind();
    }

    void Renderer::End()
    {
        m_FrameBuffer->Unbind();

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

    void Renderer::SaveFrameBufferToImage(std::string& path)
    {
        m_FrameBuffer->Bind();

        std::vector<unsigned char> pixels(m_FrameBuffer->GetWidth() * m_FrameBuffer->GetHeight() * 4);
        glReadPixels(0, 0, m_FrameBuffer->GetWidth(), m_FrameBuffer->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

        if (!stbi_write_png(path.c_str(), m_FrameBuffer->GetWidth(), m_FrameBuffer->GetHeight(), 4, pixels.data(), m_FrameBuffer->GetWidth() * 4))
        {
            std::cout << "[Renderer] Faild to write FrameBuffer to file.";
        }

        m_FrameBuffer->Unbind();
    }

    void Renderer::Draw(const Ref<VertexArray>& vertexArray)
    {
        vertexArray->Bind();

        const auto& attributes = vertexArray->GetAttributes();
        const auto& indexBuffer = attributes.GetIndexBuffer();
        if (indexBuffer)
        {
            glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
        }

        vertexArray->Unbind();
    }

    void Renderer::DrawInstanced(const Ref<VertexArray>& vertexArray, uint32_t instances)
    {
        vertexArray->Bind();

        const auto& attributes = vertexArray->GetAttributes();
        const auto& indexBuffer = attributes.GetIndexBuffer();
        if (indexBuffer)
        {
            glDrawElementsInstanced(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr, instances);
        }

        vertexArray->Unbind();
    }
}