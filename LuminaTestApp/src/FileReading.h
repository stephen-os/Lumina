#include <vector>
#include <iostream>
#include <string>

#include "imgui.h"

#include "Lumina/Core/Layer.h"
#include "Lumina/Core/Aliases.h"
#include "Lumina/Core/Log.h"
#include "Lumina/Utils/Timer.h"
#include "Lumina/Renderer/Renderer.h"
#include "Lumina/Renderer/TextureAtlas.h"

#include "Lumina/Renderer/Cameras/OrthographicCamera.h"
#include "Lumina/Renderer/Cameras/PerspectiveCamera.h"

#include "Lumina/Utils/FileReader.h"
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Dev Dependencies
#include "Lumina/Core/Input.h"
#include "Lumina/Core/KeyCode.h"

class FileReading : public Lumina::Layer
{
public:
    virtual void OnAttach() override
    {

    }

    virtual void OnDetach() override {}

    virtual void OnUpdate(float ts) override
    {
    }

    virtual void OnUIRender() override
    {
        ImGui::Begin("Test Layer");
		ImGui::End(); 
    }
private:
};
