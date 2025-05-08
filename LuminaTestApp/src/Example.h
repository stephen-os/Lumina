#include <vector>
#include <iostream>
#include <string>

#include "imgui.h"

#include "Lumina/Core/Layer.h"
#include "Lumina/Core/Aliases.h"
#include "Lumina/Utils/Timer.h"
#include "Lumina/Renderer/Renderer.h"
#include "Lumina/Renderer/TextureAtlas.h"

#include "Lumina/Renderer/Cameras/OrthographicCamera.h"
#include "Lumina/Renderer/Cameras/PerspectiveCamera.h"

#include "Lumina/Utils/FileReader.h"
#include <algorithm>

class Example : public Lumina::Layer
{
public:
    virtual void OnAttach() override
    {
        Lumina::Renderer::Init();

        std::string source = "res/texture/factory_atlas.png";
        m_Atlas = Lumina::MakeShared<Lumina::TextureAtlas>(source, 16, 16);

        int tilesWide = static_cast<int>(1.0f / 0.04f);
        int tilesHigh = static_cast<int>(1.0f / 0.04f);

		m_Quads.reserve(tilesWide * tilesHigh);

        srand(static_cast<unsigned>(time(nullptr)));

        for (int y = 0; y < tilesHigh; ++y)
        {
            for (int x = 0; x < tilesWide; ++x)
            {
                Lumina::QuadAttributes quad;
                quad.Texture = m_Atlas->GetTexture();
                quad.Size = { 0.02f, 0.02f };
                quad.Position = { x * quad.Size.x * 2, y * quad.Size.y * 2, 0.0f };
                int randomTile = rand() % 4;
                quad.TextureCoords = m_Atlas->GetTextureCoords(randomTile);
                m_Quads.push_back(quad);
            }
        }

		m_Camera.SetPosition({ 0.9f, 0.9f, -1.0f });
    }

    virtual void OnDetach() override {}

    virtual void OnUpdate(float ts) override
    {
        float elapsedTime = m_FrameTimer.Elapsed();
        m_FPS = 1.0f / elapsedTime;
        m_FrameTimer.Reset();

		m_Camera.HandleKeyInput(0.01f);
    }

    virtual void OnUIRender() override
    {
        ImGui::Begin("Example Window");
        ImGui::SetCursorPos({ 0.0f, 0.0f });

		glm::vec2 viewportSize = { 1000, 1000 };
        Lumina::Renderer::SetResolution(viewportSize.x, viewportSize.y);
		m_Camera.SetProjectionMatrix(1.0f * m_CameraZoom, viewportSize.x / viewportSize.y, 0.01f, 100.0f);

        Lumina::Renderer::Begin(m_Camera);

        m_Camera.HandleMouseInput(0.01f);

        for (const auto& quad : m_Quads)
        {
            Lumina::Renderer::DrawQuad(quad);
        }

        Lumina::Renderer::End();

        ImGui::Image((void*)(intptr_t)Lumina::Renderer::GetImage(), { viewportSize.x, viewportSize.y });
        ImGui::End();

        ImGui::Begin("Statistic");
        Lumina::Renderer::Statistics stats = Lumina::Renderer::GetStats();
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quad Count: %d", stats.QuadCount);
        ImGui::Text("Shaders Used: %d", stats.ShadersUsed);
        ImGui::Text("Textures Used: %d", stats.TexturesUsed);
        ImGui::Text("Data Size (Bytes): %d", stats.DataSize);
        Lumina::Renderer::ResetStats();
        ImGui::End();

        ImGui::Begin("FPS Counter");
        ImGui::Text("FPS: %.1f", m_FPS);
        ImGui::End();

        ImGui::Begin("Quad Parameters");

        if (ImGui::Button("Spawn New Quad"))
        {
            Lumina::QuadAttributes newQuad;
            newQuad.Texture = m_Atlas->GetTexture();
            newQuad.Position = { 0.0f, 0.0f, 0.0f };
            newQuad.Size = { 0.1f, 0.1f };
            newQuad.TintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
            newQuad.TextureCoords = m_Atlas->GetTextureCoords(0);
            m_Quads.push_back(newQuad);
            m_SelectedQuad = static_cast<int>(m_Quads.size()) - 1;
        }


        ImGui::Separator();
        ImGui::Text("Quad Selection:");
        for (int i = 0; i < m_Quads.size(); ++i) 
        {
            std::string label = "Quad " + std::to_string(i);
            if (ImGui::Selectable(label.c_str(), m_SelectedQuad == i)) 
            {
                m_SelectedQuad = i;
            }
        }
		ImGui::End();

		ImGui::Begin("Quad Editor");

        if (m_SelectedQuad >= 0 && m_SelectedQuad < m_Quads.size())
        {
            auto& quad = m_Quads[m_SelectedQuad];
            ImGui::Separator();
            ImGui::Text("Editing Quad %d", m_SelectedQuad);
            ImGui::DragFloat3("Position", &quad.Position.x, 0.01f);
            ImGui::DragFloat2("Size", &quad.Size.x, 0.01f);
            ImGui::ColorEdit4("Color", &quad.TintColor.r);
            ImGui::InputInt("Tile Index", &m_TileIndex);
            m_TileIndex = std::clamp(m_TileIndex, 0, m_Atlas->GetWidth() * m_Atlas->GetHeight() - 1);
            quad.TextureCoords = m_Atlas->GetTextureCoords(m_TileIndex);

            if (ImGui::Button("Delete Selected Quad")) 
            {
                m_Quads.erase(m_Quads.begin() + m_SelectedQuad);
                if (m_SelectedQuad >= m_Quads.size()) 
                {
                    m_SelectedQuad = static_cast<int>(m_Quads.size()) - 1;
                }
            }
        }

        ImGui::End();

        ImGui::Begin("Camera Parameters");
        ImGui::DragFloat("Camera Zoom", &m_CameraZoom, 0.01f);
		ImGui::End();
    }
private:
    Lumina::OrthographicCamera m_Camera;
	float m_CameraZoom = 10.0f;

    int m_SelectedQuad = 0; 
    std::vector<Lumina::QuadAttributes> m_Quads; 

    Lumina::Shared<Lumina::TextureAtlas> m_Atlas;
    Lumina::Timer m_FrameTimer;
    float m_FPS = 0.0f;

    int m_TileIndex = 0;
};
