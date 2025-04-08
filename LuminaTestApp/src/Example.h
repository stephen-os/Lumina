#include <vector>
#include <iostream>
#include <string>

#include "imgui.h"

#include "Lumina/Core/Layer.h"
#include "Lumina/Core/Aliases.h"
#include "Lumina/Utils/Timer.h"
#include "Lumina/Renderer/Renderer.h"
#include "Lumina/Renderer/Texture.h" 

class Example : public Lumina::Layer
{
public:
    virtual void OnAttach() override
    {      
        Lumina::Renderer::Init();

        std::string source = "res/texture/dirt.jpg";
        m_Texture = Lumina::Texture::Create(source);
    }

    virtual void OnDetach() override
    {

    }

    virtual void OnUpdate(float ts) override
    {
        // Update FPS counter
        float elapsedTime = m_FrameTimer.Elapsed();
        m_FPS = 1.0f / elapsedTime;
        m_FrameTimer.Reset();
    }

    virtual void OnUIRender() override
    {
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        Lumina::Renderer::SetResolution(viewportSize.x, viewportSize.y);

        Lumina::Renderer::Begin();
        Lumina::Renderer::DrawQuad({ 0, 0 }, { 1, 1 }, m_Texture, { 1, 1, 1, 1 });
        Lumina::Renderer::End(); 

        // Begin Window 
        ImGui::Begin("Example Window");
        ImGui::Image((void*)(intptr_t)Lumina::Renderer::GetImage(), viewportSize);
        ImGui::End();

        ImGui::Begin("Statistic");
        Lumina::Renderer::Statistics stats = Lumina::Renderer::GetStats();

        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quad Count: %d", stats.QuadCount);

        Lumina::Renderer::ResetStats(); 

        ImGui::End(); 

        // FPS Window
        ImGui::Begin("FPS Counter");
        ImGui::Text("FPS: %.1f", m_FPS);
        ImGui::End();
    }

private:
    Lumina::Shared<Lumina::Texture> m_Texture; 
    Lumina::Timer m_FrameTimer;
    float m_FPS = 0.0f;
};