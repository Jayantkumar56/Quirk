


#include "SceneViewport.h"
#include "Core/Application/Application.h"
#include "Core/Imgui/ImguiUI.h"
#include "Core/Renderer/RenderCommands.h"

namespace Quirk {

	SceneViewport::SceneViewport(const std::string& name):
			Layer(name),
			m_Frame(FrameBuffer::Create({ 890, 800})),
			m_CameraSpeed(8.0f),
			m_CameraController(890 / 800),
			m_RendererStats({ 0, 0 })
	{
		m_CameraController.SetTranslationSpeed(m_CameraSpeed);
		RenderCommands::UpdateViewPort(890, 800);

		m_QuadTexture = Texture2D::Create("assets/Images/container.jpg");

		QuadFormat format;
		format.Width = 500;
		format.Height = 500;
		format.Position = { 0.0f, 0.0f, 0.0f };
		format.Color = { 0.0f, 0.0f, 1.0f, 1.0f };
		format.Texture2d = m_QuadTexture;

		m_Rectangle.push_back(std::make_shared<Quad>(format));

		/*for (int i = -590; i <= 590; i += 10) {
			for (int j = -390; j <= 390; j += 10) {
				format.Position = { i, j, 0.0f };
				m_Rectangle.push_back(std::make_shared<Quad>(format));
			}
		}*/
	}

	void SceneViewport::OnAttach() {

	}

	void SceneViewport::OnDetach() {

	}

	bool SceneViewport::OnEvent(Event& event) {
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

	void SceneViewport::OnUpdate() {
		m_CameraController.OnUpdate();

		m_Frame->Bind();
		Renderer2D::ResetStats();
		Renderer2D::BeginScene(m_CameraController.GetCamera());

		for (int i = 0; i < m_Rectangle.size(); ++i) {
			Renderer2D::Submit(m_Rectangle[i]);
		}

		Renderer2D::EndScene();
		m_Frame->Unbind();
		m_RendererStats = Renderer2D::GetStats();
	}

	void SceneViewport::OnImguiUiUpdate() {
		ImGuiIO& io = ImGui::GetIO();
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration;

		ImGui::Begin("Scene Viewport");

		uint32_t color = m_Frame->GetColorBuffer();
		ImGui::Image((void*)color, ImVec2(890.0f, 800.0f));

		ImGui::End();

		bool showdemo = true;
		ImGui::ShowDemoWindow(&showdemo);
	}

}

