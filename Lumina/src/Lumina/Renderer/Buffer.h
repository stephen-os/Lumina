#pragma once

#include <string>
#include <vector>

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

	struct BufferElement
	{
		std::string Name;
		BufferDataType Type;
		uint32_t Size;
		size_t Offset;
		bool Normalized;

		BufferElement() = default;

		BufferElement(BufferDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(CalculateDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case BufferDataType::Float:   return 1;
				case BufferDataType::Float2:  return 2;
				case BufferDataType::Float3:  return 3;
				case BufferDataType::Float4:  return 4;
				case BufferDataType::Mat3:    return 3;
				case BufferDataType::Mat4:    return 4;
				case BufferDataType::Int:     return 1;
				case BufferDataType::Int2:    return 2;
				case BufferDataType::Int3:    return 3;
				case BufferDataType::Int4:    return 4;
				case BufferDataType::Bool:    return 1;
			}

			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;

		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_Elements(elements)
		{
			CalculateStride();
		}

		uint32_t GetStride() const { return m_Stride; }
		const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateStride()
		{
			size_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
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

		const BufferLayout& GetLayout() const { return m_BufferLayout; }
		void SetLayout(const BufferLayout& layout) { m_BufferLayout = layout; }
	private:
		uint32_t m_BufferID;
		BufferLayout m_BufferLayout;
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