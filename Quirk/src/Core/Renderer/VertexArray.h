

#pragma once

#include "Core/Core.h"
#include "Buffers.h"

namespace Quirk {

	class VertexArray {
	public:
		static Ref<VertexArray> Create();

	public:
		VertexArray() = default;
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector< Ref<VertexBuffer> >& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;
	};

}
