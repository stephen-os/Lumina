#pragma once

#include "Aliases.h"
#include "Context.h"
#include "API.h"

#include "../Renderer/Vulkan/VulkanContext.h"
#include "../Renderer/Opengl/OpenglContext.h"

namespace Lumina
{
	class ContextFactory
	{
	public: 
        static Unique<Context> Create(API api) {
            if (api == API::VULKAN) {
                return std::make_unique<VulkanContext>();
            }
            else {
                return std::make_unique<OpenGLContext>();
            }
        }
	};
}