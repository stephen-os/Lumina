#pragma once

#include "Lumina/OpenGL/DepthBuffer.h"
#include "Lumina/OpenGL/Framebuffer.h"
#include "Lumina/OpenGL/Texture.h"
#include "Lumina/OpenGL/ShaderProgram.h"

#include "Lumina/Renderer/Camera.h"
#include "Lumina/Renderer/Model.h"

#include "World.h"

#include <vector>

class Renderer
{
public:
	Renderer();
	~Renderer() = default;

	// void InitWindow(const float width, const float height); 
	// void ShutDown(); 

	void Render(Camera& camera, GL::ShaderProgram& program);
	void SetViewportSize(const float width, const float height);

	unsigned int GetRendererID() { return m_Texture.GetID(); }
private:
	GL::DepthBuffer m_DepthBuffer;
	GL::FrameBuffer m_FrameBuffer;
	GL::Texture m_Texture;

	World m_World;

	float m_Width = 1;
	float m_Height = 1;
};