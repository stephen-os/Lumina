#include "OpenGLCommands.h"

#include <glad/glad.h>
#include "RendererDebug.h"

#include "../../Core/Assert.h"

namespace Lumina
{
    void OpenGLCommands::SetViewport(int x, int y, int width, int height)  
    {
        LUMINA_ASSERT(width > 0 && height > 0, "Viewport dimensions must be greater than zero!");

        GLCALL(glViewport(x, y, width, height));
    }

    void OpenGLCommands::Clear()  
    {
        GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void OpenGLCommands::EnableDepthTest()  
    {
        GLCALL(glEnable(GL_DEPTH_TEST));
    }
}