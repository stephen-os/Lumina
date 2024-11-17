#pragma once

#include "Mesh.h"

#include "../OpenGL/DepthBuffer.h"
#include "../OpenGL/Framebuffer.h"
#include "../OpenGL/Texture.h"
#include "../OpenGL/ShaderProgram.h"

#include "Camera.h"
#include "Model.h"

#include <vector>

class Renderer
{
public: 
	Renderer(); 
	~Renderer() = default; 

	void Render(Camera& camera, std::vector<Model>& models, GL::ShaderProgram& program);
	void SetViewportSize(const float width, const float height); 

	unsigned int GetRendererID() { return m_Texture.GetID(); }
private:
	GL::DepthBuffer m_DepthBuffer; 
	GL::FrameBuffer m_FrameBuffer;
	GL::Texture m_Texture;

	float m_Width = 1920;
	float m_Height = 1080;
};