

#include<qkpch.h>

#include "Application.h"
#include "Core/Core.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Imgui/ImguiLayer.h"

// temporary
#include "glad/glad.h"


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

		m_SquareVA = VertexArray::Create();

		float squareVertices[3 * 4] = {
			-0.50f, -0.50f, 0.0f,
			 0.50f, -0.50f, 0.0f,
			 0.50f,  0.50f, 0.0f,
			-0.50f,  0.50f, 0.0f
		};

		VertexBuffer* squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		IndexBuffer* squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(squareIndices[0]));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_BlueShader = Shader::Create(blueShaderVertexSrc, blueShaderFragmentSrc);
	}

	Application::~Application() {
		delete m_ImguiLayer;
	}

	void Application::Run() {
		while (m_Running) {
			RenderCommands::Clear();
			m_Window.OnUpdate();

			m_BlueShader->Bind();
			m_SquareVA->Bind();
			glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

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
