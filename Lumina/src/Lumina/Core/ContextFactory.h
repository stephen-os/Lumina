#pragma once

#include <memory>

#include "Context.h"
#include "API.h"

#include "../Graphics/Opengl/OpenglContext.h"

namespace Lumina
{
	class ContextFactory
	{
	public: 
        static std::unique_ptr<Context> Create(API api) {
            if (api == API::OPENGL) {
                return std::make_unique<OpenGLContext>();
            }
            else {
                return nullptr; 
            }
        }
	};
}