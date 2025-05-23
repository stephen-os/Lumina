#include "ShaderProgram.h"

#include "Opengl/OpenGLShaderProgram.h"

#include "../Core/API.h"

namespace Lumina
{
	Shared<ShaderProgram> ShaderProgram::Create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (RendererAPI::GetAPI())
		{
			case API::OPENGL: return MakeShared<OpenGLShaderProgram>(vertexSource, fragmentSource);
			default: return nullptr;
		}
	}
}
