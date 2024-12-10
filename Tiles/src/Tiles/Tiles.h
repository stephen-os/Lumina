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

// Client
#include "TileEditor.h"
#include "TileRenderer.h"

class Tiles : public Lumina::Layer
{
public:
    virtual void OnUpdate(float timestep) override
    {
        // m_Camera.HandleKeyInput(0.1f);
        float elapsedTime = m_FrameTimer.Elapsed();
        m_FPS = 1.0f / elapsedTime;
        m_FrameTimer.Reset();
    }

    virtual void OnUIRender() override
    {
        m_TileEditor.Render(); 

        glm::vec2 viewportSize = m_TileRenderer.GetViewportSize();

        m_TileRenderer.Render(m_TileEditor.GetMatrices(), m_TileEditor.GetOffsets());

        ImGui::Begin("FPS Counter");
        ImGui::Text("FPS: %.1f", m_FPS);
        ImGui::End(); 
    }

    virtual void OnAttach() override
    {   
    }

    virtual void OnDetach() override
    {
        m_TileEditor.Shutdown(); 
    }
private:
    TileEditor m_TileEditor; 
    TileRenderer m_TileRenderer;

    float m_Zoom = 90.0f;

    Lumina::Timer m_FrameTimer;
    float m_FPS = 0;
};