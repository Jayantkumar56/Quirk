


#include "SceneViewport.h"

namespace Quirk {

	SceneViewport::SceneViewport(const std::string& name):
			m_ViewportWidth(890),
			m_ViewportHeight(800),
			Layer(name),
			m_Frame(FrameBuffer::Create({ m_ViewportWidth, m_ViewportHeight})),
			m_CameraSpeed(8.0f),
			m_CameraController(m_ViewportWidth / m_ViewportHeight),
			m_RendererStats({ 0, 0 }),
			m_IsInFocus(false)

	{
		m_CameraController.SetTranslationSpeed(m_CameraSpeed);
		RenderCommands::UpdateViewPort(m_ViewportWidth, m_ViewportHeight);

		m_QuadTexture = Texture2D::Create("assets/Images/container.jpg");

		QuadFormat format;
		format.Width = 500;
		format.Height = 500;
		format.Position = { 0.0f, 0.0f, 0.0f };
		//format.Color = { 0.0f, 0.0f, 1.0f, 1.0f };
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
		if (!m_IsInFocus)
			return false;

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

		if(EventDispatcher::GetCurrentEventType() != EventType::WindowResizeEvent)
			m_CameraController.OnEvent(event);

		return false;
	}

	void SceneViewport::OnUpdate() {
		if (!m_IsInFocus) {
			return;
		}

		m_CameraController.OnUpdate();
	}

	void SceneViewport::OnImguiUiUpdate() {
		ImGuiIO& io = ImGui::GetIO();
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Scene Viewport");

		m_IsInFocus = ImGui::IsWindowFocused();
		CheckAndHandleResize();
		RenderViewport();

		uint32_t color = m_Frame->GetColorBuffer();
		ImGui::Image((void*)color, ImVec2(m_ViewportWidth, m_ViewportHeight), { 0, 1 }, { 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void SceneViewport::CheckAndHandleResize() {
		ImVec2 windowSize = ImGui::GetContentRegionAvail();

		if (windowSize.x < 0 || windowSize.y < 0) {
			return;
		}

		if (m_ViewportWidth != (int)windowSize.x || m_ViewportHeight != (int)windowSize.y) {
			m_ViewportWidth  = (int)windowSize.x;
			m_ViewportHeight = (int)windowSize.y;

			m_Frame->Resize(m_ViewportWidth, m_ViewportHeight);
			RenderCommands::UpdateViewPort(m_ViewportWidth, m_ViewportHeight);
			m_CameraController.HandleWindowResize(m_ViewportWidth, m_ViewportHeight);
		}
	}

	void SceneViewport::RenderViewport() {
		m_Frame->Bind();
		RenderCommands::Clear();
		Renderer2D::ResetStats();
		Renderer2D::BeginScene(m_CameraController.GetCamera());

		for (int i = 0; i < m_Rectangle.size(); ++i) {
			Renderer2D::Submit(m_Rectangle[i]);
		}

		Renderer2D::EndScene();
		m_Frame->Unbind();
		m_RendererStats = Renderer2D::GetStats();
	}

}

