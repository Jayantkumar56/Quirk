

#pragma once

namespace Quirk {

	class Window;

	class GraphicalContext {
	public:
		GraphicalContext() = default;
		virtual ~GraphicalContext() {}

		virtual void Init() = 0;
		virtual void SwapBuffer() = 0;
	};

}
