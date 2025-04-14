#include "Commands.h"

#include "../Core/API.h"

#include "OpenGL/OpenGLCommands.h"

namespace Lumina
{
	Shared<Commands> Commands::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case API::OPENGL: return MakeShared<OpenGLCommands>();
			case API::VULKAN: return nullptr;
			default: return nullptr;
		}
	}
} 