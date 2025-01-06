#pragma once

#include "VertexArray.h"

#include "../base.h"

namespace Lumina
{
	class Renderer
	{
	public:
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void Clear();

		static void Begin(); 
		static void End(); 

		static void Draw(const Ref<VertexArray>& vertexArray);
	};
}