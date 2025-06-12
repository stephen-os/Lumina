#pragma once

#include <memory>

#include "VertexArray.h"

namespace Lumina
{
    class RenderCommands 
    {
    public:
        static void SetViewport(int x, int y, int width, int height);
        static void Clear();
        static void EnableDepthTest();

        static void DrawLines(const std::shared_ptr<VertexArray>& vao, uint32_t count);
        static void DrawLineStrips(const std::shared_ptr<VertexArray>& vao, uint32_t count);
        static void DrawTriangles(const std::shared_ptr<VertexArray>& vao);
    };
}