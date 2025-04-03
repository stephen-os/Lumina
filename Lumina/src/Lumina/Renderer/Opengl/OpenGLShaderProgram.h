#pragma once

#include "../ShaderProgram.h"

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include <glm/glm.hpp>

#include <glad/glad.h>

namespace Lumina
{
    class OpenGLShaderProgram : public ShaderProgram
    {
    public: 
        OpenGLShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
        ~OpenGLShaderProgram();
 
        void Bind() const override;
        void Unbind() const override;

        int GetAttributeLocation(const std::string& name);

        void SetUniformInt(const std::string& name, int value) override;
        void SetUniformFloat(const std::string& name, float value) override;

        void SetUniformVec2(const std::string& name, float a, float b) override;
        void SetUniformVec2(const std::string& name, const glm::vec2& value) override;

        void SetUniformVec3(const std::string& name, float a, float b, float c) override;
        void SetUniformVec3(const std::string& name, const glm::vec3& value) override;

        void SetUniformMat4(const std::string& name, float a, float b, float c, float d) override;
        void SetUniformMat4(const std::string& name, const glm::mat4& value) override;
    private:
        unsigned int CompileSource(unsigned int type, const std::string& source);
        void AssertUniform(const std::string& name);
    private:
        unsigned int m_VertexShaderID = 0;
        unsigned int m_FragmentShaderID = 0;
        unsigned int m_ShaderProgramID = 0;

        std::unordered_map<std::string, int> m_Uniforms;
    };
}