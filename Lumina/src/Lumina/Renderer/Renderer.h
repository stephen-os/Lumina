#pragma once

#include "Mesh.h"

#include "../OpenGL/Framebuffer.h"
#include "../OpenGL/Texture.h"

#include <vector>

class Renderer
{
public: 
	Renderer(); 
	~Renderer(); 

	void Render(const std::vector<Mesh>& meshes) const;

private:
	GL::FrameBuffer m_FrameBuffer;
	GL::Texture m_Texture;
};