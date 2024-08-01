#include "ShaderProgram.h"

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
        m_Program = glCreateProgram();
        glAttachShader(m_Program, m_VertexShader);
        glAttachShader(m_Program, m_FragmentShader);
        glLinkProgram(m_Program);

        GLint isOkay;
        glGetProgramiv(m_Program, GL_LINK_STATUS, &isOkay);
        if (!isOkay)
        {
            GLchar message[512];
            glGetProgramInfoLog(m_Program, 512, nullptr, message);
            glDeleteProgram(m_Program);

            std::cerr << "Shader compilation error: " << message << std::endl;
        }

        // Query uniforms
        GLint nuniforms;
        glGetProgramiv(m_Program, GL_ACTIVE_UNIFORMS, &nuniforms);
        for (GLint i = 0; i < nuniforms; ++i)
        {
            GLchar name[256];
            GLsizei length;
            GLint size;
            GLenum type;
            glGetActiveUniform(m_Program, i, 256, &length, &size, &type, name);
            GLint location = glGetUniformLocation(m_Program, name);
            m_Uniforms[name] = location;

            // If uniform is an array, find locations of other elements
            for (GLint elementIndex = 1; elementIndex < size; ++elementIndex)
            {
                std::string elementName = std::string(name).substr(0, length - 3) + "[" + std::to_string(elementIndex) + "]";
                location = glGetUniformLocation(m_Program, elementName.c_str());
                if (location != -1)
                {
                    m_Uniforms[elementName] = location;
                }
            }
        }

        Unbind();
    }

    ShaderProgram::~ShaderProgram()
    {
        Destroy();
    }

    void ShaderProgram::Destroy()
    {
        glDeleteShader(m_VertexShader);
        glDeleteShader(m_FragmentShader);
        glDeleteProgram(m_Program);
    }

    GLuint ShaderProgram::CompileSource(GLenum type, const std::string& source)
    {
        GLuint shader = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint isOkay;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isOkay);
        if (!isOkay) {
            GLchar message[512];
            glGetShaderInfoLog(shader, 512, nullptr, message);
            glDeleteShader(shader);

            std::cerr << "Shader compilation error: " << message << std::endl;
        }

        return shader;
    }

    GLint ShaderProgram::GetAttributeLocation(const std::string& name)
    {
        return glGetAttribLocation(m_Program, name.c_str());
    }

    void ShaderProgram::Bind()
    {
        glUseProgram(m_Program);
        m_IsBound = true;
    }

    void ShaderProgram::Unbind()
    {
        glUseProgram(0);
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
        glUniform1i(m_Uniforms[name], value);
    }

    void ShaderProgram::SetUniform1f(const std::string& name, float value)
    {
        AssertUniform(name);
        glUniform1f(m_Uniforms[name], value);
    }

    void ShaderProgram::SetUniform2f(const std::string& name, float a, float b)
    {
        AssertUniform(name);
        glUniform2f(m_Uniforms[name], a, b);
    }

    void ShaderProgram::SetUniform2fv(const std::string& name, glm::vec2 value)
    {
        AssertUniform(name);
        glUniform2fv(m_Uniforms[name], 1, glm::value_ptr(value));
    }

    void ShaderProgram::SetUniform3f(const std::string& name, float a, float b, float c)
    {
        AssertUniform(name);
        glUniform3f(m_Uniforms[name], a, b, c);
    }

    void ShaderProgram::SetUniformMatrix4fv(const std::string& name, const glm::mat4& matrix)
    {
        AssertUniform(name);
        glUniformMatrix4fv(m_Uniforms[name], 1, GL_FALSE, glm::value_ptr(matrix));
    }
}