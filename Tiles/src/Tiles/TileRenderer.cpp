#include "TileRenderer.h"

#include "Lumina/OpenGL/GLUtils.h"

#include "Lumina/Utils/FileReader.h"

#include <glm/glm.hpp>

#include <imgui.h>

TileRenderer::TileRenderer()
{
    GLCALL(glEnable(GL_DEPTH_TEST));
    GLCALL(glDepthFunc(GL_LEQUAL));
    GLCALL(glEnable(GL_CULL_FACE));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_DepthBuffer.SetData((int)m_Width, (int)m_Height);
    m_Texture.SetResolution((int)m_Width, (int)m_Height);

    m_FrameBuffer.AttachDepthBuffer(m_DepthBuffer.GetID());
    m_FrameBuffer.AttachTexture(m_Texture.GetID());

    {
        std::string vertex = Lumina::ReadFile("res/shaders/tile.vert");
        std::string fragment = Lumina::ReadFile("res/shaders/tile.frag");
        m_TileShader.SetSource(vertex, fragment);
    }

    {
        std::string vertex = Lumina::ReadFile("res/shaders/grid.vert");
        std::string fragment = Lumina::ReadFile("res/shaders/grid.frag");
        m_GridShader.SetSource(vertex, fragment);
    }

    m_Camera.SetPosition(glm::vec3(10.0f, 10.0f, 26.0f));
}

TileRenderer::~TileRenderer()
{
    // Destroy buffers
    m_DepthBuffer.Destroy();
    m_FrameBuffer.Destroy();
    m_Texture.Destroy();

    // Destroy shaders
    m_GridShader.Destroy();
    m_TileShader.Destroy();
}

void TileRenderer::Render(std::vector<glm::mat4>& transforms, std::vector<glm::vec2>& offsets)
{
    ImGui::Begin("Scene View");

    m_Camera.HandleMouseInput(0.1f);

    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    float zoom = 90.0f; 

    SetViewportSize(viewportSize.x, viewportSize.y);

    m_Camera.SetProjectionMatrix(
        -viewportSize.x / zoom, 
        viewportSize.x / zoom, 
        -viewportSize.y / zoom, 
        viewportSize.y / zoom, 
        0.1f, 100.0f
    );

    m_Texture.Bind();
    m_DepthBuffer.Bind();
    m_FrameBuffer.Bind();

    GLCALL(glViewport(0, 0, (int)m_Width, (int)m_Height));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // Draw grid first
    m_GridShader.Bind();
    m_GridShader.SetUniformMatrix4fv("u_View", m_Camera.GetViewMatrix());
    m_GridShader.SetUniformMatrix4fv("u_Projection", m_Camera.GetProjectionMatrix());
    m_Grid.Draw(m_GridShader);
    m_GridShader.Unbind();

    // Draw tiles
    m_TileShader.Bind();
    m_TileShader.SetUniformMatrix4fv("u_View", m_Camera.GetViewMatrix());
    m_TileShader.SetUniformMatrix4fv("u_Projection", m_Camera.GetProjectionMatrix());
    m_TileShader.SetUniform1f("u_NumberOfRows", 16.0f);

    for (size_t i = 0; i < transforms.size(); i++)
    {
        m_TileShader.SetUniformMatrix4fv("u_Transform", transforms[i]);
        m_TileShader.SetUniform2fv("u_Offset", offsets[i]);
        m_TileObject.Draw(m_TileShader);
    }

    m_TileShader.Unbind();

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

    if (m_DepthBuffer.SetData((int)m_Width, (int)m_Height))
        m_FrameBuffer.AttachDepthBuffer(m_DepthBuffer.GetID());

    if (m_Texture.SetResolution((int)m_Width, (int)m_Height))
        m_FrameBuffer.AttachTexture(m_Texture.GetID());
}