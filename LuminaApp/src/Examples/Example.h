#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Lumina/Utils/Timer.h"
#include "Lumina/Utils/FileReader.h"

#include "Lumina/OpenGL/GLUtils.h"

#include "Lumina/OpenGL/ShaderProgram.h"
#include "Lumina/OpenGL/FrameBuffer.h"
#include "Lumina/OpenGL/Texture.h"
#include "Lumina/OpenGL/VertexAttributes.h"
#include "Lumina/OpenGL/VertexArray.h"

#include "Lumina/Renderer/Camera.h"
#include "Lumina/Renderer/Renderer.h"
#include "Lumina/Renderer/Model.h"

#include "Lumina/Windows/TileEditor.h"

class Example : public Lumina::Layer
{
public:
    Example()
        : m_Camera(45.0f, m_Width / m_Height, 0.1f, 100.0f),
          m_TileEditor(10, 10, 1)
    {
        m_Camera.SetPosition(glm::vec3(0.0f, 0.0f, 25.0f));
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
        m_TileEditor.Render(); 

        ImGui::Begin("Side Window");
        
        m_Camera.HandleMouseInput(0.1f); 

        auto viewportSize = ImGui::GetContentRegionAvail();
        m_Width = viewportSize.x;
        m_Height = viewportSize.y;
        m_Camera.SetProjectionMatrix(45.0f, m_Width / m_Height, 0.1f, 100.0f);

        m_Renderer.SetViewportSize(m_Width, m_Height);
        m_Renderer.Render(m_Camera, m_Cube, m_ShaderProgram);

        ImGui::Image((void*)(intptr_t)m_Renderer.GetRendererID(), ImVec2(m_Width, m_Height));
        ImGui::End();

        ImGui::Begin("FPS Counter");
        ImGui::Text("FPS: %.1f", m_FPS);
        ImGui::End(); 
    }

    virtual void OnAttach() override
    {   
        std::string vertex = Lumina::ReadFile("res/shaders/instanced.vert");
        std::string fragment = Lumina::ReadFile("res/shaders/instanced.frag");
        m_ShaderProgram.SetSource(vertex, fragment); 
    }

    virtual void OnDetach() override
    {
        
    }
private:
    Renderer m_Renderer;

    GL::ShaderProgram m_ShaderProgram; 

    Cube m_Cube; 

    float m_Width = 900;
    float m_Height = 900;

    Lumina::Timer m_FrameTimer;
    float m_FPS;

    Camera m_Camera;

    Lumina::TileEditor m_TileEditor; 
};