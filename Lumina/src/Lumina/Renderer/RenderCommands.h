#pragma once

#include "Commands.h"

namespace Lumina
{
    class RenderCommands 
    {
    public:
        static void Init();
        static void SetViewport(int x, int y, int width, int height);
        static void Clear();
        static void EnableDepthTest();

    private:
        static Shared<Commands> s_Commands;
    };
}