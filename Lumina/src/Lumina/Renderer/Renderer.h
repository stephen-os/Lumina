#pragma once

#include "VertexArray.h"

#include <glm/glm.hpp>

namespace Lumina
{
	
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown(); 

		static void Begin();
		static void End();
		static void Flush(); 

		static void SetResolution(uint32_t width, uint32_t height); 
		static uint32_t GetImage(); 

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }

		};

		static void ResetStats();
		static Statistics GetStats();
	};
}