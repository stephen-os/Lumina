#pragma once

#include "Lumina/OpenGL/DepthBuffer.h"
#include "Lumina/OpenGL/Framebuffer.h"
#include "Lumina/OpenGL/Texture.h"
#include "Lumina/OpenGL/ShaderProgram.h"

#include "Lumina/Renderer/Camera.h"
#include "Lumina/Renderer/Model.h"

#include "Lumina/Geometry/Cube.h"

#include <vector>

// We need a solution to dealing with the renderer
// Some how I would like to abstract away the setup from the user and just have the user write what needs
// to be drawn. This also needs to be reusable, for multiple layers. 

class Renderer
{
public:
	Renderer();
	~Renderer() = default;

	// void InitWindow(const float width, const float height); 
	// void ShutDown(); 

	void Render(Camera& camera, std::vector<glm::mat4>& transforms, GL::ShaderProgram& program);
	void SetViewportSize(const float width, const float height);

	unsigned int GetRendererID() { return m_Texture.GetID(); }
private:
	GL::DepthBuffer m_DepthBuffer;
	GL::FrameBuffer m_FrameBuffer;
	GL::Texture m_Texture;

	Cube cube;

	float m_Width = 1;
	float m_Height = 1;
};