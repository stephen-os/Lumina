#pragma once

#include <GLFW/glfw3.h>

#include "../../Core/Context.h"

namespace Lumina
{

	class OpenGLContext : public Context
	{
	public: 
		void Init(GLFWwindow* window) override;
		void PreRender() override;
		void Render() override;
		void PostRender() override;
		void Shutdown() override;
	private:
		GLFWwindow* m_Window = nullptr;
	};

}