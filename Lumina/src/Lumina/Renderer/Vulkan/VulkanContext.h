#pragma once

#include <vulkan/vulkan.h>

#include <spdlog/spdlog.h>

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <functional>

#include <imgui.h>

#include "backends/imgui_impl_vulkan.h"

#include "../../Core/Context.h"

#ifdef LUMINA_DEBUG
    #define IMGUI_VULKAN_DEBUG_REPORT
#endif

namespace Lumina
{
	static void CheckResult(VkResult err)
	{
		if (err == 0)
			return;
		fprintf(stderr, "[Vulkan Context] Error: VkResult = %d\n", err);
		if (err < 0)
			abort();
	}

#ifdef IMGUI_VULKAN_DEBUG_REPORT
	static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object,
		size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
	{
		(void)flags; (void)object; (void)location; (void)messageCode; (void)pUserData; (void)pLayerPrefix; // Unused arguments
		fprintf(stderr, "[vulkan] Debug report from ObjectType: %i\nMessage: %s\n\n", objectType, pMessage);
		return VK_FALSE;
	}
#endif

	class VulkanContext : public Context
	{
	public:
		void Init(GLFWwindow* window) override;
		void PreRender() override;
		void Render() override;
		void PostRender() override;
		void Shutdown() override;
	private:
		void CreateInstance();
		void SelectGPU(); 
		void SelectQueueFamily();
		void CreateLogicalDevice();
		void CreateDescriptorPools();
		void CreateFramebuffer();
		void CreateContext();
		void UploadFont(); 
	private:
		GLFWwindow* m_Window = nullptr;

		VkAllocationCallbacks*		m_Allocator = NULL;
		VkInstance					m_Instance = VK_NULL_HANDLE;
		VkPhysicalDevice			m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice					m_Device = VK_NULL_HANDLE;
		uint32_t					m_QueueFamily = (uint32_t)-1;
		VkQueue						m_Queue = VK_NULL_HANDLE;
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
