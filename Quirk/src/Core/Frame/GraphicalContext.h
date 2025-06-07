

#pragma once

#include "Core/Renderer/RendererAPI.h"
#include "Core/Utility/View.h"


namespace Quirk {

	class Window;

	class GraphicalContext {
	public:
		GraphicalContext() = default;
		virtual ~GraphicalContext() = default;

		// return a pointer (create by new), caller is responsible for the ownership and deletion of the pointer.
		static GraphicalContext* Create(View<Window> window);

		static Scope<GraphicalContext> Create(View<Window> window, RendererAPI::API rendererAPI);
		static void Init(RendererAPI::API rendererAPI);

	public:
		virtual void DestroyContext(View<Window> window) = 0;
		virtual inline void MakeContextCurrent() const	 = 0;
		virtual inline void SetVSync(int toggle)	     = 0;
		virtual inline void SwapBuffer()			     = 0;
	};

}
