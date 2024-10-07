#include <vector>
#include <iostream>

#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Lumina/Utils/Camera.h"
#include "Lumina/Utils/Timer.h"

#include "Lumina/OpenGL/GLUtils.h"

#include "Lumina/OpenGL/ShaderProgram.h"
#include "Lumina/OpenGL/FrameBuffer.h"
#include "Lumina/OpenGL/Texture.h"
#include "Lumina/OpenGL/VertexAttributes.h"
#include "Lumina/OpenGL/VertexArray.h"

#include "Lumina/Renderer/Mesh.h"

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
        m_Camera.SetPosition(glm::vec3(0.0f, 25.0f, 0.0f));
        m_Camera.Pitch(-90.0f); 
    }

    virtual void OnUpdate(float timestep) override
    {
        m_Camera.HandleKeyInput(0.1f); 

        float elapsedTime = m_FrameTimer.Elapsed();
        m_FPS = 1.0f / elapsedTime;
        m_FrameTimer.Reset();
    }

    virtual void OnUIRender() override
    {
        ImGui::Begin("Side Window");
        
        m_Camera.HandleMouseInput(0.1f); 

        auto viewportSize = ImGui::GetContentRegionAvail();
        m_Width = viewportSize.x;
        m_Height = viewportSize.y;
        m_Camera.SetProjectionMatrix(45.0f, m_Width / m_Height, 0.1f, 100.0f);

        // Texture
        m_Texture->Bind();
        m_Texture->SetData(m_Width, m_Height);

        // Frame Buffer
        m_FrameBuffer->Bind();
        m_FrameBuffer->AttachTexture(m_Texture->GetID());

        GLCALL(glViewport(0, 0, (GLsizei)m_Width, (GLsizei)m_Height));
        GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GLCALL(glEnable(GL_CULL_FACE));

        GLCALL(glEnable(GL_DEPTH_TEST));
        glDepthFunc(GL_LEQUAL);

        m_Mesh.AttachProjection(m_Camera.GetProjectionMatrix()); 
        m_Mesh.AttachView(m_Camera.GetViewMatrix());
        m_Mesh.Draw(*m_ShaderProgram);

        ImGui::Image((void*)(intptr_t)m_Texture->GetID(), ImVec2(m_Width, m_Height));
        ImGui::End();

        ImGui::Begin("FPS Counter");
        ImGui::Text("FPS: %.1f", m_FPS);
        ImGui::End();

        // Cleanup
        m_Texture->Unbind();
        m_FrameBuffer->Unbind();
    }

    virtual void OnAttach() override
    {
        std::string vertexShader = ReadFile("res/shaders/normal.vert");
        std::string fragmentShader = ReadFile("res/shaders/normal.frag");

        m_ShaderProgram = new GL::ShaderProgram(vertexShader, fragmentShader);
        m_Mesh.AttachShader(*m_ShaderProgram);

        m_Texture = new GL::Texture();
        m_FrameBuffer = new GL::FrameBuffer();
        m_FrameBuffer->AttachDepthBuffer(m_Width, m_Height);
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
    GL::Texture* m_Texture = nullptr;;

    float m_Width = 900;
    float m_Height = 900;

    // Timer
    Lumina::Timer m_FrameTimer;
    float m_FPS;

    Camera m_Camera;

    Mesh m_Mesh = Mesh("res/gltf/multiple_boxes.gltf");
};