
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

		template<FrameType T, typename ...Args>
		inline void AddFrame(Args&& ...args) { m_FrameManager.AddFrame<T>(std::forward<Args>(args)...); }

	private:
		std::wstring m_AppName;
		bool		 m_Running		  = true;
		bool		 m_ImguiUiEnabled = false;
		FrameManager m_FrameManager;

	private:
		static Application* s_Instance;
	};

}
