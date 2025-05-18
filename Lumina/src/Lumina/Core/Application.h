#pragma once

#include "imgui.h"

#include <GLFW/glfw3.h>

#include "Layer.h"

#include <string>
#include <vector>
#include <memory>

#include "Context.h"
#include "Aliases.h"
#include "API.h"

namespace Lumina 
{

	struct ApplicationSpecification
	{
		std::string Name = "Lumina App";
		uint32_t Width = 1600;
		uint32_t Height = 900;
		bool Fullscreen = false;
		bool Dock = false;
		bool Theme = true; 
		API Api = API::OPENGL;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& applicationSpecification = ApplicationSpecification());
		~Application();

		void Run();

		// From Walnut 
		template<typename T>
		void PushLayer()
		{
			static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of Layer!");
			m_LayerStack.emplace_back(std::make_shared<T>())->OnAttach();
		}

		void SetWindowFullscreen();

		void ApplyLuminaTheme();


		static Application& GetInstance();
		GLFWwindow* GetWindowHandle() const { return m_Window; };
	private:
		GLFWwindow* m_Window = nullptr;

		Unique<Context> m_Context;

		std::vector<std::shared_ptr<Layer>> m_LayerStack;
		ApplicationSpecification m_Specifications;

		float m_TimeStep = 0.0f; 
	};

	// Implemented by CLIENT
	Application* CreateApplication(int argc, char** argv);
}