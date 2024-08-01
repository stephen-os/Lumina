#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include <glm/glm.hpp>

#include <glad/glad.h>

namespace GL
{
    class ShaderProgram
    {
    public:
        ShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);
        ~ShaderProgram();

        void Destroy();
        GLint GetAttributeLocation(const std::string& name);

        void Bind();
        void Unbind();

        void SetUniform1i(const std::string& name, int value);
        void SetUniform1f(const std::string& name, float value);
        void SetUniform2f(const std::string& name, float a, float b);
        void SetUniform2fv(const std::string& name, glm::vec2 value);
        void SetUniform3f(const std::string& name, float a, float b, float c);
        void SetUniformMatrix4fv(const std::string& name, const glm::mat4& matrix);
    private:
        GLuint CompileSource(GLenum type, const std::string& source);
        void AssertUniform(const std::string& name);
    private:
        GLuint m_VertexShader;
        GLuint m_FragmentShader;
        GLuint m_Program;
        std::unordered_map<std::string, GLint> m_Uniforms;
        bool m_IsBound;
    };
}

#endif