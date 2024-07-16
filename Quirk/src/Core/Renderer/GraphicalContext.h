

#pragma once

namespace Quirk {

	class GraphicalContext {
	public:
		GraphicalContext() = default;
		virtual ~GraphicalContext() {}

		virtual void Init(void* window) = 0;
		virtual void SwapBuffer() = 0;

		static GraphicalContext* CreateContext();
	};

}
