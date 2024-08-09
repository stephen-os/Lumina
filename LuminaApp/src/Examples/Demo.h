#include "Lumina/Application.h"

#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Lumina/Utils/Camera.h"

#include "Lumina/OpenGL/GLUtils.h"

#include "Lumina/OpenGL/ShaderProgram.h"
#include "Lumina/OpenGL/FrameBuffer.h"
#include "Lumina/OpenGL/Texture.h"
#include "Lumina/OpenGL/IndexBuffer.h"
#include "Lumina/OpenGL/VertexBuffer.h"

#include "Lumina/OpenGL/VertexAttributes.h"
#include "Lumina/OpenGL/VertexArray.h"

class Demo : public Lumina::Layer
{
public:
    Demo()
        : m_Camera(45.0f, m_Width / m_Height, 0.1f, 100.0f)
    {
        m_Camera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    }

    virtual void OnUIRender() override
    {
        glm::mat4 mvp = m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix() * glm::mat4(1.0f);

        ImGui::Begin("Demo Window");

        auto viewportSize = ImGui::GetContentRegionAvail();
        m_Width = viewportSize.x;
        m_Height = viewportSize.y;

        // Texture
        m_Texture->Bind(); 
        m_Texture->SetData(m_Width, m_Height);

        // Frame Buffer
        m_FrameBuffer->Bind();
        m_FrameBuffer->AttachTexture(m_Texture->GetID());

        GLCALL(glViewport(0, 0, (GLsizei)m_Width, (GLsizei)m_Height));
        GLCALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GLCALL(glEnable(GL_DEPTH_TEST));
        GLCALL(glEnable(GL_CULL_FACE));

        // Shader
        m_ShaderProgram->Bind();
        m_ShaderProgram->SetUniformMatrix4fv("u_MVP", mvp);

        // Vertex Array
        m_VertexArray->Bind();
        m_VertexArray->DrawIndexed(GL_TRIANGLES);
        m_VertexArray->Unbind();

        ImGui::Image((void*)(intptr_t)m_Texture->GetID(), ImVec2(m_Width, m_Height));
        ImGui::End();

        // Cleanup
        m_Texture->Unbind();
        m_FrameBuffer->Unbind();
    }

    virtual void OnAttach() override
    {
        float vertices[12] = {
            -1.0f, -1.0f,
             1.0f, -1.0f,
             1.0f,  1.0f,
            -1.0f,  1.0f,
            -1.0f, -1.0f,
             1.0f,  1.0f
        };

        float positions[12]{
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
        };

        float colors[12]{
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
            0.0, 1.0, 0.0
        };

        unsigned int indices[6]{
            0, 1, 2,
            2, 3, 0
        };

        int stride = 3;
        m_VertexAttributes = new GL::VertexAttributes();
        m_VertexAttributes->AddVertices("a_Position", 12, stride, positions);
        m_VertexAttributes->AddVertices("a_Color", 12, stride, colors);
        m_VertexAttributes->AddIndices(indices, 6);

        std::string vertexShader = ReadFile("res/shaders/color.vert");
        std::string fragmentShader = ReadFile("res/shaders/color.frag");

        m_ShaderProgram = new GL::ShaderProgram(vertexShader, fragmentShader);

        m_VertexArray = new GL::VertexArray(*m_ShaderProgram, *m_VertexAttributes);

        m_Texture = new GL::Texture();
        m_FrameBuffer = new GL::FrameBuffer();
    }

private:
    std::string ReadFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "Error: Failed to open file: " << filename << std::endl;
            throw std::runtime_error("failed to open file!");
        }

        std::streampos fileSize = file.tellg();
        size_t size = static_cast<size_t>(fileSize);
        std::string buffer(size, '\0');

        file.seekg(0);
        file.read(&buffer[0], size);
        file.close();

        return buffer;
    }
private:
    GL::ShaderProgram* m_ShaderProgram = nullptr;
    GL::FrameBuffer* m_FrameBuffer = nullptr;
    GL::Texture* m_Texture = nullptr;
    GL::VertexAttributes* m_VertexAttributes = nullptr;
    GL::VertexArray* m_VertexArray = nullptr;

    float m_Width = 900;
    float m_Height = 900;

    Camera m_Camera;
};