
#pragma once


#include "RendererAPI.h"

namespace Quirk {

	class RenderCommands {
	public:
		static void Init(RendererAPI::API rendererAPI);
		static inline RendererAPI::API GetAPI() { return s_RendererAPI->GetAPI(); }

		static inline void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
		static inline void Clear() { s_RendererAPI->Clear(); }
		static inline void SwapBuffers() { s_RendererAPI->SwapBuffers(); }

	private:
		static RendererAPI* s_RendererAPI;
	};

}

