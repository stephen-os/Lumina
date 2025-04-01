#pragma once

#include <vulkan/vulkan.h>

#include <spdlog/spdlog.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef LUMINA_DEBUG

namespace Lumina
{
	static void VCheck(VkResult err)
	{
		if (err == 0)
			return;
		fprintf(stderr, "[Vulkan Context] Error: VkResult = %d\n", err);
		if (err < 0)
			abort();
		return; 
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objectType,
		uint64_t object,
		size_t location,
		int32_t messageCode,
		const char* pLayerPrefix,
		const char* pMessage,
		void* pUserData)
	{
		(void)flags; (void)object; (void)location;
		(void)messageCode; (void)pUserData; (void)pLayerPrefix; // Unused arguments

		const char* severity = (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) ? "ERROR" :
			(flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) ? "WARNING" :
			(flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) ? "PERFORMANCE" :
			(flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) ? "INFO" : "DEBUG";

		fprintf(stderr,
			"[Vulkan Debug Report]\n"
			"Severity: %s\n"
			"Layer: %s\n"
			"Object Type: %d\n"
			"Object: 0x%" PRIx64 "\n"
			"Location: %zu\n"
			"Message Code: %d\n"
			"Message: %s\n\n",
			severity, pLayerPrefix, objectType, object, location, messageCode, pMessage);

		return VK_FALSE;
	}
}

#endif