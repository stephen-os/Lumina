#pragma once

#include <vulkan/vulkan.h>

#include <spdlog/spdlog.h>

#include <stdio.h>
#include <stdlib.h>

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
}