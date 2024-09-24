
#pragma once

#include "Core/Application/Window.h"
#include "Core/Application/Layer.h"

namespace Quirk {

	class Application {
	public:
		Application(const std::wstring& appName);
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& event);
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow()		 { return m_Window; }

		inline void PushLayer(Layer* layer)		{ LayerStack::PushLayer(layer); }
		inline void PushOverlay(Layer* overlay) { LayerStack::PushOverlay(overlay); }

	private:
		std::wstring m_AppName;
		Window		m_Window;
		bool		m_Running;

	private:
		static Application* s_Instance;
	};

}
