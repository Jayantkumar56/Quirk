

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

	Application::Application(const std::wstring& appName) :
			m_AppName(appName),
			m_Window({ appName, 1600, 900, 200, 50, false })
	{
		m_Window.CreateGraphicalContext(RendererAPI::API::OpenGL);
		//m_Window.SetAsCurrentContext();

		s_Instance = this;
		EventDispatcher::RegisterEventCallback(QK_BIND_EVENT_FN(Application::OnEvent));

		Renderer::InitRenderer(RendererAPI::API::OpenGL);
		Renderer2D::InitRenderer();
	}

	void Application::Run() {
		while (m_Running) {
			Time::RefreshTime();

			RenderCommands::Clear();

			m_Window.OnUpdate();
			LayerStack::UpdateLayers();
			LayerStack::UpdateImguiUiLayers();

			m_Window.SwapBuffers();
		}
	}

	void Application::OnEvent(Event& event) {
		EventDispatcher::HandleEvent<WindowCloseEvent>(QK_BIND_EVENT_FN(Application::OnWindowClose));
		EventDispatcher::HandleEvent<WindowResizeEvent>(QK_BIND_EVENT_FN(Application::OnWindowResize));

		LayerStack::HandleEvent(event);
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
