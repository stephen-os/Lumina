#include "OpenGLContext.h"

#include <glad/glad.h>

#include <imgui.h>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include <spdlog/spdlog.h>

namespace Lumina
{
	void OpenGLContext::Init(GLFWwindow* window)
	{
        m_Window = window; 

        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(1);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            spdlog::error("[Opengl Context] Failed to initialize GLAD."); 
            return;
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        const char* glsl_version = "#version 130";
        ImGui_ImplOpenGL3_Init(glsl_version);
	}

	void OpenGLContext::PreRender()
	{
        ImGui_ImplOpenGL3_NewFrame();
	}

	void OpenGLContext::Render()
	{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void OpenGLContext::PostRender()
	{
        glfwSwapBuffers(m_Window);
	}

	void OpenGLContext::Shutdown()
	{
        ImGui_ImplOpenGL3_Shutdown();
	}
}