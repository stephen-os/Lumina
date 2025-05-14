#include "OpenGLRenderCommands.h"

#include <glad/glad.h>
#include "RendererDebug.h"

#include "../../Core/Assert.h"

namespace Lumina
{
    void OpenGLRenderCommands::SetViewport(int x, int y, int width, int height)
    {
        LUMINA_ASSERT(width > 0 && height > 0, "Viewport dimensions must be greater than zero!");

        GLCALL(glViewport(x, y, width, height));
    }

    void OpenGLRenderCommands::Clear()
    {
        GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void OpenGLRenderCommands::EnableDepthTest()
    {
        GLCALL(glEnable(GL_DEPTH_TEST));
    }

    void OpenGLRenderCommands::DrawLines(uint32_t count)
    {
        GLCALL(glDrawArrays(GL_LINES, 0, count));
    }

    void OpenGLRenderCommands::DrawLineStrips(uint32_t count)
    {
        GLCALL(glDrawArrays(GL_LINE_STRIP, 0, count));
    }

    void OpenGLRenderCommands::DrawTriangles(uint32_t count)
    {
        GLCALL(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
    }
}