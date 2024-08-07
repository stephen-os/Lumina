#include "Lumina/Application.h"

#include <vector>
#include <iostream>

#include <fstream>

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

class Cube : public Lumina::Layer
{
public:
    Cube()
        : m_Camera(45.0f, m_Width / m_Height, 0.1f, 100.0f)
    {
        m_Camera.SetPosition(glm::vec3(-3.0f, -3.0f, 3.0f));
        m_Camera.Yaw(-45.0f);
        m_Camera.Pitch(35.0f);
    }

    virtual void OnUIRender() override
    {
        glm::mat4 mvp = m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix() * glm::mat4(1.0f);

        ImGui::Begin("Side Window");

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
        float positions[72]{
            // Front face
            -0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,

            // Back face
            -0.5f, -0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,

            // Left face
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f,

            // Right face
            0.5f, -0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,

            // Top face
            -0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, -0.5f,

            // Bottom face
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f
        };

        float normals[72]{
            // Front face
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,

            // Back face
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,

            // Left face
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,

            // Right face
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            // Top face
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            // Bottom face
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f
        };

        unsigned int indices[36]{
            // Front face
            0, 1, 2,
            2, 3, 0,

            // Back face
            4, 5, 6,
            6, 7, 4,

            // Left face
            8, 9, 10,
            10, 11, 8,

            // Right face
            12, 13, 14,
            14, 15, 12,

            // Top face
            16, 17, 18,
            18, 19, 16,

            // Bottom face
            20, 21, 22,
            22, 23, 20
        };

        int stride = 3;
        m_VertexAttributes = new GL::VertexAttributes();
        m_VertexAttributes->AddVertices("a_Position", 72, stride, positions);
        m_VertexAttributes->AddVertices("a_Normal", 72, stride, normals);
        m_VertexAttributes->AddIndices(indices, 36);

        std::string vertexShader = ReadFile("res/shaders/lighting.vert");
        std::string fragmentShader = ReadFile("res/shaders/lighting.frag");

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