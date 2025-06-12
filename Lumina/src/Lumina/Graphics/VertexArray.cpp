#include "VertexArray.h"

#include "Opengl/OpenGLVertexArray.h"

#include "../Core/API.h"

namespace Lumina
{
	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case API::OPENGL: return std::make_shared<OpenGLVertexArray>();
			default: return nullptr; 
		}
	}
}