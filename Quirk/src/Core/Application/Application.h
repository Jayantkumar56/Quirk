
#pragma once

#include "Core/Frame/Window.h"
#include "Core/Input/ApplicationEvents.h"
#include "Core/Frame/Frame.h"

namespace Quirk {

	class Application {
	public:
		inline static Application& Get() noexcept { return *s_Instance; }
		inline static const std::filesystem::path& GetWorkingDirectory() noexcept { return s_Instance->m_CurrentWorkingDirectory; }

	public:
		Application(std::wstring appName, RendererAPI::API renderingAPI);
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& event);
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

		// adds a new frame to the frame manager and makes the frame be the current context
		// if called from another frame make sure to reset the context with Frame::MakeContextCurrent()
		template<FrameType T, typename ...Args>
		inline T* AddFrame(Args&& ...args) { return m_FrameManager.AddFrame<T>(std::forward<Args>(args)...); }

	private:
		std::wstring m_AppName;
		bool		 m_Running		  = true;
		bool		 m_ImguiUiEnabled = false;

		FrameManager		  m_FrameManager;
		std::filesystem::path m_CurrentWorkingDirectory;

	private:
		static Application* s_Instance;
	};

}
