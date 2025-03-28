#include "Application.h"

#include <iostream>
#include <vector> 

#include <spdlog/spdlog.h>

#include <fstream>
#include <filesystem>
#include <stdlib.h>

#include "../Renderer/Vulkan/VulkanContext.h"
#include <backends/imgui_impl_glfw.h>


#include "../ImGui/Roboto-Regular.embed"

namespace Lumina
{

    static void GLFWErrorCallback(int error, const char* description)
    {
        std::cerr << "[GLFW ERROR] \"" << error << "\".\n";
        std::cerr << description << ".\n";
    }

    Application::Application(const ApplicationSpecification& applicationSpecification)
    {
        m_Specifications = applicationSpecification;

        glfwSetErrorCallback(GLFWErrorCallback);

        if (!glfwInit())
            return;

        if (m_Specifications.Api == API::VULKAN)
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_Window = glfwCreateWindow(m_Specifications.Width, m_Specifications.Height, m_Specifications.Name.c_str(), NULL, NULL);
        if (!m_Window)
        {
            glfwTerminate();
            return;
        }

        // Vulkan
        if (m_Specifications.Api == API::VULKAN)
        {
            if (!glfwVulkanSupported())
            {
                std::cerr << "GLFW: Vulkan not supported!\n";
                return;
            }

            uint32_t extensions_count = 0;
            const char** extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
            Vulkan::SetupVulkan(extensions, extensions_count);


            // Create Window Surface
            VkSurfaceKHR surface;
            VkResult err = glfwCreateWindowSurface(Vulkan::g_Instance, m_Window, Vulkan::g_Allocator, &surface);
            Vulkan::CheckResult(err);

            // Create Framebuffers
            int w, h;
            glfwGetFramebufferSize(m_Window, &w, &h);
            ImGui_ImplVulkanH_Window* wd = &Vulkan::g_MainWindowData;
            Vulkan::SetupVulkanWindow(wd, surface, w, h);

            Vulkan::s_AllocatedCommandBuffers.resize(wd->ImageCount);
            Vulkan::s_ResourceFreeQueue.resize(wd->ImageCount);

            spdlog::info("[Application] Finished Setting Up Vulkan.");

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

            ImGui_ImplGlfw_InitForVulkan(m_Window, true);
            ImGui_ImplVulkan_InitInfo init_info = {};
            init_info.Instance = Vulkan::g_Instance;
            init_info.PhysicalDevice = Vulkan::g_PhysicalDevice;
            init_info.Device = Vulkan::g_Device;
            init_info.QueueFamily = Vulkan::g_QueueFamily;
            init_info.Queue = Vulkan::g_Queue;
            init_info.PipelineCache = Vulkan::g_PipelineCache;
            init_info.DescriptorPool = Vulkan::g_DescriptorPool;
            init_info.Subpass = 0;
            init_info.MinImageCount = Vulkan::g_MinImageCount;
            init_info.ImageCount = wd->ImageCount;
            init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
            init_info.Allocator = Vulkan::g_Allocator;
            init_info.CheckVkResultFn = Vulkan::CheckResult;
            init_info.RenderPass = wd->RenderPass; 
            ImGui_ImplVulkan_Init(&init_info);

            ImFontConfig fontConfig;
            fontConfig.FontDataOwnedByAtlas = false;
            ImFont* robotoFont = io.Fonts->AddFontFromMemoryTTF((void*)g_RobotoRegular, sizeof(g_RobotoRegular), 20.0f, &fontConfig);
            io.FontDefault = robotoFont;

            // Upload Fonts
            {
                // Use any command queue
                VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;
                VkCommandBuffer command_buffer = wd->Frames[wd->FrameIndex].CommandBuffer;

                err = vkResetCommandPool(Vulkan::g_Device, command_pool, 0);
                Vulkan::CheckResult(err);
                VkCommandBufferBeginInfo begin_info = {};
                begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
                err = vkBeginCommandBuffer(command_buffer, &begin_info);
                Vulkan::CheckResult(err);

                ImGui_ImplVulkan_CreateFontsTexture(); 
                // ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

                VkSubmitInfo end_info = {};
                end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                end_info.commandBufferCount = 1;
                end_info.pCommandBuffers = &command_buffer;
                err = vkEndCommandBuffer(command_buffer);
                Vulkan::CheckResult(err);
                err = vkQueueSubmit(Vulkan::g_Queue, 1, &end_info, VK_NULL_HANDLE);
                Vulkan::CheckResult(err);

                err = vkDeviceWaitIdle(Vulkan::g_Device);
                Vulkan::CheckResult(err);
                // ImGui_ImplVulkan_DestroyFontUploadObjects();
            }

            spdlog::info("[Application] Vulkan Initialization Complete");
            // exit(1);
        }

        // OpenGL  
        if (m_Specifications.Api == API::OPENGL)
        {

            glfwMakeContextCurrent(m_Window);
            glfwSwapInterval(1);

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
    
        // Fullscreen with taskbar
        if (m_Specifications.Dock)
        {
            GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
            if (primaryMonitor)
            {
                int xpos, ypos, width, height;
                glfwGetMonitorWorkarea(primaryMonitor, &xpos, &ypos, &width, &height);

                glfwSetWindowPos(m_Window, xpos, ypos);
                glfwSetWindowSize(m_Window, width, height);
            }
        }

        // Fullscreen
        if (m_Specifications.Fullscreen)
        {
            SetWindowFullscreen();
        }

        // Apply Theme
        if (m_Specifications.Theme)
            ApplyLuminaTheme(); 
    }


    Application::~Application()
    {
        for (auto& layer : m_LayerStack)
            layer->OnDetach();

        m_LayerStack.clear(); 
        
        // OpenGL
        if (m_Specifications.Api == API::OPENGL)
        {
            ImGui_ImplOpenGL3_Shutdown();
        }

        // Vulkan
        if (m_Specifications.Api == API::VULKAN)
        {
            // Cleanup
            VkResult err = vkDeviceWaitIdle(Vulkan::g_Device);
            Vulkan::CheckResult(err);

            // Free resources in queue
            for (auto& queue : Vulkan::s_ResourceFreeQueue)
            {
                for (auto& func : queue)
                    func();
            }
            Vulkan::s_ResourceFreeQueue.clear();

            ImGui_ImplVulkan_Shutdown();
        }

        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Application::Run()
    {
        // Only for Vulkan 
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        // Main loop
        while (!glfwWindowShouldClose(m_Window))
        {
            for (auto& layer : m_LayerStack)
                layer->OnUpdate(m_TimeStep);

            glfwPollEvents();

            // OpenGL
            if (m_Specifications.Api == API::OPENGL)
            {
                ImGui_ImplOpenGL3_NewFrame();
            }

            // Vulkan
            if (m_Specifications.Api == API::VULKAN)
            {
                // Resize swap chain?
                if (Vulkan::g_SwapChainRebuild)
                {
                    int width, height;
                    glfwGetFramebufferSize(m_Window, &width, &height);
                    if (width > 0 && height > 0)
                    {
                        ImGui_ImplVulkan_SetMinImageCount(Vulkan::g_MinImageCount);
                        ImGui_ImplVulkanH_CreateOrResizeWindow(Vulkan::g_Instance, Vulkan::g_PhysicalDevice, Vulkan::g_Device, &Vulkan::g_MainWindowData, Vulkan::g_QueueFamily, Vulkan::g_Allocator, width, height, Vulkan::g_MinImageCount);
                        Vulkan::g_MainWindowData.FrameIndex = 0;

                        // Clear allocated command buffers from here since entire pool is destroyed
                        Vulkan::s_AllocatedCommandBuffers.clear();
                        Vulkan::s_AllocatedCommandBuffers.resize(Vulkan::g_MainWindowData.ImageCount);

                        Vulkan::g_SwapChainRebuild = false;
                    }
                }

                ImGui_ImplVulkan_NewFrame();
            }

            // ImGui new frame
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

            // OpenGL Draw
            if (m_Specifications.Api == API::OPENGL)
            {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            }

            // Vulkan Draw
            if (m_Specifications.Api == API::VULKAN)
            {
                ImDrawData* main_draw_data = ImGui::GetDrawData();
                const bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);
                Vulkan::g_MainWindowData.ClearValue.color.float32[0] = clear_color.x * clear_color.w;
                Vulkan::g_MainWindowData.ClearValue.color.float32[1] = clear_color.y * clear_color.w;
                Vulkan::g_MainWindowData.ClearValue.color.float32[2] = clear_color.z * clear_color.w;
                Vulkan::g_MainWindowData.ClearValue.color.float32[3] = clear_color.w;
                if (!main_is_minimized)
                    Vulkan::FrameRender(&Vulkan::g_MainWindowData, main_draw_data);
            }

            // Handle ImGui viewport if enabled
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }

            if (m_Specifications.Api == API::OPENGL)
            {
                glfwSwapBuffers(m_Window);
            }

            if (m_Specifications.Api == API::VULKAN)
            {
                ImDrawData* main_draw_data = ImGui::GetDrawData();
                const bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);
                if (!main_is_minimized)
                    Vulkan::FramePresent(&Vulkan::g_MainWindowData);
            }
        }
    }

    void Application::SetWindowFullscreen()
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        if (!monitor) return;

        // Get the monitor's video mode
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        // Set the window to fullscreen
        glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }

    void Application::ApplyLuminaTheme()
    {
        ImGui::StyleColorsDark();                                               // First apply dark theme 

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 5.0f;                                            // Rounded corners for windows
        style.FrameRounding = 5.0f;                                             // Rounded corners for frames
        style.FramePadding = ImVec2(5.0f, 5.0f);                                // Padding within a frame
        style.ItemSpacing = ImVec2(8.0f, 6.0f);                                 // Spacing between items

        ImVec4* colors = style.Colors;
        colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);             // Dark background
        colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2f, 0.2f, 0.2f, 0.6f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);              // Frame background
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);       // Hovered frame background
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);        // Active frame background
        colors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);               // Button
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);        // Hovered button
        colors[ImGuiCol_ButtonActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);         // Active button
        colors[ImGuiCol_Header] = ImVec4(0.8f, 0.4f, 0.1f, 0.8f);               // Header (Orange)
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.9f, 0.5f, 0.2f, 0.8f);        // Hovered header
        colors[ImGuiCol_HeaderActive] = ImVec4(1.0f, 0.6f, 0.3f, 1.0f);         // Active header
        colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);            // Checkmark (Orange)
        colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);           // Slider grab
        colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.6f, 0.2f, 1.0f);     // Active slider grab
        colors[ImGuiCol_Tab] = ImVec4(0.8f, 0.4f, 0.1f, 1.0f);                  // Tab (Orange)
        colors[ImGuiCol_TabHovered] = ImVec4(0.9f, 0.5f, 0.2f, 1.0f);           // Hovered tab
        colors[ImGuiCol_TabActive] = ImVec4(1.0f, 0.6f, 0.3f, 1.0f);            // Active tab
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);         // Unfocused tab
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);   // Unfocused active tab

        // Maintain compatibility with multiple viewports
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;                                        // Disable rounding for additional platform windows
            colors[ImGuiCol_WindowBg].w = 1.0f;                                 // Fully opaque background
        }
    }
}