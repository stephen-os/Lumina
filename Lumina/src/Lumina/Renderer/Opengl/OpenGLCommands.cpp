#include "OpenGLCommands.h"

#include <glad/glad.h>

namespace Lumina
{
    void OpenGLCommands::SetViewport(int x, int y, int width, int height)  
    {
        glViewport(x, y, width, height);
    }

    void OpenGLCommands::Clear()  
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLCommands::EnableDepthTest()  
    {
        glEnable(GL_DEPTH_TEST);
    }
}