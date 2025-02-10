
#pragma once

#include <utility>
#include "glm/vec4.hpp"

#include "Core/Renderer/RendererPrimitives/VertexArray.h"

namespace Quirk {

	class RendererAPI {
	public:
		enum class API : int8_t {
			Invalid = -1, None = 0, OpenGL = 1
		};

	public:
		RendererAPI(API rendererAPI);
		~RendererAPI() {}

		static inline API GetAPI() { 
			QK_CORE_ASSERT(s_API != API::Invalid, "RendererAPI is not Initialized yet!");
			return s_API; 
		}

	public:
		virtual inline void SetClearColor(const glm::vec4& color) const = 0;
		virtual inline void Clear() const = 0;
		virtual inline void UpdateViewPort(uint32_t width, uint32_t height) const = 0;

		virtual inline void EnableBlending() const = 0;

		virtual inline void EnableFaceCulling() const = 0;
		virtual inline void DisableFaceCulling() const = 0;

		virtual inline void DrawIndexed(Ref<VertexArray>& vertexArray, uint32_t indicesCount) = 0;
		virtual inline void DrawIndexedInstanced(Ref<VertexArray>& vertexArray, uint32_t noOfInstances) = 0;
		virtual inline void DrawVertices(Ref<VertexArray>& vertexArray) = 0;

	private:
		static API s_API;
	};

}
