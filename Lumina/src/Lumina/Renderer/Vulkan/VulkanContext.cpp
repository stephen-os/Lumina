#include "VulkanContext.h"

#include "imgui.h"

#include "../../ImGui/Roboto-Regular.embed"

#include <backends/imgui_impl_glfw.h>

namespace Lumina 
{

	void VulkanContext::Init(GLFWwindow* window)
	{
		m_Window = window; 

		VkResult err;

		CreateInstance();
		
		SelectGPU();

		SelectQueueFamily(); 

		CreateLogicalDevice(); 
		
		CreateDescriptorPools(); 

		// Create Suface 
		{
			err = glfwCreateWindowSurface(m_Instance, m_Window, m_Allocator, &m_Surface);
			CheckResult(err);
		}

		CreateFramebuffer(); 

		CreateContext(); 
	}

	void VulkanContext::PreRender()
	{
		// Resize swap chain?
		if (m_SwapChainRebuild)
		{
			int width, height;
			glfwGetFramebufferSize(m_Window, &width, &height);
			if (width > 0 && height > 0)
			{
				ImGui_ImplVulkan_SetMinImageCount(m_MinImageCount);
				ImGui_ImplVulkanH_CreateOrResizeWindow(m_Instance,m_PhysicalDevice, m_Device, &m_MainWindowData, m_QueueFamily, m_Allocator, width, height, m_MinImageCount);
				m_MainWindowData.FrameIndex = 0;

				// Clear allocated command buffers from here since entire pool is destroyed
				m_AllocatedCommandBuffers.clear();
				m_AllocatedCommandBuffers.resize(m_MainWindowData.ImageCount);

				m_SwapChainRebuild = false;
			}
		}

		ImGui_ImplVulkan_NewFrame();
	}

	void VulkanContext::Render()
	{
		// Temporary
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		m_MainWindowData.ClearValue.color.float32[0] = clear_color.x * clear_color.w;
		m_MainWindowData.ClearValue.color.float32[1] = clear_color.y * clear_color.w;
		m_MainWindowData.ClearValue.color.float32[2] = clear_color.z * clear_color.w;
		m_MainWindowData.ClearValue.color.float32[3] = clear_color.w;

		VkResult err;

		VkSemaphore image_acquired_semaphore = m_MainWindowData.FrameSemaphores[m_MainWindowData.SemaphoreIndex].ImageAcquiredSemaphore;
		VkSemaphore render_complete_semaphore = m_MainWindowData.FrameSemaphores[m_MainWindowData.SemaphoreIndex].RenderCompleteSemaphore;
		err = vkAcquireNextImageKHR(m_Device, m_MainWindowData.Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &m_MainWindowData.FrameIndex);
		if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
		{
			m_SwapChainRebuild = true;
			return;
		}
		CheckResult(err);

		m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % m_MainWindowData.ImageCount;

		ImGui_ImplVulkanH_Frame* fd = &m_MainWindowData.Frames[m_MainWindowData.FrameIndex];
		{
			err = vkWaitForFences(m_Device, 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
			CheckResult(err);

			err = vkResetFences(m_Device, 1, &fd->Fence);
			CheckResult(err);
		}

		{
			// Free resources in queue
			for (auto& func : m_ResourceFreeQueue[m_CurrentFrameIndex])
				func();
			m_ResourceFreeQueue[m_CurrentFrameIndex].clear();
		}

		{
			// Free command buffers allocated by Application::GetCommandBuffer
			// These use g_MainWindowData.FrameIndex and not s_CurrentFrameIndex because they're tied to the swapchain image index
			auto& allocatedCommandBuffers = m_AllocatedCommandBuffers[m_MainWindowData.FrameIndex];
			if (allocatedCommandBuffers.size() > 0)
			{
				vkFreeCommandBuffers(m_Device, fd->CommandPool, (uint32_t)allocatedCommandBuffers.size(), allocatedCommandBuffers.data());
				allocatedCommandBuffers.clear();
			}

			err = vkResetCommandPool(m_Device, fd->CommandPool, 0);
			CheckResult(err);
			VkCommandBufferBeginInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
			CheckResult(err);
		}
		{
			VkRenderPassBeginInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			info.renderPass = m_MainWindowData.RenderPass;
			info.framebuffer = fd->Framebuffer;
			info.renderArea.extent.width = m_MainWindowData.Width;
			info.renderArea.extent.height = m_MainWindowData.Height;
			info.clearValueCount = 1;
			info.pClearValues = &m_MainWindowData.ClearValue;
			vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
		}


		ImDrawData* draw_data = ImGui::GetDrawData();
		// Record dear imgui primitives into command buffer
		ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

		// Submit command buffer
		vkCmdEndRenderPass(fd->CommandBuffer);
		{
			VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			VkSubmitInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			info.waitSemaphoreCount = 1;
			info.pWaitSemaphores = &image_acquired_semaphore;
			info.pWaitDstStageMask = &wait_stage;
			info.commandBufferCount = 1;
			info.pCommandBuffers = &fd->CommandBuffer;
			info.signalSemaphoreCount = 1;
			info.pSignalSemaphores = &render_complete_semaphore;

			err = vkEndCommandBuffer(fd->CommandBuffer);
			CheckResult(err);
			err = vkQueueSubmit(m_Queue, 1, &info, fd->Fence);
			CheckResult(err);
		}
	}

	void VulkanContext::PostRender()
	{
		if (m_SwapChainRebuild)
			return;
		VkSemaphore render_complete_semaphore = m_MainWindowData.FrameSemaphores[m_MainWindowData.SemaphoreIndex].RenderCompleteSemaphore;
		VkPresentInfoKHR info = {};
		info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &render_complete_semaphore;
		info.swapchainCount = 1;
		info.pSwapchains = &m_MainWindowData.Swapchain;
		info.pImageIndices = &m_MainWindowData.FrameIndex;
		VkResult err = vkQueuePresentKHR(m_Queue, &info);
		if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
		{
			m_SwapChainRebuild = true;
			return;
		}
		CheckResult(err);
		m_MainWindowData.SemaphoreIndex = (m_MainWindowData.SemaphoreIndex + 1) % m_MainWindowData.ImageCount; // Now we can use the next set of semaphores
	}
	void VulkanContext::Shutdown()
	{
		VkResult err = vkDeviceWaitIdle(m_Device);
		CheckResult(err);

		// Free resources in queue
		for (auto& queue : m_ResourceFreeQueue)
		{
			for (auto& func : queue)
				func();
		}
		m_ResourceFreeQueue.clear();

		ImGui_ImplVulkan_Shutdown();

		ImGui_ImplVulkanH_DestroyWindow(m_Instance, m_Device, &m_MainWindowData, m_Allocator);
	}

	void VulkanContext::CreateInstance()
	{
		spdlog::info("[Vulkan Context] Creating Vulkan Instance.");

		VkResult err;

		uint32_t extensions_count = 0;
		const char** extensions = glfwGetRequiredInstanceExtensions(&extensions_count);

		VkInstanceCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		create_info.enabledExtensionCount = extensions_count;
		create_info.ppEnabledExtensionNames = extensions;

#ifdef LUMINA_DEBUG
		// Enabling validation layers
		const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
		create_info.enabledLayerCount = 1;
		create_info.ppEnabledLayerNames = layers;

		// Enable debug report extension (we need additional storage, so we duplicate the user array to add our new extension to it)
		const char** extensions_ext = (const char**)malloc(sizeof(const char*) * (extensions_count + 1));
		memcpy(extensions_ext, extensions, extensions_count * sizeof(const char*));
		extensions_ext[extensions_count] = "VK_EXT_debug_report";
		create_info.enabledExtensionCount = extensions_count + 1;
		create_info.ppEnabledExtensionNames = extensions_ext;

		// Create Vulkan Instance
		err = vkCreateInstance(&create_info, m_Allocator, &m_Instance);
		CheckResult(err);
		free(extensions_ext);

		// Get the function pointer (required for any extensions)
		auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugReportCallbackEXT");
		IM_ASSERT(vkCreateDebugReportCallbackEXT != NULL);

		// Setup the debug report callback
		VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
		debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
		debug_report_ci.pfnCallback = debug_report;
		debug_report_ci.pUserData = NULL;
		err = vkCreateDebugReportCallbackEXT(m_Instance, &debug_report_ci, m_Allocator, &m_DebugReport);
		CheckResult(err);

#else
		// Create Vulkan Instance without any debug feature
		err = vkCreateInstance(&create_info, m_Allocator, &m_Instance);
		CheckResult(err);
		IM_UNUSED(m_DebugReport);
#endif
	}

	void VulkanContext::SelectGPU()
	{
		spdlog::info("[Vulkan Context] Select Physical Device.");

		VkResult err;

		uint32_t gpu_count;
		err = vkEnumeratePhysicalDevices(m_Instance, &gpu_count, NULL);
		CheckResult(err);
		IM_ASSERT(gpu_count > 0);

		VkPhysicalDevice* gpus = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * gpu_count);
		err = vkEnumeratePhysicalDevices(m_Instance, &gpu_count, gpus);
		CheckResult(err);

		// If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
		// most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
		// dedicated GPUs) is out of scope of this sample.
		int use_gpu = 0;
		for (int i = 0; i < (int)gpu_count; i++)
		{
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(gpus[i], &properties);
			if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				use_gpu = i;
				break;
			}
		}

		m_PhysicalDevice = gpus[use_gpu];
		free(gpus);
	}

	void VulkanContext::SelectQueueFamily()
	{
		spdlog::info("[Vulkan Context] Select Queue Family.");

		uint32_t count;
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &count, NULL);
		VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * count);
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &count, queues);
		for (uint32_t i = 0; i < count; i++)
			if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				m_QueueFamily = i;
				break;
			}
		free(queues);
		IM_ASSERT(m_QueueFamily != (uint32_t)-1);
	}

	void VulkanContext::CreateLogicalDevice()
	{
		spdlog::info("[Vulkan Context] Create Logical Device.");

		VkResult err;

		int device_extension_count = 1;
		const char* device_extensions[] = { "VK_KHR_swapchain" };
		const float queue_priority[] = { 1.0f };
		VkDeviceQueueCreateInfo queue_info[1] = {};
		queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_info[0].queueFamilyIndex = m_QueueFamily;
		queue_info[0].queueCount = 1;
		queue_info[0].pQueuePriorities = queue_priority;
		VkDeviceCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
		create_info.pQueueCreateInfos = queue_info;
		create_info.enabledExtensionCount = device_extension_count;
		create_info.ppEnabledExtensionNames = device_extensions;
		err = vkCreateDevice(m_PhysicalDevice, &create_info, m_Allocator, &m_Device);
		CheckResult(err);
		vkGetDeviceQueue(m_Device, m_QueueFamily, 0, &m_Queue);
	}

	void VulkanContext::CreateDescriptorPools()
	{
		spdlog::info("[Vulkan Context] Create Descriptor Pools.");

		VkDescriptorPoolSize pool_sizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		VkResult err;

		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
		pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;
		err = vkCreateDescriptorPool(m_Device, &pool_info, m_Allocator, &m_DescriptorPool);
		CheckResult(err);
	}

	void VulkanContext::CreateFramebuffer()
	{
		spdlog::info("[Vulkan Context] Create Framebuffer.");

		int w, h;
		glfwGetFramebufferSize(m_Window, &w, &h);
		ImGui_ImplVulkanH_Window* wd = &m_MainWindowData; // This doesnt need to be redeclared like this 

		wd->Surface = m_Surface;

		// Check for WSI support
		VkBool32 res;
		vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, m_QueueFamily, wd->Surface, &res);
		if (res != VK_TRUE)
		{
			fprintf(stderr, "Error no WSI support on physical device 0\n");
			exit(-1);
		}

		// Select Surface Format
		const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
		const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(m_PhysicalDevice, wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

		// Select Present Mode
#ifdef IMGUI_UNLIMITED_FRAME_RATE
		VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
#else
		VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
#endif
		wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(m_PhysicalDevice, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
		//printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

		// Create SwapChain, RenderPass, Framebuffer, etc.
		IM_ASSERT(m_MinImageCount >= 2);
		ImGui_ImplVulkanH_CreateOrResizeWindow(m_Instance, m_PhysicalDevice, m_Device, wd, m_QueueFamily, m_Allocator, w, h, m_MinImageCount);

		m_AllocatedCommandBuffers.resize(wd->ImageCount);
		m_ResourceFreeQueue.resize(wd->ImageCount);
	}

	void VulkanContext::CreateContext()
	{
		spdlog::info("[Vulkan Context] Create Context.");

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

		// Init Vulkan and upload font
		{
			ImGui_ImplGlfw_InitForVulkan(m_Window, true);
			ImGui_ImplVulkan_InitInfo init_info = {};
			init_info.Instance = m_Instance;
			init_info.PhysicalDevice = m_PhysicalDevice;
			init_info.Device = m_Device;
			init_info.QueueFamily = m_QueueFamily;
			init_info.Queue = m_Queue;
			init_info.PipelineCache = m_PipelineCache;
			init_info.DescriptorPool = m_DescriptorPool;
			init_info.Subpass = 0;
			init_info.MinImageCount = m_MinImageCount;
			init_info.ImageCount = m_MainWindowData.ImageCount;
			init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
			init_info.Allocator = m_Allocator;
			init_info.CheckVkResultFn = CheckResult;
			init_info.RenderPass = m_MainWindowData.RenderPass;
			ImGui_ImplVulkan_Init(&init_info);			
		}
	}

	void VulkanContext::UploadFont()
	{
		VkResult err;

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImFontConfig fontConfig;
		fontConfig.FontDataOwnedByAtlas = false;
		ImFont* robotoFont = io.Fonts->AddFontFromMemoryTTF((void*)g_RobotoRegular, sizeof(g_RobotoRegular), 20.0f, &fontConfig);
		io.FontDefault = robotoFont;


		// Upload Fonts
		{
			// Use any command queue
			VkCommandPool command_pool = m_MainWindowData.Frames[m_MainWindowData.FrameIndex].CommandPool;
			VkCommandBuffer command_buffer = m_MainWindowData.Frames[m_MainWindowData.FrameIndex].CommandBuffer;

			err = vkResetCommandPool(m_Device, command_pool, 0);
			CheckResult(err);
			VkCommandBufferBeginInfo begin_info = {};
			begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			err = vkBeginCommandBuffer(command_buffer, &begin_info);
			CheckResult(err);

			ImGui_ImplVulkan_CreateFontsTexture();

			VkSubmitInfo end_info = {};
			end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			end_info.commandBufferCount = 1;
			end_info.pCommandBuffers = &command_buffer;
			err = vkEndCommandBuffer(command_buffer);
			CheckResult(err);
			err = vkQueueSubmit(m_Queue, 1, &end_info, VK_NULL_HANDLE);
			CheckResult(err);

			err = vkDeviceWaitIdle(m_Device);
			CheckResult(err);
		}
	}
}