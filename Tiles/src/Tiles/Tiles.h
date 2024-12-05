#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Lumina/Layer.h"

#include "Lumina/Utils/Timer.h"
#include "Lumina/Utils/FileReader.h"

#include "Lumina/OpenGL/GLUtils.h"
#include "Lumina/OpenGL/ShaderProgram.h"
#include "Lumina/OpenGL/FrameBuffer.h"
#include "Lumina/OpenGL/Texture.h"
#include "Lumina/OpenGL/VertexAttributes.h"
#include "Lumina/OpenGL/VertexArray.h"

#include "Lumina/Renderer/Camera.h"

// Client
#include "TileEditor.h"
#include "Renderer.h"

class Tiles : public Lumina::Layer
{
public:
    Tiles()
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

        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        m_Camera.SetProjectionMatrix(45.0f, viewportSize.x / viewportSize.y, 0.1f, 100.0f);

        // m_Renderer.SetViewportSize(viewportSize.x, viewportSize.y);
        // m_Renderer.Render(m_Camera, m_TileEditor.GetMatrices(), m_ShaderProgram);

        ImGui::Image((void*)(intptr_t)m_Renderer.GetRendererID(), ImVec2(viewportSize.x, viewportSize.y));
        ImGui::End();

        ImGui::Begin("FPS Counter");
        ImGui::Text("FPS: %.1f", m_FPS);
        ImGui::End(); 
    }

    virtual void OnAttach() override
    {   
        m_TileEditor.InitEditor(20, 20);

        // m_Renderer.InitWindow(900, 900); 

        std::string vertex = Lumina::ReadFile("res/shaders/world.vert");
        std::string fragment = Lumina::ReadFile("res/shaders/world.frag");
        m_ShaderProgram.SetSource(vertex, fragment); 
    }

    virtual void OnDetach() override
    {
        m_ShaderProgram.Destroy(); 
    }
private:
    Renderer m_Renderer;
    TileEditor m_TileEditor; 

    Lumina::Timer m_FrameTimer;
    float m_FPS = 0;
    
    Camera m_Camera;

    GL::ShaderProgram m_ShaderProgram; 
};