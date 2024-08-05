#include "ShaderProgram.h"
#include "GLUtils.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace GL
{
    ShaderProgram::ShaderProgram(const std::string& vertexSource, const std::string& fragmentSource)
    {
        m_IsBound = false;

        // Compile shaders
        m_VertexShader = CompileSource(GL_VERTEX_SHADER, vertexSource);
        m_FragmentShader = CompileSource(GL_FRAGMENT_SHADER, fragmentSource);

        // Link program
        GLCALL(m_Program = glCreateProgram());
        GLCALL(glAttachShader(m_Program, m_VertexShader));
        GLCALL(glAttachShader(m_Program, m_FragmentShader));
        GLCALL(glLinkProgram(m_Program));

        GLint isOkay;
        GLCALL(glGetProgramiv(m_Program, GL_LINK_STATUS, &isOkay));
        if (!isOkay)
        {
            GLchar message[512];
            GLCALL(glGetProgramInfoLog(m_Program, 512, nullptr, message));
            GLCALL(glDeleteProgram(m_Program));

            std::cerr << "Shader compilation error: " << message << std::endl;
        }

        // Query uniforms
        GLint nuniforms;
        GLCALL(glGetProgramiv(m_Program, GL_ACTIVE_UNIFORMS, &nuniforms));
        for (GLint i = 0; i < nuniforms; ++i)
        {
            GLchar name[256];
            GLsizei length;
            GLint size;
            GLenum type;
            GLCALL(glGetActiveUniform(m_Program, i, 256, &length, &size, &type, name));
            GLint location;
            GLCALL(location = glGetUniformLocation(m_Program, name));
            m_Uniforms[name] = location;

            // If uniform is an array, find locations of other elements
            for (GLint elementIndex = 1; elementIndex < size; ++elementIndex)
            {
                std::string elementName = std::string(name).substr(0, length - 3) + "[" + std::to_string(elementIndex) + "]";
                GLCALL(location = glGetUniformLocation(m_Program, elementName.c_str()));
                if (location != -1)
                {
                    m_Uniforms[elementName] = location;
                }
            }
        }

        Unbind();
    }

    void ShaderProgram::Destroy()
    {
        GLCALL(glDeleteShader(m_VertexShader));
        GLCALL(glDeleteShader(m_FragmentShader));
        GLCALL(glDeleteProgram(m_Program));
    }

    GLuint ShaderProgram::CompileSource(GLenum type, const std::string& source)
    {
        GLuint shader;
        GLCALL(shader = glCreateShader(type));
        const char* src = source.c_str();
        GLCALL(glShaderSource(shader, 1, &src, nullptr));
        GLCALL(glCompileShader(shader));

        GLint isOkay;
        GLCALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &isOkay));
        if (!isOkay)
        {
            GLchar message[512];
            GLCALL(glGetShaderInfoLog(shader, 512, nullptr, message));
            GLCALL(glDeleteShader(shader));

            std::cerr << "Shader compilation error: " << message << std::endl;
        }

        return shader;
    }

    GLint ShaderProgram::GetAttributeLocation(const std::string& name)
    {
        GLint location;
        GLCALL(location = glGetAttribLocation(m_Program, name.c_str()));
        return location;
    }

    void ShaderProgram::Bind()
    {
        GLCALL(glUseProgram(m_Program));
        m_IsBound = true;
    }

    void ShaderProgram::Unbind()
    {
        GLCALL(glUseProgram(0));
        m_IsBound = false;
    }

    void ShaderProgram::AssertUniform(const std::string& name)
    {
        if (m_Uniforms.find(name) == m_Uniforms.end())
        {
            std::cerr << name << " isn't a valid uniform." << std::endl;
        }
    }

    void ShaderProgram::SetUniform1i(const std::string& name, int value)
    {
        AssertUniform(name);
        GLCALL(glUniform1i(m_Uniforms[name], value));
    }

    void ShaderProgram::SetUniform1f(const std::string& name, float value)
    {
        AssertUniform(name);
        GLCALL(glUniform1f(m_Uniforms[name], value));
    }

    void ShaderProgram::SetUniform2f(const std::string& name, float a, float b)
    {
        AssertUniform(name);
        GLCALL(glUniform2f(m_Uniforms[name], a, b));
    }

    void ShaderProgram::SetUniform2fv(const std::string& name, glm::vec2 value)
    {
        AssertUniform(name);
        GLCALL(glUniform2fv(m_Uniforms[name], 1, glm::value_ptr(value)));
    }

    void ShaderProgram::SetUniform3f(const std::string& name, float a, float b, float c)
    {
        AssertUniform(name);
        GLCALL(glUniform3f(m_Uniforms[name], a, b, c));
    }

    void ShaderProgram::SetUniformMatrix4fv(const std::string& name, const glm::mat4& matrix)
    {
        AssertUniform(name);
        GLCALL(glUniformMatrix4fv(m_Uniforms[name], 1, GL_FALSE, glm::value_ptr(matrix)));
    }
}