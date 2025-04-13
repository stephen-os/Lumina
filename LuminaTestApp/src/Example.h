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

class Example : public Lumina::Layer
{
public:
    virtual void OnAttach() override
    {
        Lumina::Renderer::Init();

        std::string source = "res/texture/factory_atlas.png";
        m_Atlas = Lumina::MakeShared<Lumina::TextureAtlas>(source, 16, 16);

		// Create a shader program
		std::string vertexShaderSource = Lumina::ReadFile("res/shaders/Background.vert");
		std::string fragmentShaderSource = Lumina::ReadFile("res/shaders/Background.frag");
		Lumina::Shared<Lumina::ShaderProgram> shader = Lumina::ShaderProgram::Create(vertexShaderSource, fragmentShaderSource);

		m_BackgroundAttributes.Shader = shader;
        // Positive direction moves away from the camera
		m_BackgroundAttributes.Position = { 0.0f, 0.0f, 0.0f };

		m_QuadAttributes.Texture = m_Atlas->GetTexture();
		m_QuadAttributes.Position = { 0.02f, 0.02f, 0.0f };
		m_QuadAttributes.Size = { 0.02f, 0.02f };

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

        // ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		glm::vec2 viewportSize = { 1000, 1000 };
        Lumina::Renderer::SetResolution(viewportSize.x, viewportSize.y);
		m_Camera.SetProjectionMatrix(1.0f * m_CameraZoom, viewportSize.x / viewportSize.y, 0.01f, 100.0f);

        Lumina::Renderer::Begin(m_Camera);

        m_Camera.HandleMouseInput(0.01f);

        m_BackgroundAttributes.Shader->Bind();
        m_BackgroundAttributes.Shader->SetUniformVec2("u_GridSize", { 100, 100 });
        m_BackgroundAttributes.Shader->Unbind(); 

		Lumina::Renderer::DrawQuad(m_BackgroundAttributes);
        Lumina::Renderer::DrawQuad(m_QuadAttributes);
        Lumina::Renderer::End();

        ImGui::Image((void*)(intptr_t)Lumina::Renderer::GetImage(), { viewportSize.x, viewportSize.y });
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

        ImGui::Begin("Quad Parameters");
        ImGui::DragFloat3("Position", &m_QuadAttributes.Position.x, 0.01f);
        ImGui::DragFloat2("Size", &m_QuadAttributes.Size.x, 0.01f);
        ImGui::ColorEdit4("Color", &m_QuadAttributes.TintColor.r);
        ImGui::InputInt("Tile Index", &m_TileIndex);
		m_QuadAttributes.TextureCoords = m_Atlas->GetTextureCoords(m_TileIndex);
        if (m_TileIndex < 0) m_TileIndex = 0;
        int maxIndex = m_Atlas->GetWidth() * m_Atlas->GetHeight() - 1;
        if (m_TileIndex > maxIndex) m_TileIndex = maxIndex;
        ImGui::End();

        ImGui::Begin("Camera Parameters");
        ImGui::DragFloat("Camera Zoom", &m_CameraZoom, 0.01f);
		ImGui::End();
    }
private:
    Lumina::OrthographicCamera m_Camera;
	float m_CameraZoom = 10.0f;

    Lumina::Shared<Lumina::TextureAtlas> m_Atlas;
    Lumina::Timer m_FrameTimer;
    float m_FPS = 0.0f;

    Lumina::QuadAttributes m_QuadAttributes;
	Lumina::QuadAttributes m_BackgroundAttributes;
    int m_TileIndex = 0;
};
