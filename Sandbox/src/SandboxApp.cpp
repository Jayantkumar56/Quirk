
#include "Quirk.h"

class ExampleLayer : public Quirk::Layer {
public:
	ExampleLayer() : 
			Layer("Example Layer"),
			m_CameraController(45.0f, 1200.0f / 800.0f, 0.1f, 100.0f),
			m_SquareVA(Quirk::VertexArray::Create())
	{
		float squareVertices[3 * 8] = {
			/* 0 */		-0.50f, -0.50f,  0.50f,
			/* 1 */		 0.50f, -0.50f,  0.50f,
			/* 2 */		 0.50f,  0.50f,  0.50f,
			/* 3 */		-0.50f,  0.50f,  0.50f,
			/* 4 */		-0.50f, -0.50f, -0.50f,
			/* 5 */		 0.50f, -0.50f, -0.50f,
			/* 6 */		 0.50f,  0.50f, -0.50f,
			/* 7 */		-0.50f,  0.50f, -0.50f 
		};

		Quirk::Ref<Quirk::VertexBuffer> squareVB(Quirk::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Quirk::ShaderDataType::Float3, "a_Position" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[3 * 8] = { 
			0, 1, 2,	4, 5, 6,
			2, 3, 0,	6, 7, 4,

			0, 1, 4,	2, 3, 6,
			4, 5, 1,	6, 7, 3,


		};

		Quirk::Ref<Quirk::IndexBuffer> squareIB(Quirk::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(squareIndices[0])));
		m_SquareVA->SetIndexBuffer(squareIB);

		m_Shader = m_ShaderLibrary.LoadShader("assets/shader.glsl");
	}

	virtual void OnAttach() override {

	}

	virtual void OnDetach() override{

	}

	virtual bool OnEvent(Quirk::Event& event) override{
		Quirk::EventDispatcher::Dispatch<Quirk::KeyPressedEvent>([&](Quirk::KeyPressedEvent& e) -> bool {
			if (e.GetKeyCode() == QK_Key_L) {
				Quirk::Cursor::PlaceAtCenter();
				Quirk::Cursor::LockCursor();
				Quirk::Cursor::HideCursor();
			}

			if (e.GetKeyCode() == QK_Key_Escape) {
				Quirk::Cursor::UnlockCursor();
				Quirk::Cursor::ShowCursor();
			}

			if (e.GetKeyCode() == QK_Key_T) {
				Quirk::Cursor::PlaceAtCenter();
			}

			return false;
		});

		return false;
	}

	virtual void OnImguiUiUpdate() override{
		ImGuiIO& io = ImGui::GetIO();

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

			ImGui::DragFloat3("Camera Position", glm::value_ptr(m_CameraController.GetPosition()), 0.1f);

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}
	}

	virtual void OnUpdate() override {
		m_CameraController.OnUpdate();

		Quirk::Renderer::BeginScene(m_CameraController.GetCamera());
		Quirk::Renderer::Submit(m_Shader, m_SquareVA);
		Quirk::Renderer::EndScene();
	}

private:
	Quirk::PerspectiveCameraController m_CameraController;
	//Quirk::OrthographicCamera m_OrthoCamera;
	Quirk::Ref<Quirk::VertexArray> m_SquareVA;
	Quirk::ShaderLibrary m_ShaderLibrary;
	Quirk::Ref<Quirk::Shader> m_Shader;

	float m_CameraSpeed = 3.0f;
};


class SandboxApp : public Quirk::Application {
public:
	SandboxApp() {
		PushLayer(&m_ExampleLayer);
		PushOverlay(&m_ImguiLayer);
	}

	~SandboxApp() {

	}

private:
	ExampleLayer m_ExampleLayer;
	Quirk::ImguiLayer m_ImguiLayer;
};


// Quirk::LaunchApp() must be defined in the application
// Create app object inside this function and call Run method
void Quirk::LaunchApp() {
	SandboxApp app;
	app.Run();
}

