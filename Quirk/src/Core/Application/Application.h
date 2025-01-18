
#pragma once

#include "Core/Application/Window.h"
#include "Core/Application/Layer.h"
#include "Core/Input/ApplicationEvents.h"
#include "Core/Imgui/ImguiUI.h"

namespace Quirk {

	class Application {
	public:
		Application(const std::wstring& appName, RendererAPI::API renderingAPI);
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& event);
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow()		 { return m_Window; }

		inline void PushLayer(Layer* layer)		{ LayerStack::PushLayer(layer); }
		inline void PushOverlay(Layer* overlay) { LayerStack::PushOverlay(overlay); }

		inline void EnableImguiUI(bool enableDocking = false) {
			QK_CORE_ASSERT(!m_ImguiUiEnabled, "Imgui already Enabled");

			m_ImguiUiEnabled = true;
			ImguiUI::Init(m_Window);
			if (enableDocking) ImguiUI::EnableDocking();
		}

	private:
		std::wstring m_AppName;
		Window		 m_Window;
		bool		 m_Running		  = true;
		bool		 m_ImguiUiEnabled = false;

	private:
		inline static Application* s_Instance = nullptr;
	};

}
