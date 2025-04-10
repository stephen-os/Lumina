#include <vector>
#include <iostream>
#include <string>

#include "imgui.h"

#include "Lumina/Core/Layer.h"
#include "Lumina/Core/Aliases.h"
#include "Lumina/Utils/Timer.h"
#include "Lumina/Renderer/Renderer.h"
#include "Lumina/Renderer/TextureAtlas.h"

class Example : public Lumina::Layer
{
public:
    virtual void OnAttach() override
    {
        Lumina::Renderer::Init();

        std::string source = "res/texture/factory_atlas.png";
        m_Atlas = Lumina::MakeShared<Lumina::TextureAtlas>(source, 16, 16);
        m_TileIndex = 3;
    }

    virtual void OnDetach() override {}

    virtual void OnUpdate(float ts) override
    {
        float elapsedTime = m_FrameTimer.Elapsed();
        m_FPS = 1.0f / elapsedTime;
        m_FrameTimer.Reset();
    }

    virtual void OnUIRender() override
    {
        ImGui::Begin("Example Window");

        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        Lumina::Renderer::SetResolution(viewportSize.x, viewportSize.y);

        Lumina::Renderer::Begin();
        Lumina::Renderer::DrawQuad(m_Position, m_Size, m_Atlas->GetTexture(), m_Atlas->GetTexCoods(m_TileIndex), m_Color);
        Lumina::Renderer::End();

        ImGui::Image((void*)(intptr_t)Lumina::Renderer::GetImage(), viewportSize);
        ImGui::End();

        ImGui::Begin("Statistic");
        Lumina::Renderer::Statistics stats = Lumina::Renderer::GetStats();
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quad Count: %d", stats.QuadCount);
        Lumina::Renderer::ResetStats();
        ImGui::End();

        ImGui::Begin("FPS Counter");
        ImGui::Text("FPS: %.1f", m_FPS);
        ImGui::End();

        ImGui::Begin("DrawQuad Parameters");
        ImGui::DragFloat2("Position", &m_Position.x, 0.01f);
        ImGui::DragFloat2("Size", &m_Size.x, 0.01f);
        ImGui::ColorEdit4("Color", &m_Color.r);
        ImGui::InputInt("Tile Index", &m_TileIndex);
        if (m_TileIndex < 0) m_TileIndex = 0;
        int maxIndex = m_Atlas->GetWidth() * m_Atlas->GetHeight() - 1;
        if (m_TileIndex > maxIndex) m_TileIndex = maxIndex;
        ImGui::End();
    }

private:
    Lumina::Shared<Lumina::TextureAtlas> m_Atlas;
    Lumina::Timer m_FrameTimer;
    float m_FPS = 0.0f;

    glm::vec2 m_Position = { 0.0f, 0.0f };
    glm::vec2 m_Size = { 1.0f, 1.0f };
    glm::vec4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    int m_TileIndex = 0;
};
