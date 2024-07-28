

#include<qkpch.h>

#include "Core/Core.h"
#include "Core/Application/Application.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/RenderCommands.h"

#include "imgui.h"
#include "Core/Utility/Time.h"

namespace Quirk {

	Application* Application::s_Instance = nullptr;

	Application::Application() :
		m_Window(L"Quirk Engine!!!!", 1200, 800),
		m_Running(true)
	{
		s_Instance = this;
		m_Window.SetEventCallback(QK_BIND_EVENT_FN(Application::OnEvent));

		Renderer::InitRenderer(RendererAPI::API::OpenGL);
	}

	void Application::Run() {
		while (m_Running) {
			Time::RefreshTime();

			RenderCommands::Clear();

			m_Window.OnUpdate();
			m_LayerStack.UpdateLayers();
			m_LayerStack.UpdateImguiUiLayers();

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
