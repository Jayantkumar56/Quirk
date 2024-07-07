
#pragma once

#include "Core/Window.h"

namespace Quirk {

	class Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		Window* m_Window;
		bool m_Running;

	private:
		static Application* s_Instance;

	};

	// To be defined in the actual software using Quirk engine
	Application* CreateApplication();

}
