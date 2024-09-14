
#include "Quirk.h"

class ExampleLayer : public Quirk::Layer {
public:
	ExampleLayer() : 
			Layer("Example Layer"),
			m_CameraController(45.0f, 1200.0f / 800.0f, 0.1f, 100.0f),
			m_SquareVA(Quirk::VertexArray::Create())
	{
		float squareVertices[] = {
			// back face
			-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f, // bottom-left
			 0.5f,  0.5f, -0.5f,		1.0f, 0.0f, 0.0f, // top-right
			 0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f, // bottom-right
			 0.5f,  0.5f, -0.5f,		1.0f, 0.0f, 0.0f, // top-right
			-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f, // bottom-left
			-0.5f,  0.5f, -0.5f,		1.0f, 0.0f, 0.0f, // top-left
			// front face					  			
			-0.5f, -0.5f, 0.5f,			1.0f, 0.0f, 0.0f, // bottom-left
			 0.5f, -0.5f, 0.5f,			1.0f, 0.0f, 0.0f, // bottom-right
			 0.5f,  0.5f, 0.5f,			1.0f, 0.0f, 0.0f, // top-right
			 0.5f,  0.5f, 0.5f,			1.0f, 0.0f, 0.0f, // top-right
			-0.5f,  0.5f, 0.5f,			1.0f, 0.0f, 0.0f, // top-left
			-0.5f, -0.5f, 0.5f,			1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			- 0.5f, 0.5f,  0.5f,		1.0f, 0.0f, 0.0f, // top-right
			-0.5f,  0.5f, -0.5f,		1.0f, 0.0f, 0.0f, // top-left
			-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f, // bottom-left
			-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f, // bottom-left
			-0.5f, -0.5f,  0.5f,		1.0f, 0.0f, 0.0f, // bottom-right
			-0.5f,  0.5f,  0.5f,		1.0f, 0.0f, 0.0f, // top-right
			// right face					  		
			 0.5f,  0.5f,  0.5f,		1.0f, 0.0f, 0.0f, // top-left
			 0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f, // bottom-right
			 0.5f,  0.5f, -0.5f,		1.0f, 0.0f, 0.0f, // top-right
			 0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f, // bottom-right
			 0.5f,  0.5f,  0.5f,		1.0f, 0.0f, 0.0f, // top-left
			 0.5f, -0.5f,  0.5f,		1.0f, 0.0f, 0.0f, // bottom-left
			// bottom face					  	
			-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f, // top-right
			 0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f, // top-left
			 0.5f, -0.5f,  0.5f,		1.0f, 0.0f, 0.0f, // bottom-left
			 0.5f, -0.5f,  0.5f,		1.0f, 0.0f, 0.0f, // bottom-left
			-0.5f, -0.5f,  0.5f,		1.0f, 0.0f, 0.0f, // bottom-right
			-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f, // top-right
			// top face						  	
			-0.5f,  0.5f, -0.5f,		1.0f, 0.0f, 0.0f, // top-left
			 0.5f,  0.5f,  0.5f,		1.0f, 0.0f, 0.0f, // bottom-right
			 0.5f,  0.5f, -0.5f,		1.0f, 0.0f, 0.0f, // top-right
			 0.5f,  0.5f,  0.5f,		1.0f, 0.0f, 0.0f, // bottom-right
			-0.5f,  0.5f, -0.5f,		1.0f, 0.0f, 0.0f, // top-left
			-0.5f,  0.5f,  0.5f,		1.0f, 0.0f, 0.0f  // bottom-left
		};

		Quirk::Ref<Quirk::VertexBuffer> squareVB(Quirk::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Quirk::ShaderDataType::Float3, "a_Position" },
			{ Quirk::ShaderDataType::Float3, "a_Color" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[] = { 
			0, 1, 2,	4, 5, 6,
			2, 3, 0,	6, 7, 4,

			0, 1, 4,	2, 3, 6,
			4, 5, 1,	6, 7, 3,

			/*1, 5, 2,	0, 3, 4,
			5, 2, 6,	3, 4, 7*/
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
				Quirk::Cursor::HideCursor();
				Quirk::Cursor::PlaceAtCenter();
				Quirk::Cursor::LockCursor();
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

		m_CameraController.OnEvent(event);

		return false;
	}

	virtual void OnImguiUiUpdate() override{
		float pitch = m_CameraController.GetPitch(), yaw = m_CameraController.GetYaw();
		glm::vec3 cameraPos = m_CameraController.GetPosition();

		ImGuiIO& io = ImGui::GetIO();
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground;

		ImGui::Begin("Hello, world!", nullptr, flags);          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("Camera Properties:-");               // Display some text (you can use a format strings too)
		ImGui::DragFloat3("Camera Position", glm::value_ptr(cameraPos), 0.1f);
		ImGui::DragFloat("Camera Pitch", &pitch, 0.1f);
		ImGui::DragFloat("Camera Yaw", &yaw, 0.1f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	virtual void OnUpdate() override {
		m_CameraController.OnUpdate();

		Quirk::Renderer::BeginScene(m_CameraController.GetCamera());
		Quirk::Renderer::Submit(m_Shader, m_SquareVA);
		Quirk::Renderer::EndScene();
	}

private:
	Quirk::PerspectiveCameraController m_CameraController;

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

		GetWindow().SetVSync(true);
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

