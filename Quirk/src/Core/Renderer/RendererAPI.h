
#pragma once

#include <utility>
#include "glm/vec4.hpp"

#include "VertexArray.h"

namespace Quirk {

	class RendererAPI {
	public:
		enum class API : int8_t {
			None = 0, OpenGL = 1
		};

	public:
		RendererAPI(API rendererAPI);
		~RendererAPI() {}

		static inline API GetAPI() { return s_API; }

	public:
		virtual inline void SetClearColor(const glm::vec4& color) const = 0;
		virtual inline void Clear() const = 0;
		virtual inline void UpdateViewPort(uint32_t width, uint32_t height) const = 0;

		virtual inline void EnableFaceCulling() const = 0;
		virtual inline void DisableFaceCulling() const = 0;

		virtual inline void DrawIndexed(Ref<VertexArray>& vertexArray, uint32_t indicesCount) = 0;
		virtual inline void DrawVertices(Ref<VertexArray>& vertexArray) = 0;

	private:
		static API s_API;
	};

}
