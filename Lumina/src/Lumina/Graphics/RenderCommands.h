#pragma once

#include "VertexArray.h"

namespace Lumina
{
    class RenderCommands 
    {
    public:
        static void SetViewport(int x, int y, int width, int height);
        static void Clear();
        static void EnableDepthTest();

        static void DrawLines(const Shared<VertexArray>& vao, uint32_t count);
        static void DrawLineStrips(const Shared<VertexArray>& vao, uint32_t count); 
        static void DrawTriangles(const Shared<VertexArray>& vao);
    };
}