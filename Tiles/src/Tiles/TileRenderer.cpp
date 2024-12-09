#include "TileRenderer.h"

#include "Lumina/OpenGL/GLUtils.h"

#include <glm/glm.hpp>

#include <imgui.h>

TileRenderer::TileRenderer()
{
    GLCALL(glEnable(GL_DEPTH_TEST));
    GLCALL(glDepthFunc(GL_LEQUAL));
    GLCALL(glEnable(GL_CULL_FACE));

    m_DepthBuffer.SetData((int)m_Width, (int)m_Height);
    m_Texture.SetResolution((int)m_Width, (int)m_Height);

    m_FrameBuffer.AttachDepthBuffer(m_DepthBuffer.GetID());
    m_FrameBuffer.AttachTexture(m_Texture.GetID());
}

TileRenderer::~TileRenderer()
{
    m_DepthBuffer.Destroy();
    m_FrameBuffer.Destroy();
    m_Texture.Destroy();
}

void TileRenderer::Render(OrthographicCamera& camera, std::vector<glm::mat4>& transforms, std::vector<glm::vec2>& offsets, GL::ShaderProgram& shader)
{
    ImGui::Begin("Scene View");

    camera.HandleMouseInput(0.1f);

    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    SetViewportSize(viewportSize.x, viewportSize.y);

    m_Texture.Bind();
    m_DepthBuffer.Bind();
    m_FrameBuffer.Bind();

    GLCALL(glViewport(0, 0, (int)m_Width, (int)m_Height));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    shader.Bind();
    shader.SetUniformMatrix4fv("u_View", camera.GetViewMatrix());
    shader.SetUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());
    shader.SetUniform1f("u_NumberOfRows", 4.0f);

    for (size_t i = 0; i < transforms.size(); i++)
    {
        shader.SetUniformMatrix4fv("u_Transform", transforms[i]);
        shader.SetUniform2fv("u_Offset", offsets[i]);
        m_TileObject.Draw(shader);
    }

    shader.Unbind();

    m_Texture.Unbind();
    m_DepthBuffer.Unbind();
    m_FrameBuffer.Unbind();

    ImGui::Image((void*)(intptr_t)GetRendererID(), ImVec2(viewportSize.x, viewportSize.y));
    ImGui::End();
}


void TileRenderer::SetViewportSize(const float width, const float height)
{
    m_Width = width;
    m_Height = height;

    if (m_DepthBuffer.SetData((int)width, (int)height))
        m_FrameBuffer.AttachDepthBuffer(m_DepthBuffer.GetID());

    if (m_Texture.SetResolution((int)width, (int)height))
        m_FrameBuffer.AttachTexture(m_Texture.GetID());
}