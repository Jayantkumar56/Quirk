

#include "Qkpch.h"

#include "RendererAPI.h"

namespace Quirk {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::Invalid;

	RendererAPI::RendererAPI(API rendererAPI) {
		s_API = rendererAPI;
	}

}
