

#pragma once

#include "Core/Renderer/RendererAPI.h"

namespace Quirk {

	class Window;

	class GraphicalContext {
	public:
		GraphicalContext() = default;
		virtual ~GraphicalContext() = default;

		static GraphicalContext* Create(RendererAPI::API rendererAPI);

	public:
		virtual void Init(Window& window) = 0;
		virtual void DestroyContext(Window& window) = 0;
		virtual inline void SetVSync(int toggle) = 0;
		virtual inline void SwapBuffer() = 0;
	};

}
