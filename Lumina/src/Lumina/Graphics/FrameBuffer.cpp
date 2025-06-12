#include "FrameBuffer.h"

#include "OpenGL/OpenGLFrameBuffer.h"

#include "../Core/API.h"

namespace Lumina
{
	std::shared_ptr<FrameBuffer> FrameBuffer::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case API::OPENGL: return std::make_shared<OpenGLFrameBuffer>(); 
			default: return nullptr; 
		}
	}
}
