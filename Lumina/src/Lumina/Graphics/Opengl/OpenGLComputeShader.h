#pragma once

#include "../ComputeShader.h"

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glad/glad.h>

namespace Lumina
{
    class OpenGLComputeShader : public ComputeShader
    {
    public:
        OpenGLComputeShader(const std::string& source);
        ~OpenGLComputeShader();

        void Bind() const override;
        void Unbind() const override;
        void Dispatch(uint32_t groupX, uint32_t groupY, uint32_t groupZ) const override;

        void SetUniformInt(const std::string& name, int value) override;
        void SetUniformFloat(const std::string& name, float value) override;

        void SetUniformVec2(const std::string& name, float a, float b) override;
        void SetUniformVec2(const std::string& name, const glm::vec2& value) override;

        void SetUniformVec3(const std::string& name, float a, float b, float c) override;
        void SetUniformVec3(const std::string& name, const glm::vec3& value) override;

        void SetUniformMat4(const std::string& name, float a, float b, float c, float d) override;
        void SetUniformMat4(const std::string& name, const glm::mat4& value) override;

		void SetStorageBuffer(const std::string& name, const void* data, size_t size) override;

        void BindImageTexture(uint32_t unit, uint32_t textureID, bool read, bool write, int mipLevel = 0) override;
        void InsertMemoryBarrier() const override;

        const std::string& GetName() const override { return m_Name; }

    private:
        uint32_t CompileShader(const std::string& source);
        void AssertUniform(const std::string& name);

    private:
        uint32_t m_ShaderProgramID = 0;
        uint32_t m_ComputeShaderID = 0;
        std::unordered_map<std::string, int> m_Uniforms;
        std::unordered_map<std::string, int> m_SSBOs;
        std::string m_Name;
    };
}
