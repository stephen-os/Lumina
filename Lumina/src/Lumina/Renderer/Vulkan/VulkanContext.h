#pragma once

#include <vulkan/vulkan.h>

#include <spdlog/spdlog.h>

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <functional>
#include <inttypes.h>

#include <imgui.h>

#include "backends/imgui_impl_vulkan.h"

#include "../../Core/Context.h"

namespace Lumina
{
	class VulkanContext : public Context
	{
	public:
		void Init(GLFWwindow* window) override;
		void PreRender() override;
		void Render() override;
		void PostRender() override;
		void Shutdown() override;
		
		static VkDevice GetDevice() { return m_Device; }
		static VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
		static VkQueue GetQueue() { return m_Queue; }
		static uint32_t GetQueueFamily() { return m_QueueFamily; }
		static VkInstance GetInstance() { return m_Instance; }
	private:
		void CreateInstance();
		void SelectGPU(); 
		void SelectQueueFamily();
		void CreateLogicalDevice();
		void CreateDescriptorPools();
		void CreateFramebuffer();
		void CreateContext();
	private:
		GLFWwindow* m_Window = nullptr;

		VkAllocationCallbacks*		m_Allocator = NULL;

		static VkInstance			m_Instance;
		static VkPhysicalDevice		m_PhysicalDevice;
		static VkDevice				m_Device;
		static uint32_t				m_QueueFamily;
		static VkQueue				m_Queue;
		VkDebugReportCallbackEXT	m_DebugReport = VK_NULL_HANDLE;
		VkPipelineCache				m_PipelineCache = VK_NULL_HANDLE;
		VkDescriptorPool			m_DescriptorPool = VK_NULL_HANDLE;
		VkSurfaceKHR				m_Surface = VK_NULL_HANDLE;

		ImGui_ImplVulkanH_Window	m_MainWindowData;
		int							m_MinImageCount = 2;
		bool						m_SwapChainRebuild = false;

		// Per-frame-in-flight
		std::vector<std::vector<VkCommandBuffer>>		m_AllocatedCommandBuffers;
		std::vector<std::vector<std::function<void()>>> m_ResourceFreeQueue;

		// Unlike g_MainWindowData.FrameIndex, this is not the the swapchain image index
		// and is always guaranteed to increase (eg. 0, 1, 2, 0, 1, 2)
		uint32_t m_CurrentFrameIndex = 0;

	};
}
