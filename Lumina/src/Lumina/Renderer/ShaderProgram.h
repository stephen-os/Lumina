#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include <glm/glm.hpp>

#include <glad/glad.h>

#include "VertexArray.h"

namespace Lumina
{
    class ShaderProgram
    {
    public: 
        ShaderProgram();
        ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
        ~ShaderProgram();

        void Destroy(); 
        void Bind();
        void Unbind();

        void SetSource(const std::string& vertexSource, const std::string& fragmentSource);

        int GetAttributeLocation(const std::string& name);

        void SetUniform1i(const std::string& name, int value);
        void SetUniform1f(const std::string& name, float value);
        void SetUniform2f(const std::string& name, float a, float b);
        void SetUniform2fv(const std::string& name, glm::vec2 value);
        void SetUniform3f(const std::string& name, float a, float b, float c);
        void SetUniform3fv(const std::string& name, const glm::vec3 value);
        void SetUniformMatrix4f(const std::string& name, float a, float b, float c, float d);
        void SetUniformMatrix4fv(const std::string& name, const glm::mat4& matrix);
    private:
        unsigned int CompileSource(unsigned int type, const std::string& source);
        void AssertUniform(const std::string& name);
    private:
        bool m_IsBound;
        std::unordered_map<std::string, int> m_Uniforms;
        unsigned int m_VertexShaderID;
        unsigned int m_FragmentShaderID;
        unsigned int m_ShaderProgramID;
    };
}

#endif