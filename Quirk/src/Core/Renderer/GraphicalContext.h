

#pragma once
#include "Core/Renderer/RendererAPI.h"

namespace Quirk {

	class Window;

	class GraphicalContext {
	public:
		GraphicalContext() = default;
		virtual ~GraphicalContext() = default;

		static Scope<GraphicalContext> Create(Window& window);
		static Scope<GraphicalContext> Create(Window& window, RendererAPI::API rendererAPI);
		static void Init(RendererAPI::API rendererAPI);

	public:
		virtual void DestroyContext(Window& window) = 0;
		virtual inline void MakeContextCurrent()	= 0;
		virtual inline void SetVSync(int toggle)	= 0;
		virtual inline void SwapBuffer()			= 0;
	};

}
