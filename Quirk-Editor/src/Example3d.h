

#pragma once

#include "Quirk.h"

class Example3DLayer : public Layer {
public:
	Example3DLayer() :
		Layer("Example Layer"),
		m_CameraController(45.0f, 1200.0f / 800.0f, 0.1f, 100.0f),
		m_SquareVA(VertexArray::Create())
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
			-0.5f, 0.5f,  0.5f,		1.0f, 0.0f, 0.0f, // top-right
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

		Ref<VertexBuffer> squareVB(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Color" }
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

		Ref<IndexBuffer> squareIB(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(squareIndices[0])));
		m_SquareVA->SetIndexBuffer(squareIB);

		m_Shader = ShaderLibrary::LoadShader("assets/shader.glsl");
	}

	virtual void OnAttach() override {

	}

	virtual void OnDetach() override {

	}

	virtual bool OnEvent(Event& event) override {
		EventDispatcher::Dispatch<KeyPressedEvent>([&](KeyPressedEvent& e) -> bool {
			if (e.GetKeyCode() == QK_Key_L) {
				Cursor::HideCursor();
				Cursor::PlaceAtCenter();
				Cursor::LockCursor();
			}

			if (e.GetKeyCode() == QK_Key_Escape) {
				Cursor::UnlockCursor();
				Cursor::ShowCursor();
			}

			if (e.GetKeyCode() == QK_Key_T) {
				Cursor::PlaceAtCenter();
			}

			return false;
			});

		m_CameraController.OnEvent(event);

		return false;
	}

	virtual void OnImguiUiUpdate() override {
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

		Renderer::BeginScene(m_CameraController.GetCamera());
		Renderer::Submit(m_Shader, m_SquareVA);
		Renderer::EndScene();
	}

private:
	PerspectiveCameraController m_CameraController;

	Ref<VertexArray> m_SquareVA;
	Ref<Shader> m_Shader;

	float m_CameraSpeed = 3.0f;
};
