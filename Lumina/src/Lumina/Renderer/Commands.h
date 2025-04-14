#pragma once

#include "../Core/Aliases.h"

namespace Lumina
{
    class Commands {
    public:
        virtual ~Commands() = default;

        virtual void SetViewport(int x, int y, int width, int height) = 0;
        virtual void Clear() = 0;
        virtual void EnableDepthTest() = 0;

        static Shared<Commands> Create();
    };
}