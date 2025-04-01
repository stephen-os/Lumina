#include "VulkanContext.h"

#include "imgui.h"

#include "../../ImGui/Roboto-Regular.embed"

#include <backends/imgui_impl_glfw.h>

#include "VulkanCheck.h"

namespace Lumina 
{

	void VulkanContext::Init(GLFWwindow* window)
	{
		m_Window = window; 

		CreateInstance();

		SelectGPU();

		SelectQueueFamily(); 

		CreateLogicalDevice(); 
		
		CreateDescriptorPools(); 

		VCheck(glfwCreateWindowSurface(m_Instance, m_Window, m_Allocator, &m_Surface));

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
				if (m_AllocatedCommandBuffers.size() != m_MainWindowData.ImageCount)
				{
					m_AllocatedCommandBuffers.clear();
					m_AllocatedCommandBuffers.resize(m_MainWindowData.ImageCount);
				}

				m_SwapChainRebuild = false;
			}
		}

		ImGui_ImplVulkan_NewFrame();
	}

	void VulkanContext::Render()
	{
		VkSemaphore image_acquired_semaphore = m_MainWindowData.FrameSemaphores[m_MainWindowData.SemaphoreIndex].ImageAcquiredSemaphore;
		VkSemaphore render_complete_semaphore = m_MainWindowData.FrameSemaphores[m_MainWindowData.SemaphoreIndex].RenderCompleteSemaphore;
		
		// Acquire next image from swapchain
		VkResult err = vkAcquireNextImageKHR(m_Device, m_MainWindowData.Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &m_MainWindowData.FrameIndex);
		VCheck(err);

		if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
		{
			m_SwapChainRebuild = true;
			return;
		}

		// Update the frame index (this is based on swapchain image count)
		m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % m_MainWindowData.ImageCount;

		// Prepare the frame data
		ImGui_ImplVulkanH_Frame* frameData = &m_MainWindowData.Frames[m_MainWindowData.FrameIndex];
		
		// wait indefinitely instead of periodically checking
		VCheck(vkWaitForFences(m_Device, 1, &frameData->Fence, VK_TRUE, UINT64_MAX)); 
		VCheck(vkResetFences(m_Device, 1, &frameData->Fence));

		// Free resources in queue
		for (auto& func : m_ResourceFreeQueue[m_CurrentFrameIndex])
		{
			func();
		}
		m_ResourceFreeQueue[m_CurrentFrameIndex].clear();

		// Free command buffers allocated by Application::GetCommandBuffer
		// These use g_MainWindowData.FrameIndex and not s_CurrentFrameIndex because they're tied to the swapchain image index
		auto& allocatedCommandBuffers = m_AllocatedCommandBuffers[m_MainWindowData.FrameIndex];
		if (!allocatedCommandBuffers.empty())
		{
			vkFreeCommandBuffers(m_Device, frameData->CommandPool, static_cast<uint32_t>(allocatedCommandBuffers.size()), allocatedCommandBuffers.data());
			allocatedCommandBuffers.clear();
		}

		VCheck(vkResetCommandPool(m_Device, frameData->CommandPool, 0));

		// Begin new command buffer recording
		VkCommandBufferBeginInfo commandBufferInfo = {};
		commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		VCheck(vkBeginCommandBuffer(frameData->CommandBuffer, &commandBufferInfo));

		// Begin render pass
		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_MainWindowData.RenderPass;
		renderPassInfo.framebuffer = frameData->Framebuffer;
		renderPassInfo.renderArea.extent.width = m_MainWindowData.Width;
		renderPassInfo.renderArea.extent.height = m_MainWindowData.Height;
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &m_MainWindowData.ClearValue;
		vkCmdBeginRenderPass(frameData->CommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		
		// Render ImGui data into the command buffer
		ImDrawData* draw_data = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(draw_data, frameData->CommandBuffer);

		// Submit command buffer
		vkCmdEndRenderPass(frameData->CommandBuffer);

		// Submit the command buffer to the queue 
		VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &image_acquired_semaphore;
		submitInfo.pWaitDstStageMask = &wait_stage;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &frameData->CommandBuffer;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &render_complete_semaphore;

		VCheck(vkEndCommandBuffer(frameData->CommandBuffer));
		VCheck(vkQueueSubmit(m_Queue, 1, &submitInfo, frameData->Fence));
	}

	void VulkanContext::PostRender()
	{
		// If a swap chain rebuild is requested, skip the present step
		if (m_SwapChainRebuild)
			return;

		VkSemaphore render_complete_semaphore = m_MainWindowData.FrameSemaphores[m_MainWindowData.SemaphoreIndex].RenderCompleteSemaphore;

		// Prepare present info structure
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &render_complete_semaphore;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_MainWindowData.Swapchain;
		presentInfo.pImageIndices = &m_MainWindowData.FrameIndex;
		
		// Submit the present command to the queue
		VkResult err = vkQueuePresentKHR(m_Queue, &presentInfo);
		VCheck(err);

		// Rebuild swap chain in case of error
		if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
		{
			m_SwapChainRebuild = true;
			return;
		}

		// Update the semaphore index for the next frame
		m_MainWindowData.SemaphoreIndex = (m_MainWindowData.SemaphoreIndex + 1) % m_MainWindowData.ImageCount; // Now we can use the next set of semaphores
	}

	void VulkanContext::Shutdown()
	{
		VCheck(vkDeviceWaitIdle(m_Device));

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

		uint32_t extensions_count = 0;
		const char** extensions = glfwGetRequiredInstanceExtensions(&extensions_count);

		// Create the Vulkan instance create info structure
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.enabledExtensionCount = extensions_count;
		createInfo.ppEnabledExtensionNames = extensions;

#ifdef LUMINA_DEBUG
		// Enabling validation layers
		const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
		createInfo.enabledLayerCount = 1;
		createInfo.ppEnabledLayerNames = layers;

		// Dynamically allocate memory for extension list to include debug report extension 
		std::vector<const char*> extensions_ext(extensions, extensions + extensions_count);
		extensions_ext.push_back("VK_EXT_debug_report");

		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions_ext.size());
		createInfo.ppEnabledExtensionNames = extensions_ext.data();

		// Create Vulkan Instance
		VCheck(vkCreateInstance(&createInfo, m_Allocator, &m_Instance));
	
		// Get the function pointer (required for any extensions)
		auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugReportCallbackEXT");
		if (vkCreateDebugReportCallbackEXT == nullptr)
		{
			spdlog::error("[Vulkan Context] Failed to get debug callback function pointer.");
			return;
		}

		// Setup the debug report callback
		VkDebugReportCallbackCreateInfoEXT debugReportCI = {};
		debugReportCI.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		debugReportCI.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
		debugReportCI.pfnCallback = debug_report;
		debugReportCI.pUserData = nullptr;
		VCheck(vkCreateDebugReportCallbackEXT(m_Instance, &debugReportCI, m_Allocator, &m_DebugReport));

#else
		// Create Vulkan Instance without any debug feature
		VCheck(vkCreateInstance(&createInfo, m_Allocator, &m_Instance));
		IM_UNUSED(m_DebugReport);
#endif
	}

	void VulkanContext::SelectGPU()
	{
		spdlog::info("[Vulkan Context] Select Physical Device.");

		uint32_t gpuCount;
		VCheck(vkEnumeratePhysicalDevices(m_Instance, &gpuCount, nullptr));
		
		// Check if there are any GPUs
		if (gpuCount == 0)
		{
			spdlog::error("[Vulkan Context] No GPUs found.");
			return;
		}

		std::vector<VkPhysicalDevice> gpus(gpuCount);
		VCheck(vkEnumeratePhysicalDevices(m_Instance, &gpuCount, gpus.data()));
		
		// Select the first discrete GPU if available, or fallback to the first available GPU
		m_PhysicalDevice = VK_NULL_HANDLE;
		for (uint32_t i = 0; i < gpuCount; i++)
		{
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(gpus[i], &properties);

			if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				m_PhysicalDevice = gpus[i];
				break;
			}
		}

		// Fallback to the first GPU if no discrete GPU is found
		if (m_PhysicalDevice == VK_NULL_HANDLE)
		{
			m_PhysicalDevice = gpus[0];
			spdlog::warn("[Vulkan Context] No discrete GPU found. Using the first available GPU.");
		}
	}

	void VulkanContext::SelectQueueFamily()
	{
		spdlog::info("[Vulkan Context] Select Queue Family.");

		uint32_t count;
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &count, nullptr);

		// Use a vector instead of manually allocating memory
		std::vector<VkQueueFamilyProperties> queues(count);
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &count, queues.data());

		m_QueueFamily = static_cast<uint32_t>(-1); // Initialize to an invalid index

		// Look for a queue with VK_QUEUE_GRAPHICS_BIT flag
		for (uint32_t i = 0; i < count; i++)
		{
			if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				m_QueueFamily = i;
				break;
			}
		}

		// Ensure we found a valid queue family
		if (m_QueueFamily == static_cast<uint32_t>(-1))
		{
			spdlog::error("[Vulkan Context] No suitable queue family found.");
		}

		IM_ASSERT(m_QueueFamily != static_cast<uint32_t>(-1));
	}

	void VulkanContext::CreateLogicalDevice()
	{
		spdlog::info("[Vulkan Context] Create Logical Device.");

		// Check if m_QueueFamily is valid
		if (m_QueueFamily == static_cast<uint32_t>(-1))
		{
			spdlog::error("[Vulkan Context] Invalid queue family index.");
			return; // Early exit since we can't create a device without a valid queue family
		}

		int device_extension_count = 1;
		const char* device_extensions[] = { "VK_KHR_swapchain" };

		const float queue_priority[] = { 1.0f };
		VkDeviceQueueCreateInfo queueInfo[1] = {};
		queueInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo[0].queueFamilyIndex = m_QueueFamily;
		queueInfo[0].queueCount = 1;
		queueInfo[0].pQueuePriorities = queue_priority;

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = sizeof(queueInfo) / sizeof(queueInfo[0]);
		createInfo.pQueueCreateInfos = queueInfo;
		createInfo.enabledExtensionCount = device_extension_count;
		createInfo.ppEnabledExtensionNames = device_extensions;

		VCheck(vkCreateDevice(m_PhysicalDevice, &createInfo, m_Allocator, &m_Device));

		// Retrieve the device queue
		vkGetDeviceQueue(m_Device, m_QueueFamily, 0, &m_Queue);
	}

	void VulkanContext::CreateDescriptorPools()
	{
		spdlog::info("[Vulkan Context] Create Descriptor Pools.");

		VkDescriptorPoolSize poolSizes[] =
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

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolInfo.maxSets = 1000 * IM_ARRAYSIZE(poolSizes);
		poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
		poolInfo.pPoolSizes = poolSizes;
		VCheck(vkCreateDescriptorPool(m_Device, &poolInfo, m_Allocator, &m_DescriptorPool));
	}

	void VulkanContext::CreateFramebuffer()
	{
		spdlog::info("[Vulkan Context] Create Framebuffer.");

		int width, height;
		glfwGetFramebufferSize(m_Window, &width, &height);
		
		m_MainWindowData.Surface = m_Surface;

		// Check for WSI support
		VkBool32 res;
		VCheck(vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, m_QueueFamily, m_MainWindowData.Surface, &res));
		if (res != VK_TRUE)
		{
			fprintf(stderr, "Error no WSI support on physical device 0\n");
			exit(-1);
		}

		// Select Surface Format
		const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
		const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		m_MainWindowData.SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(m_PhysicalDevice, m_MainWindowData.Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

		// Select Present Mode
#ifdef IMGUI_UNLIMITED_FRAME_RATE
		VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
#else
		VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
#endif
		m_MainWindowData.PresentMode = ImGui_ImplVulkanH_SelectPresentMode(m_PhysicalDevice, m_MainWindowData.Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));

		// Create SwapChain, RenderPass, Framebuffer, etc.
		IM_ASSERT(m_MinImageCount >= 2);
		ImGui_ImplVulkanH_CreateOrResizeWindow(m_Instance, m_PhysicalDevice, m_Device, &m_MainWindowData, m_QueueFamily, m_Allocator, width, height, m_MinImageCount);

		m_AllocatedCommandBuffers.resize(m_MainWindowData.ImageCount);
		m_ResourceFreeQueue.resize(m_MainWindowData.ImageCount);
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
			ImGui_ImplVulkan_InitInfo initInfo = {};
			initInfo.Instance = m_Instance;
			initInfo.PhysicalDevice = m_PhysicalDevice;
			initInfo.Device = m_Device;
			initInfo.QueueFamily = m_QueueFamily;
			initInfo.Queue = m_Queue;
			initInfo.PipelineCache = m_PipelineCache;
			initInfo.DescriptorPool = m_DescriptorPool;
			initInfo.Subpass = 0;
			initInfo.MinImageCount = m_MinImageCount;
			initInfo.ImageCount = m_MainWindowData.ImageCount;
			initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
			initInfo.Allocator = m_Allocator;
			initInfo.CheckVkResultFn = VCheck;
			initInfo.RenderPass = m_MainWindowData.RenderPass;
			ImGui_ImplVulkan_Init(&initInfo);
		}
	}
}