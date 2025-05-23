#pragma once

#include <cstdint>

namespace Lumina
{
    class OpenGLRenderCommands
    {
    public:
        static void SetViewport(int x, int y, int width, int height);
        static void Clear();
        
        // Enable
        static void EnableDepthTest();

        // Draw Commands
        static void DrawLines(uint32_t count);
        static void DrawLineStrips(uint32_t count); 
        static void DrawTriangles(uint32_t count);
    };
}