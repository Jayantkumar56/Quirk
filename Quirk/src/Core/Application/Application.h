
#pragma once

#include "Core/Application/Window.h"
#include "Core/Input/ApplicationEvents.h"
#include "Core/Application/Frame.h"

namespace Quirk {

	class Application {
	public:
		Application(std::wstring appName, RendererAPI::API renderingAPI);
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& event);
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow()		 { return m_FrameManager.GetWindow(); }

		inline void AddFrame(Frame* frame)		{ m_FrameManager.AddFrame(frame); }

	private:
		std::wstring m_AppName;
		bool		 m_Running		  = true;
		bool		 m_ImguiUiEnabled = false;
		FrameManager m_FrameManager;

	private:
		static Application* s_Instance;
	};

}
