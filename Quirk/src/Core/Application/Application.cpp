
#include<qkpch.h>

#include "Application.h"
#include "Core/Core.h"

#include "Core/Imgui/ImguiLayer.h"
#include "Core/Input/MouseEvents.h"

// temporary till we have renderer
#include "glad/glad.h"

namespace Quirk {

	Application* Application::s_Instance = nullptr;

	Application::Application() :
		m_Window(L"Quirk Engine!!!!", 1200, 800),
		m_Running(true)
	{
		s_Instance = this;
		m_Window.SetEventCallback(QK_BIND_EVENT_FN(Application::OnEvent));

		m_ImguiLayer = new ImguiLayer();
		m_LayerStack.PushLayer(m_ImguiLayer);
		glClearColor(0.10156f, 0.17968f, 0.20703f, 1.0f);
	}

	Application::~Application() {
		
	}

	void Application::Run() {
		while (m_Running) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_LayerStack.UpdateLayers();
			m_Window.OnUpdate();
		}
	}

	void Application::OnEvent(Event& event) {
		EventDispatcher::HandleEvent(event);
		EventDispatcher::Dispatch<WindowCloseEvent>(QK_BIND_EVENT_FN(Application::OnWindowClose));

		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureKeyboard || io.WantCaptureMouse) {
			return;
		}

		EventDispatcher::Dispatch<MouseButtonPressedEvent>([&](MouseButtonPressedEvent& event) -> bool {
			QK_CORE_INFO("Mouse Pos {} {}", event.GetPosX(), event.GetPosY());
			return true;
		});

		m_LayerStack.HandleEvent(event);
	}

	bool Application::OnWindowClose(WindowCloseEvent& event){
		m_Running = false;
		return true;
	}

}
