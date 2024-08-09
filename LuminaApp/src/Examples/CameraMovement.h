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
#include "Lumina/OpenGL/VertexAttributes.h"
#include "Lumina/OpenGL/VertexArray.h"

#define KEY_W GLFW_KEY_W
#define KEY_S GLFW_KEY_S

#define KEY_D GLFW_KEY_D
#define KEY_A GLFW_KEY_A

#define KEY_ESC GLFW_KEY_ESCAPE

class CameraMovement : public Lumina::Layer
{
public:
    CameraMovement()
        : m_Camera(45.0f, m_Width / m_Height, 0.1f, 100.0f)
    {
        m_Camera.SetPosition(glm::vec3(-3.0f, -3.0f, 3.0f));
        m_Camera.Yaw(-45.0f);
        m_Camera.Pitch(35.0f);
    }

    virtual void OnUpdate(float timestep) override
    {
        if (ImGui::IsKeyPressed((ImGuiKey)KEY_W))
            m_Forward = true;
        if (ImGui::IsKeyPressed((ImGuiKey)KEY_S))
            m_Backward = true; 
        if (ImGui::IsKeyPressed((ImGuiKey)KEY_D))
            m_Right = true;
        if (ImGui::IsKeyPressed((ImGuiKey)KEY_A))
            m_Left = true;

        if (ImGui::IsKeyPressed((ImGuiKey)KEY_ESC))
            glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
        
        if (ImGui::IsKeyReleased((ImGuiKey)KEY_W))
            m_Forward = false;
        if (ImGui::IsKeyReleased((ImGuiKey)KEY_S))
            m_Backward = false;
        if (ImGui::IsKeyReleased((ImGuiKey)KEY_D))
            m_Right = false;
        if (ImGui::IsKeyReleased((ImGuiKey)KEY_A))
            m_Left = false;

        if (m_Forward)
            m_Camera.Advance(0.1f);
        if (m_Backward)
            m_Camera.Advance(-0.1f);
        if (m_Right)
            m_Camera.Strafe(0.1f);
        if (m_Left)
            m_Camera.Strafe(-0.1f);
    }

    virtual void OnUIRender() override
    {
        glm::mat4 mvp = m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix() * glm::mat4(1.0f);

        ImGui::Begin("Side Window");
        
        // Camera Mouse
        ImGuiIO& io = ImGui::GetIO();

        static bool firstMouse = true;
        static float lastX = io.MousePos.x;
        static float lastY = io.MousePos.y;

        bool isWindowFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

        if (isWindowFocused)
        {
            io.MouseDrawCursor = false;
        }
        else
        {
            io.MouseDrawCursor = true;
        }

        if (isWindowFocused)
        {
            if (firstMouse)
            {
                lastX = io.MousePos.x;
                lastY = io.MousePos.y;
                firstMouse = false;
            }

            float xOffset = io.MousePos.x - lastX;
            float yOffset = lastY - io.MousePos.y;

            lastX = io.MousePos.x;
            lastY = io.MousePos.y;

            float sensitivity = 0.1f;
            xOffset *= sensitivity;
            yOffset *= sensitivity;

            m_Camera.Yaw(-xOffset);
            m_Camera.Pitch(-yOffset);
        }
        // End Camera Mouse

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

    // Input Control 
    bool m_Forward = false; 
    bool m_Backward = false;
    bool m_Left = false;
    bool m_Right = false;

    float m_Width = 900;
    float m_Height = 900;

    Camera m_Camera;
};