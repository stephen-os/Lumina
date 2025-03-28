#include <vector>
#include <iostream>
#include <string>

#include "imgui.h"
#include "ImGuiFileDialog.h"

#include "Lumina/Core/Layer.h"
#include "Lumina/Utils/Timer.h"
#include "Lumina/Renderer/Renderer.h"
#include "Lumina/Renderer/VertexArray.h"
#include "Lumina/Renderer/Buffer.h"
#include "Lumina/Renderer/ShaderProgram.h"
#include "Lumina/Renderer/FrameBuffer.h"
#include "Lumina/Renderer/RendererDebug.h"

#include "spdlog/spdlog.h"

class Dialog : public Lumina::Layer
{
public:
    virtual void OnAttach() override
    {
        spdlog::info("Welcome to spdlog!");

        float vertices[] = {
            // Positions       // Colors
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // Bottom-left
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom-right
             0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // Top-right
            -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f   // Top-left
        };

        uint32_t indices[] = {
            0, 1, 2,  // First triangle
            2, 3, 0   // Second triangle
        };

        m_VertexArray = Lumina::MakeShared<Lumina::VertexArray>();
        auto vertexBuffer = Lumina::MakeShared<Lumina::VertexBuffer>(vertices, sizeof(vertices));

        vertexBuffer->SetLayout({
            { Lumina::BufferDataType::Float3, "a_Position" },
            { Lumina::BufferDataType::Float3, "a_Color" }
            });

        m_VertexArray->AddVertexBuffer(vertexBuffer);
        auto indexBuffer = Lumina::MakeShared<Lumina::IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        const std::string vertexSrc = R"(
            #version 330 core
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec3 a_Color;

            out vec3 v_Color;

            void main()
            {
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0);
            }
        )";

        const std::string fragmentSrc = R"(
            #version 330 core
            in vec3 v_Color;
            out vec4 FragColor;

            void main()
            {
                FragColor = vec4(v_Color, 1.0);
            }
        )";

        m_Shader = Lumina::MakeShared<Lumina::ShaderProgram>(vertexSrc, fragmentSrc);
        Lumina::Renderer::Init();
    }

    virtual void OnDetach() override
    {
        Lumina::Renderer::Shutdown(); 
    }

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
        Lumina::Renderer::Begin();
        Lumina::Renderer::OnWindowResize(viewportSize.x, viewportSize.y);
        Lumina::Renderer::Clear();
        Lumina::Renderer::ClearColor(0.1f, 0.1f, 0.1f);
        Lumina::Renderer::Enable(Lumina::State::DEPTH_TEST);
        Lumina::Renderer::Enable(Lumina::State::CULL_FACE);

        m_Shader->Bind();
        Lumina::Renderer::Draw(m_VertexArray);
        ImGui::Image((void*)(intptr_t)Lumina::Renderer::GetID(), viewportSize);
        Lumina::Renderer::End();
        ImGui::End();

        ImGui::Begin("File Dialog");

        if (ImGui::Button("Open File"))
        {
            IGFD::FileDialogConfig config;
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlg", "Choose File", ".cpp,.h,.txt", config);
        }

        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlg"))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                m_SelectedFile = ImGuiFileDialog::Instance()->GetFilePathName();
            }
            ImGuiFileDialog::Instance()->Close();
        }

        ImGui::Text("Selected File: %s", m_SelectedFile.c_str());
        ImGui::End();

        ImGui::Begin("FPS Counter");
        ImGui::Text("FPS: %.1f", m_FPS);
        ImGui::End();
    }

private:
    Lumina::Shared<Lumina::VertexArray> m_VertexArray;
    Lumina::Shared<Lumina::ShaderProgram> m_Shader;
    Lumina::Timer m_FrameTimer;
    float m_FPS = 0.0f;
    std::string m_SelectedFile;
};
