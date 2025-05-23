#include "VertexArray.h"

#include "Opengl/OpenGLVertexArray.h"

#include "../Core/API.h"

namespace Lumina
{
	Shared<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case API::OPENGL: return MakeShared<OpenGLVertexArray>();
			default: return nullptr; 
		}
	}
}