#pragma once

namespace Lumina
{
	enum class API
	{
		OPENGL,
		VULKAN
	};
	
    class RendererAPI
    {
    public:
        static API GetAPI() { return s_API; }
        static void SetAPI(API api) { s_API = api; }

    private:
        static API s_API;
    };
}