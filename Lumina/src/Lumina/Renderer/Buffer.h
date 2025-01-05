#pragma once

#include <string>
#include <glad/glad.h>

namespace Lumina
{
	enum class BufferDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t CalculateDataTypeSize(BufferDataType type)
	{
		switch (type)
		{
			case BufferDataType::Float:    return 4;
			case BufferDataType::Float2:   return 4 * 2;
			case BufferDataType::Float3:   return 4 * 3;
			case BufferDataType::Float4:   return 4 * 4;
			case BufferDataType::Mat3:     return 4 * 3 * 3;
			case BufferDataType::Mat4:     return 4 * 4 * 4;
			case BufferDataType::Int:      return 4;
			case BufferDataType::Int2:     return 4 * 2;
			case BufferDataType::Int3:     return 4 * 3;
			case BufferDataType::Int4:     return 4 * 4;
			case BufferDataType::Bool:     return 1;
		}

		return 0;
	}

	struct BufferInfo
	{
		std::string Name;
		BufferDataType Type;
		uint32_t Count;
		uint32_t Stride;
		uint32_t Size;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(uint32_t size);
		VertexBuffer(float* vertices, uint32_t size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		unsigned int GetID() const { return m_BufferID; }

		void SetData(const void* data, uint32_t size);

		const BufferInfo& GetInfo() const { return m_BufferInfo; }
		void SetInfo(const BufferInfo& info) { m_BufferInfo = info; }
	private:
		uint32_t m_BufferID;
		BufferInfo m_BufferInfo;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		uint32_t GetCount() const { return m_Count; }
	private:
		uint32_t m_BufferID;
		uint32_t m_Count;
	};
}