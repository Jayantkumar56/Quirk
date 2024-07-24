
#pragma once

#include "Core/Application/Window.h"
#include "Core/Application/Layer.h"

#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/Shader.h"

namespace Quirk {

	class Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& event);
		bool OnWindowClose(WindowCloseEvent& event);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return m_Window; }

	private:
		Window			m_Window;
		bool			m_Running;
		LayerStack		m_LayerStack;
		Layer*			m_ImguiLayer;
		VertexArray*	m_SquareVA;
		Shader*			m_BlueShader;

	private:
		static Application* s_Instance;
	};

}
