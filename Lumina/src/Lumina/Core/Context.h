#pragma once

#include <GLFW/glfw3.h>

namespace Lumina
{

	class Context
	{
	public:
		virtual ~Context() = default;
		virtual void Init(GLFWwindow* window) = 0;
		virtual void PreRender() = 0;
		virtual void Render() = 0; 
		virtual void PostRender() = 0;
		virtual void Shutdown() = 0;
	};

}
