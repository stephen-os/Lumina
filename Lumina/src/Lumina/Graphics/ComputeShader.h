#pragma once

#include "../Core/Aliases.h"

#include <string>
#include <glm/glm.hpp>

namespace Lumina 
{
    class ComputeShader
    {
    public:
        virtual ~ComputeShader() = default;

        // Bind and dispatch
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual void Dispatch(uint32_t groupX, uint32_t groupY, uint32_t groupZ) const = 0;

        // Uniform setters
        virtual void SetUniformInt(const std::string& name, int value) = 0;
        virtual void SetUniformFloat(const std::string& name, float value) = 0;

        virtual void SetUniformVec2(const std::string& name, float a, float b) = 0;
        virtual void SetUniformVec2(const std::string& name, const glm::vec2& value) = 0;

        virtual void SetUniformVec3(const std::string& name, float a, float b, float c) = 0;
        virtual void SetUniformVec3(const std::string& name, const glm::vec3& value) = 0;

        virtual void SetUniformMat4(const std::string& name, float a, float b, float c, float d) = 0;
        virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) = 0;

		virtual void SetStorageBuffer(const std::string& name, const void* data, size_t size) = 0;

        // Bind texture to image unit
        virtual void BindImageTexture(uint32_t unit, uint32_t textureID, bool read, bool write, int mipLevel = 0) = 0;

        // Optional: memory barrier to synchronize
        virtual void InsertMemoryBarrier() const = 0;

        // Debugging
        virtual const std::string& GetName() const = 0;

        // Create from GLSL compute shader source string or path
        static Shared<ComputeShader> Create(const std::string& source);
        static Shared<ComputeShader> CreateFromFile(const std::string& filepath);

    };

} // namespace Lumina
