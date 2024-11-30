#include "ShaderProgram.h"
#include "GLUtils.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace GL
{
    ShaderProgram::ShaderProgram() : m_IsBound(false), m_VertexShaderID(0), m_FragmentShaderID(0), m_ShaderProgramID(0) {}

    ShaderProgram::~ShaderProgram()
    {
        Destroy(); 
    }

    void ShaderProgram::Destroy()
    {
        if (m_VertexShaderID != 0)
        {
            GLCALL(glDeleteShader(m_VertexShaderID));
            m_VertexShaderID = 0;
        }

        if (m_FragmentShaderID != 0)
        {
            GLCALL(glDeleteShader(m_FragmentShaderID));
            m_FragmentShaderID = 0;
        }

        if (m_ShaderProgramID != 0)
        {
            GLCALL(glDeleteProgram(m_ShaderProgramID));
            m_ShaderProgramID = 0;
        }
    }

    void ShaderProgram::SetSource(const std::string& vertexSource, const std::string& fragmentSource)
    {
        Destroy();

        m_VertexShaderID = CompileSource(GL_VERTEX_SHADER, vertexSource);
        m_FragmentShaderID = CompileSource(GL_FRAGMENT_SHADER, fragmentSource);
        
        GLCALL(m_ShaderProgramID = glCreateProgram());
        GLCALL(glAttachShader(m_ShaderProgramID, m_VertexShaderID));
        GLCALL(glAttachShader(m_ShaderProgramID, m_FragmentShaderID));
        GLCALL(glLinkProgram(m_ShaderProgramID));

        int isOkay;
        GLCALL(glGetProgramiv(m_ShaderProgramID, GL_LINK_STATUS, &isOkay));
        if (!isOkay)
        {
            char message[512];
            GLCALL(glGetProgramInfoLog(m_ShaderProgramID, 512, nullptr, message));
            GLCALL(glDeleteProgram(m_ShaderProgramID));
            m_ShaderProgramID = 0;

            std::cerr << "[Error] Program linking failed:\n" << message << "\n";
        }

        // Query uniforms
        m_Uniforms.clear();
        int nuniforms;
        GLCALL(glGetProgramiv(m_ShaderProgramID, GL_ACTIVE_UNIFORMS, &nuniforms));
        for (GLint i = 0; i < nuniforms; ++i)
        {
            char name[256];
            int length;
            int size;
            unsigned int type;
            GLCALL(glGetActiveUniform(m_ShaderProgramID, i, 256, &length, &size, &type, name));
            int location;
            GLCALL(location = glGetUniformLocation(m_ShaderProgramID, name));
            m_Uniforms[name] = location;

            for (int elementIndex = 1; elementIndex < size; ++elementIndex)
            {
                std::string elementName = std::string(name).substr(0, length - 3) + "[" + std::to_string(elementIndex) + "]";
                GLCALL(location = glGetUniformLocation(m_ShaderProgramID, elementName.c_str()));
                if (location != -1)
                {
                    m_Uniforms[elementName] = location;
                }
            }
        }

        Unbind();
    }


    unsigned int ShaderProgram::CompileSource(unsigned int type, const std::string& source)
    {
        unsigned int shader;
        GLCALL(shader = glCreateShader(type));
        const char* src = source.c_str();
        GLCALL(glShaderSource(shader, 1, &src, nullptr));
        GLCALL(glCompileShader(shader));

        int success;
        char message[512];
        GLCALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
        if (!success)
        {
            GLCALL(glGetShaderInfoLog(shader, 512, nullptr, message));
            GLCALL(glDeleteShader(shader));

            std::cerr << "[Error] " << (GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader compilation error.\n";
            std::cerr << message << ".\n";
        }
            
        return shader;
    }

    int ShaderProgram::GetAttributeLocation(const std::string& name)
    {
        int location;
        GLCALL(location = glGetAttribLocation(m_ShaderProgramID, name.c_str()));
        return location;
    }

    void ShaderProgram::Bind()
    {
        if (m_ShaderProgramID == 0)
        {
            std::cerr << "[Error] Attempting to bind an uncompiled shader program.\n";
            return;
        }

        GLCALL(glUseProgram(m_ShaderProgramID));
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
            std::cerr << "[Error] " << name << " isn't a valid uniform.\n";
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

    void ShaderProgram::SetUniform3fv(const std::string& name, const glm::vec3 value)
    {
        AssertUniform(name);
        GLCALL(glUniform3fv(m_Uniforms[name], 1, glm::value_ptr(value)));
    }

    void ShaderProgram::SetUniformMatrix4f(const std::string& name, float a, float b, float c, float d)
    {
        AssertUniform(name);
        GLCALL(glUniform4f(m_Uniforms[name], a, b, c, d));
    }

    void ShaderProgram::SetUniformMatrix4fv(const std::string& name, const glm::mat4& matrix)
    {
        AssertUniform(name);
        GLCALL(glUniformMatrix4fv(m_Uniforms[name], 1, GL_FALSE, glm::value_ptr(matrix)));
    }
}