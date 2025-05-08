#include "ShaderProgram.h"

#include "Opengl/OpenGLShaderProgram.h"

#include "../Core/API.h"

namespace Lumina
{
	Shared<ShaderProgram> ShaderProgram::Create(const std::string& vertexShader, const std::string& fragmentShader)
	{
		switch (RendererAPI::GetAPI())
		{
			case API::OPENGL: return MakeShared<OpenGLShaderProgram>(vertexShader, fragmentShader);
			default: return nullptr;
		}
	}
}
