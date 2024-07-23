

#include "Qkpch.h"

#include "RenderCommands.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"


namespace Quirk {

	RendererAPI* RenderCommands::s_RendererAPI = nullptr;

	void RenderCommands::Init(RendererAPI::API rendererAPI) {
		switch (rendererAPI) {
			case RendererAPI::API::None: {
				s_RendererAPI = nullptr;
				break;
			}

			case RendererAPI::API::OpenGL: {
				s_RendererAPI = new OpenGLRendererAPI();
				break;
			}

			default: QK_CORE_ASSERT(false, "INVALID or unsuported RendererAPI provided!");
		}
	}

}
