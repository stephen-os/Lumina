#include "FrameBuffer.h"

#include "OpenGL/OpenGLFrameBuffer.h"

#include "../Core/API.h"

namespace Lumina
{
	Shared<FrameBuffer> FrameBuffer::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case API::OPENGL: return MakeShared<OpenGLFrameBuffer>(); 
			case API::VULKAN: return nullptr;
			default: return nullptr; 
		}
	}
}
