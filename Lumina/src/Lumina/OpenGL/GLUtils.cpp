#pragma once

#include "GLUtils.h"

 #include <glad/glad.h>
#include <iostream>

std::string GetGLErrorString(GLenum error)
{
    switch (error)
    {
    case GL_INVALID_ENUM:                  return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:                 return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:             return "GL_INVALID_OPERATION";
    case GL_STACK_OVERFLOW:                return "GL_STACK_OVERFLOW";
    case GL_STACK_UNDERFLOW:               return "GL_STACK_UNDERFLOW";
    case GL_OUT_OF_MEMORY:                 return "GL_OUT_OF_MEMORY";
    case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
    default:                               return "UNKNOWN_ERROR";
    }
}

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << GetGLErrorString(error)
            << " - " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}