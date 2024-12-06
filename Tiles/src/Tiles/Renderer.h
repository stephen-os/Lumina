#pragma once

#include "Lumina/OpenGL/DepthBuffer.h"
#include "Lumina/OpenGL/Framebuffer.h"
#include "Lumina/OpenGL/Texture.h"
#include "Lumina/OpenGL/ShaderProgram.h"

#include "Lumina/Renderer/Camera.h"
#include "Lumina/Renderer/Model.h"

#include "TileObject.h"

#include <vector>

class Renderer
{
public:
	Renderer();
	~Renderer() = default;

	void Render(Camera& camera, std::vector<glm::mat4>& transforms, std::vector<glm::vec2>& offsets, GL::ShaderProgram& program);
	void SetViewportSize(const float width, const float height);

	unsigned int GetRendererID() { return m_Texture.GetID(); }
private:
	GL::DepthBuffer m_DepthBuffer;
	GL::FrameBuffer m_FrameBuffer;
	GL::Texture m_Texture;

	TileObject m_TileObject;

	float m_Width = 1;
	float m_Height = 1;
};