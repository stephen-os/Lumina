#include <vector>
#include <iostream>

#include "imgui.h"

#include "Lumina/Core/Layer.h"
#include "Lumina/Core/Aliases.h"
#include "Lumina/Utils/Timer.h"
#include "Lumina/Renderer/VertexArray.h"
#include "Lumina/Renderer/Buffer.h"
#include "Lumina/Renderer/ShaderProgram.h"
#include "Lumina/Renderer/FrameBuffer.h"

class Example : public Lumina::Layer
{
public:
    virtual void OnAttach() override
    {      
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

        // Create Vertex Array
        m_VertexArray = Lumina::VertexArray::Create();

        // Create Vertex Buffer
        auto vertexBuffer = Lumina::VertexBuffer::Create(vertices, sizeof(vertices));

        // Define the layout of the vertex data
        vertexBuffer->SetLayout({
            { Lumina::BufferDataType::Float3, "a_Position" },
            { Lumina::BufferDataType::Float3, "a_Color" }
            });

        // Add the vertex buffer to the vertex array
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        // Create Index Buffer
        auto indexBuffer = Lumina::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        // Create Shader
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

        m_Shader = Lumina::ShaderProgram::Create(vertexSrc, fragmentSrc);

        m_FrameBuffer = Lumina::FrameBuffer::Create(); 
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
        // Begin rendering
        ImGui::Begin("Example Window");
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        
        // Begin
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_FrameBuffer->Bind();

        // Resize
        glViewport(0, 0, viewportSize.x, viewportSize.y);
        m_FrameBuffer->Resize(viewportSize.x, viewportSize.y);

        // Clear 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Clear Color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        // Enable
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        
        // Bind Shader
        m_Shader->Bind();

        // Draw
        m_VertexArray->Bind();
        m_VertexArray->DrawIndexed();
        m_VertexArray->Unbind(); 

        // Present Image
        ImGui::Image((void*)(intptr_t)m_FrameBuffer->GetColorAttachment(), viewportSize);

        // Unbind Framebuffer
        m_FrameBuffer->Unbind(); 

        // End rendering
        ImGui::End();

        // FPS Window
        ImGui::Begin("FPS Counter");
        ImGui::Text("FPS: %.1f", m_FPS);
        ImGui::End();
    }

private:
    Lumina::Shared<Lumina::VertexArray> m_VertexArray;
    Lumina::Shared<Lumina::ShaderProgram> m_Shader;
    Lumina::Shared<Lumina::FrameBuffer> m_FrameBuffer; 
    Lumina::Timer m_FrameTimer;
    float m_FPS = 0.0f;
};