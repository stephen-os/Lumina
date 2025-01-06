#include <vector>
#include <iostream>

#include "imgui.h"

#include "Lumina/Layer.h"
#include "Lumina/Utils/Timer.h"
#include "Lumina/Renderer/Renderer.h"
#include "Lumina/Renderer/VertexArray.h"
#include "Lumina/Renderer/Buffer.h"
#include "Lumina/Renderer/ShaderProgram.h"
#include "Lumina/Renderer/FrameBuffer.h"
#include "Lumina/Renderer/RendererDebug.h"

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
        m_VertexArray = Lumina::CreateRef<Lumina::VertexArray>();

        // Create Vertex Buffer
        auto vertexBuffer = Lumina::CreateRef<Lumina::VertexBuffer>(vertices, sizeof(vertices));

        // Define the layout of the vertex data
        vertexBuffer->SetLayout({
            { Lumina::BufferDataType::Float3, "a_Position" },
            { Lumina::BufferDataType::Float3, "a_Color" }
            });

        // Add the vertex buffer to the vertex array
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        // Create Index Buffer
        auto indexBuffer = Lumina::CreateRef<Lumina::IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));
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

        m_FrameBuffer = Lumina::CreateRef<Lumina::FrameBuffer>();

        m_Shader = Lumina::CreateRef<Lumina::ShaderProgram>(vertexSrc, fragmentSrc);
    }

    virtual void OnDetach() override
    {
        // Cleanup if necessary
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
        m_FrameBuffer->Resize(viewportSize.x, viewportSize.y);

        GLCALL(glViewport(0, 0, viewportSize.x, viewportSize.y));
        GLCALL(glClearColor(1.0f, 0.3f, 0.3f, 1.0f));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GLCALL(glEnable(GL_DEPTH_TEST));
        GLCALL(glEnable(GL_CULL_FACE));

        m_FrameBuffer->Bind();

        // Bind shader and set uniforms (if any)
        m_Shader->Bind();

        // Draw the square

        m_VertexArray->Bind();

        const auto& attributes = m_VertexArray->GetAttributes();
        const auto& indexBuffer = attributes.GetIndexBuffer();
        if (indexBuffer)
        {
            glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
        }

        ImGui::Image((void*)(intptr_t)m_FrameBuffer->GetColorAttachment(), viewportSize);

        m_FrameBuffer->Unbind();

        // End rendering
        ImGui::End();

        ImGui::Begin("FPS Counter");
        ImGui::Text("FPS: %.1f", m_FPS);
        ImGui::End();
    }

private:
    Lumina::Ref<Lumina::VertexArray> m_VertexArray;
    Lumina::Ref<Lumina::ShaderProgram> m_Shader;
    Lumina::Ref<Lumina::FrameBuffer> m_FrameBuffer;
    Lumina::Timer m_FrameTimer;
    float m_FPS = 0.0f;
};