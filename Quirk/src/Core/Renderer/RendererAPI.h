
#pragma once

#include <utility>
#include "glm/vec4.hpp"

#include "GraphicalContext.h"
#include "VertexArray.h"

namespace Quirk {

	class RendererAPI {
	public:
		enum class API : int8_t {
			None = 0, OpenGL = 1
		};

	public:
		RendererAPI(GraphicalContext* context, API rendererAPI);
		~RendererAPI() {}

		static inline API GetAPI() { return s_API; }
		static inline GraphicalContext* GetGraphicalContext() { return s_GraphicalContext; }

	public:
		virtual inline void SetClearColor(const glm::vec4& color) const = 0;
		virtual inline void Clear() const = 0;
		virtual inline void SwapBuffers() = 0;

		virtual inline void DrawIndexed(Ref<VertexArray>& vertexArray) = 0;

	private:
		static API s_API;
		static GraphicalContext* s_GraphicalContext;
	};

}
