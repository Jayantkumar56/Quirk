
#include "Quirk.h"

class ExampleLayer : public Quirk::Layer {
public:
	ExampleLayer() : 
			Layer("Example Layer"),
			m_OrthoCamera(1.5, -1.5, 1.0, -1.0),
			m_SquareVA(Quirk::VertexArray::Create())
	{
		float squareVertices[3 * 4] = {
			-0.50f, -0.50f, 0.0f,
			 0.50f, -0.50f, 0.0f,
			 0.50f,  0.50f, 0.0f,
			-0.50f,  0.50f, 0.0f
		};

		Quirk::Ref<Quirk::VertexBuffer> squareVB(Quirk::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Quirk::ShaderDataType::Float3, "a_Position" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		Quirk::Ref<Quirk::IndexBuffer> squareIB(Quirk::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(squareIndices[0])));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec4 a_Position;
			uniform mat4 u_ViewProjection;

			void main() {
				gl_Position = u_ViewProjection * a_Position;
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			out vec4 color;

			void main() {
				color = vec4(0.823, 0.490, 0.176, 1.0);
			}
		)";

		m_BlueShader.reset(Quirk::Shader::Create(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	virtual void OnAttach() override {

	}

	virtual void OnDetach() override{

	}

	virtual bool OnEvent(Quirk::Event& event) override{
		return false;
	}

	virtual void OnImguiUiUpdate() override{

	}

	virtual void OnUpdate() override {
		if(Quirk::Input::IsKeyPressed(QK_Key_W)) {
			m_OrthoCamera.SetPosition(m_OrthoCamera.GetPosition() + glm::vec3(0.0f, 0.001f, 0.0f));
		}
		if (Quirk::Input::IsKeyPressed(QK_Key_S)) {
			m_OrthoCamera.SetPosition(m_OrthoCamera.GetPosition() - glm::vec3(0.0f, 0.001f, 0.0f));
		}
		if (Quirk::Input::IsKeyPressed(QK_Key_A)) {
			m_OrthoCamera.SetPosition(m_OrthoCamera.GetPosition() + glm::vec3(0.001f, 0.0f, 0.0f));
		}
		if (Quirk::Input::IsKeyPressed(QK_Key_D)) {
			m_OrthoCamera.SetPosition(m_OrthoCamera.GetPosition() - glm::vec3(0.001f, 0.0f, 0.0f));
		}

		Quirk::Renderer::BeginScene(m_OrthoCamera);
		Quirk::Renderer::Submit(m_BlueShader, m_SquareVA);
		Quirk::Renderer::EndScene();
	}


private:
	Quirk::OrthographicCamera m_OrthoCamera;
	Quirk::Ref<Quirk::VertexArray> m_SquareVA;
	Quirk::Ref<Quirk::Shader> m_BlueShader;
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

