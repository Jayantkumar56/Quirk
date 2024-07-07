
#include<qkpch.h>

#include "Core/Application.h"


namespace Quirk {
	Application* Application::s_Instance = nullptr;

	Application::Application() :
		m_Window(Window::Create()),
		m_Running(true)
	{
		s_Instance = this;
	}

	Application::~Application() {
	}

	void Application::Run() {

		while (m_Running) {
			m_Window->OnUpdate();
		}

	}

}
