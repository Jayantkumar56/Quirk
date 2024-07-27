
#pragma once

#include "Core/Application/Window.h"
#include "Core/Application/Layer.h"

namespace Quirk {

	class Application {
	public:
		Application();
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& event);
		bool OnWindowClose(WindowCloseEvent& event);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return m_Window; }

		inline void PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }
		inline void PushOverlay(Layer* overlay) { m_LayerStack.PushOverlay(overlay); }

	private:
		Window m_Window;
		bool m_Running;
		LayerStack m_LayerStack;

	private:
		static Application* s_Instance;
	};

}
