
#include<qkpch.h>

#include "Core/Application.h"
#include "Core/Core.h"

namespace Quirk {

	Application* Application::s_Instance = nullptr;

	Application::Application() :
		m_Window(Window::Create()),
		m_Running(true)
	{
		s_Instance = this;
		m_Window->SetEventCallback(QK_BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application() {
		delete m_Window;
	}

	void Application::Run() {
		while (m_Running) {
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& event) {
		EventDispatcher::HandleEvent(event);
		EventDispatcher::Dispatch<WindowCloseEvent>(QK_BIND_EVENT_FN(Application::OnWindowClose));

		EventDispatcher::Dispatch<MouseButtonDblClickEvent>([&](MouseButtonDblClickEvent& event) -> bool {
			QK_CORE_INFO("Mouse Pos {} {}", event.GetPosX(), event.GetPosY());
			return true;
		});
	}

	bool Application::OnWindowClose(WindowCloseEvent& event){
		m_Running = false;
		return true;
	}

}
