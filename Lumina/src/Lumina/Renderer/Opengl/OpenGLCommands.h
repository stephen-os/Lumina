#pragma once

#include "../Commands.h"

namespace Lumina
{
    class OpenGLCommands : public Commands 
    {
    public:
        void SetViewport(int x, int y, int width, int height) override;
        void Clear() override;
        void EnableDepthTest() override;
    };
}