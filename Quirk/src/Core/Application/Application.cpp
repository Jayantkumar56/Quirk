

#include<qkpch.h>

#include "Core/Core.h"
#include "Core/Input/Events.h"
#include "Core/Input/MouseEvents.h"
#include "Core/Application/Application.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Renderer2D.h"
#include "Core/Renderer/RenderCommands.h"

#include "Core/Utility/Time.h"

namespace Quirk {

	Application* Application::s_Instance = nullptr;

	Application::Application(std::wstring appName, RendererAPI::API renderingAPI) :
			m_AppName(std::move(appName))
	{
		s_Instance = this;
		EventDispatcher::RegisterEventCallback(QK_BIND_EVENT_FN(Application::OnEvent));

		GraphicalContext::Init(renderingAPI);
		RenderCommands::Init(renderingAPI);

		Renderer::InitRenderer(renderingAPI);
	}

	void Application::Run() {
		Renderer2D::InitRenderer();

		while (m_Running) {
			Time::RefreshTime();
			RenderCommands::Clear();

			m_FrameManager.UpdateFrames();
			m_FrameManager.UpdateImguiUiFrames();
		}
	}

	void Application::OnEvent(Event& event) {
		EventDispatcher::HandleEvent<WindowCloseEvent>(QK_BIND_EVENT_FN(Application::OnWindowClose));
		EventDispatcher::HandleEvent<WindowResizeEvent>(QK_BIND_EVENT_FN(Application::OnWindowResize));

		m_FrameManager.HandleEvent(event);
	}

	bool Application::OnWindowClose(WindowCloseEvent& event){
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event) {
		RenderCommands::UpdateViewPort(event.GetWidth(), event.GetHeight());
		return false;
	}

}
