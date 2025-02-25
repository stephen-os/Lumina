#include "Application.h"

#include <iostream>
#include <vector> 

#include <fstream>
#include <filesystem>

static void GLFWErrorCallback(int error, const char* description)
{
    std::cerr << "[GLFW ERROR] \"" << error << "\".\n";
    std::cerr << description << ".\n";
}

Lumina::Application::Application(const ApplicationSpecification& applicationSpecification)
{
    m_Specifications = applicationSpecification;

    glfwSetErrorCallback(GLFWErrorCallback);

    if (!glfwInit())
        return;

    m_Window = glfwCreateWindow(m_Specifications.Width, m_Specifications.Height, m_Specifications.Name.c_str(), NULL, NULL);
    if (!m_Window)
    {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);

    // This isnt really docking to the monitor, but its close enough. 
    // I cant seem to figure out how to get the window to acutally dock.
    if (m_Specifications.Dock)
    {
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        if (primaryMonitor)
        {
            int xpos, ypos, width, height;
            glfwGetMonitorWorkarea(primaryMonitor, &xpos, &ypos, &width, &height);

            glfwSetWindowPos(m_Window, xpos, ypos + 25);
            glfwSetWindowSize(m_Window, width, height - 25);
        }
    }

    // Fullscreen
    if (m_Specifications.Fullscreen)
    {
        SetWindowFullscreen();
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "[GLAD ERROR]\n";
        std::cerr << "Failed to initialize GLAD.\n";
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


Lumina::Application::~Application()
{
    for (auto& layer : m_LayerStack)
        layer->OnDetach();

    m_LayerStack.clear(); 

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Lumina::Application::Run()
{
    // Main loop
    while (!glfwWindowShouldClose(m_Window))
    {
        for (auto& layer : m_LayerStack)
            layer->OnUpdate(m_TimeStep);

        glfwPollEvents();

        // ImGui new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();

        // Main window docking
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;
        
        ImGui::Begin("Lumina", nullptr, window_flags);
        ImGui::End();

        ImGui::PopStyleVar(3);

        // Layer from LuminaApp 
        for (auto& layer : m_LayerStack)
            layer->OnUIRender();

        // Render ImGui
        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Handle ImGui viewport if enabled
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(m_Window);
    }
}

void Lumina::Application::SetWindowFullscreen()
{
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor) return;

    // Get the monitor's video mode
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // Set the window to fullscreen
    glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
}