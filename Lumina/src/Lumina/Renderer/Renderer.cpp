#include "Renderer.h"

#include <glad/glad.h>
#include <iostream>

#include <stb_image_write.h>

namespace Lumina
{
    Ref<FrameBuffer> Renderer::s_FrameBuffer;

    void Renderer::Init()
    {
        s_FrameBuffer = Lumina::CreateRef<FrameBuffer>();
    }

    void Renderer::Shutdown()
    {
        s_FrameBuffer.reset();
    }

    void Renderer::Begin()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        s_FrameBuffer->Bind();
    }

    void Renderer::End()
    {
        s_FrameBuffer->Unbind();

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

    void Renderer::Enable(State param)
    {
        uint32_t state = ConvertToGLEnum(param);
        glEnable(state);
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        glViewport(0, 0, width, height);
        s_FrameBuffer->Resize(width, height);
    }

    void Renderer::SaveFrameBufferToImage(std::string& path)
    {
        s_FrameBuffer->Bind();

        std::vector<unsigned char> pixels(s_FrameBuffer->GetWidth() * s_FrameBuffer->GetHeight() * 4);
        glReadPixels(0, 0, s_FrameBuffer->GetWidth(), s_FrameBuffer->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

        if (!stbi_write_png(path.c_str(), s_FrameBuffer->GetWidth(), s_FrameBuffer->GetHeight(), 4, pixels.data(), s_FrameBuffer->GetWidth() * 4))
        {
            std::cout << "[Renderer] Failed to write FrameBuffer to file.";
        }

        s_FrameBuffer->Unbind();
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
