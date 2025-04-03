#pragma once

#include "Buffer.h"

#include "../Core/Aliases.h"

namespace Lumina
{
	class VertexArray
	{
	public: 
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(Shared<VertexBuffer> vertexBuffer) = 0;
		virtual void SetIndexBuffer(Shared<IndexBuffer> indexBuffer) = 0;

		virtual void DrawIndexed() = 0;

		virtual std::vector<Shared<VertexBuffer>>::iterator begin() = 0;
		virtual std::vector<Shared<VertexBuffer>>::iterator end() = 0;
		virtual std::vector<Shared<VertexBuffer>>::const_iterator begin() const = 0;
		virtual std::vector<Shared<VertexBuffer>>::const_iterator end() const = 0;

		static Shared<VertexArray> Create(); 
	};
}