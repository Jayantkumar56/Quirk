

#include<qkpch.h>

#include "Core/Core.h"
#include "Core/Application/Application.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Renderer2D.h"
#include "Core/Renderer/RenderCommands.h"

#include "imgui.h"
#include "Core/Utility/Time.h"

namespace Quirk {

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::wstring& appName) :
		m_AppName(appName),
		m_Window(appName, 1200, 800, RendererAPI::API::OpenGL),
		m_Running(true)
	{
		s_Instance = this;
		m_Window.SetEventCallback(QK_BIND_EVENT_FN(Application::OnEvent));

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
		EventDispatcher::HandleEvent(event);
		EventDispatcher::Dispatch<WindowCloseEvent>(QK_BIND_EVENT_FN(Application::OnWindowClose));

		EventDispatcher::Dispatch<WindowResizeEvent>(QK_BIND_EVENT_FN(Application::OnWindowResize));

		ImGuiIO& io = ImGui::GetIO();
		uint16_t cat = event.GetEventCategory();

		if ((cat & EventCategory::KeyboardEvent || cat & EventCategory::MouseEvent) && (io.WantCaptureKeyboard || io.WantCaptureMouse)) {
			return;
		}

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
