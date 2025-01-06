#include "Renderer.h"

#include <glad/glad.h>

namespace Lumina 
{

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

    void Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OnWindowResize(uint32_t width, uint32_t height)
    {
        glViewport(0, 0, width, height);
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

        // ToDo DrawArrays. 
    }

}