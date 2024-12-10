#pragma once

#include "Lumina/OpenGL/DepthBuffer.h"
#include "Lumina/OpenGL/Framebuffer.h"
#include "Lumina/OpenGL/Texture.h"
#include "Lumina/OpenGL/ShaderProgram.h"

#include "Lumina/Renderer/OrthographicCamera.h"
#include "Lumina/Renderer/Model.h"

#include "TileObject.h"
#include "InfiniteGrid.h"

#include <vector>

#include <glm/glm.hpp>

class TileRenderer
{
public:
	TileRenderer();
	~TileRenderer();

	void Render(std::vector<glm::mat4>& transforms, std::vector<glm::vec2>& offsets);
	void SetViewportSize(const float width, const float height);

	glm::vec2 GetViewportSize() const { return glm::vec2(m_Width, m_Height); }

	unsigned int GetRendererID() { return m_Texture.GetID(); }
private:
	GL::DepthBuffer m_DepthBuffer;
	GL::FrameBuffer m_FrameBuffer;
	GL::Texture m_Texture;

	GL::ShaderProgram m_TileShader;
	GL::ShaderProgram m_GridShader; 

	TileObject m_TileObject;
	InfiniteGrid m_Grid;

	OrthographicCamera m_Camera;

	float m_Width = 1;
	float m_Height = 1;
};