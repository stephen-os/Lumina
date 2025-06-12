#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Lumina
{
	class ShaderProgram
	{
	public:
		virtual ~ShaderProgram() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniformInt(const std::string& name, int value) = 0;
		virtual void SetUniformFloat(const std::string& name, float value) = 0;

		virtual void SetUniformVec2(const std::string& name, float a, float b) = 0;
		virtual void SetUniformVec2(const std::string& name, const glm::vec2& value) = 0;

		virtual void SetUniformVec3(const std::string& name, float a, float b, float c) = 0;
		virtual void SetUniformVec3(const std::string& name, const glm::vec3& value) = 0;

		virtual void SetUniformMat4(const std::string& name, float a, float b, float c, float d) = 0;
		virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) = 0;

		static std::shared_ptr<ShaderProgram> Create(const std::string& vertexSource, const std::string& fragmentSource);
	};
}
