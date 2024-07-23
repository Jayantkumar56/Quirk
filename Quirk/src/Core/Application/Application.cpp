

#include<qkpch.h>

#include "Application.h"
#include "Core/Core.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Imgui/ImguiLayer.h"


namespace Quirk {

	Application* Application::s_Instance = nullptr;

	Application::Application() :
		m_Window(L"Quirk Engine!!!!", 1200, 800),
		m_Running(true)
	{
		s_Instance = this;
		m_Window.SetEventCallback(QK_BIND_EVENT_FN(Application::OnEvent));

		Renderer::InitRenderer(RendererAPI::API::OpenGL);
		RenderCommands::SetClearColor({ 0.10156f, 0.17968f, 0.20703f, 1.0f });

		m_ImguiLayer = new ImguiLayer();
		m_LayerStack.PushLayer(m_ImguiLayer);
	}

	Application::~Application() {
		delete m_ImguiLayer;
	}

	void Application::Run() {
		while (m_Running) {
			RenderCommands::Clear();

			m_Window.OnUpdate();
			m_LayerStack.UpdateLayers();

			RenderCommands::SwapBuffers();
		}
	}

	void Application::OnEvent(Event& event) {
		EventDispatcher::HandleEvent(event);
		EventDispatcher::Dispatch<WindowCloseEvent>(QK_BIND_EVENT_FN(Application::OnWindowClose));

		ImGuiIO& io = ImGui::GetIO();
		uint16_t cat = event.GetEventCategory();
		if ((cat | EventCategory::KeyboardEvent || cat | EventCategory::MouseEvent) && (io.WantCaptureKeyboard || io.WantCaptureMouse)) {
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
