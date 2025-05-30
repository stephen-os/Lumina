#include "ComputeShader.h"

#include "OpenGL/OpenGLComputeShader.h"

#include "../Core/API.h"

namespace Lumina
{
	Shared<ComputeShader> ComputeShader::Create(const std::string& source)
	{
		switch (RendererAPI::GetAPI())
		{
		case API::OPENGL: return MakeShared<OpenGLComputeShader>(source);
		default: return nullptr;
		}
	}
}
