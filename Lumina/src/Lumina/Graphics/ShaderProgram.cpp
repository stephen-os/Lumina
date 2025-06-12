#include "ShaderProgram.h"

#include "Opengl/OpenGLShaderProgram.h"

#include "../Core/API.h"

namespace Lumina
{
	std::shared_ptr<ShaderProgram> ShaderProgram::Create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (RendererAPI::GetAPI())
		{
		case API::OPENGL: return std::make_shared<OpenGLShaderProgram>(vertexSource, fragmentSource);
		default: return nullptr;
		}
	}
}
