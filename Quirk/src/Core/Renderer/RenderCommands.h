
#pragma once

#include "Core/Core.h"
#include "RendererAPI.h"
#include "VertexArray.h"

namespace Quirk {

	class RenderCommands {
	public:
		static void Init(RendererAPI::API rendererAPI);
		static inline RendererAPI::API GetAPI() { return s_RendererAPI->GetAPI(); }

		static inline void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
		static inline void Clear() { s_RendererAPI->Clear(); }
		static inline void UpdateViewPort(uint32_t width, uint32_t height) { s_RendererAPI->UpdateViewPort(width, height); }

		static inline void EnableFaceCulling() { s_RendererAPI->EnableFaceCulling(); }
		static inline void DisableFaceCulling() { s_RendererAPI->DisableFaceCulling(); }

		static inline void DrawIndexed(Ref<VertexArray>& vertexArray) { s_RendererAPI->DrawIndexed(vertexArray); }
		static inline void DrawVertices(Ref<VertexArray>& vertexArray) { s_RendererAPI->DrawVertices(vertexArray); }

	private:
		static RendererAPI* s_RendererAPI;
	};

}

