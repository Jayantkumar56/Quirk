

#pragma once

#include "RenderCommands.h"
#include "RendererAPI.h"

namespace Quirk {

	class Renderer {
	public:
		static void InitRenderer(RendererAPI::API rendererAPI);

		static void BeginScene();
		static void EndScene();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};

}
