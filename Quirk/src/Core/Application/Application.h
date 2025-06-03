
#pragma once

#include "Core/Input/ApplicationEvents.h"
#include "Core/Frame/FrameManager.h"
#include "Core/Renderer/Renderer.h"

namespace Quirk {

	class Application {
	public:
        template<typename T>
        requires std::derived_from<T, Application>
        inline static T& GetAs() noexcept {
            QK_CORE_ASSERT(s_Instance, "Application object is not instanciated yet!");
            return *(static_cast<T*>(s_Instance));
        }

		inline static Application& Get() noexcept {
            QK_CORE_ASSERT(s_Instance, "Application object is not instanciated yet!");
            return *s_Instance;
        }

		inline static const std::filesystem::path& GetWorkingDirectory() noexcept {
            QK_CORE_ASSERT(s_Instance, "Application object is not instanciated yet!");
            return s_Instance->m_CurrentWorkingDirectory;
        }

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

		static FrameManager& GetFrameManager() { return s_Instance->m_FrameManager; }

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
