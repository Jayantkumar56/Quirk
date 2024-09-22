
#include "Example2D.h"

Example2DLayer::Example2DLayer(const std::string& name) :
	Quirk::Layer(name),
	m_CameraController(1200.0f / 800.0f)
{
	Quirk::Ref<Quirk::Texture2D> texture = Quirk::Texture2D::Create("assets/Images/container.jpg");

	Quirk::QuadFormat format;
	format.Width = 500;
	format.Height = 500;
	format.Position = { 0.0f, 0.0f, 0.0f };
	format.Color = { 0.0f, 0.0f, 1.0f, 1.0f };
	format.Texture2d = texture;

	m_Rectangle.push_back(std::make_shared<Quirk::Quad>(format));

	/*for (int i = -590; i <= 590; i += 10) {
		for (int j = -390; j <= 390; j += 10) {
			format.Position = { i, j, 0.0f };
			m_Rectangle.push_back(std::make_shared<Quirk::Quad>(format));
		}
	}*/
}

void Example2DLayer::OnAttach() {

}
void Example2DLayer::OnDetach() {

}

bool Example2DLayer::OnEvent(Quirk::Event& event) {
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

void Example2DLayer::OnImguiUiUpdate()  {
	float rotationAngle = m_CameraController.GetRotation();
	glm::vec3 cameraPos = m_CameraController.GetPosition();

	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground;

	ImGui::Begin("Hello, world!", nullptr, flags);          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("Camera Properties:-");               // Display some text (you can use a format strings too)
	ImGui::DragFloat3("Camera Position", glm::value_ptr(cameraPos), 0.1f);
	ImGui::DragFloat("Camera Pitch", &rotationAngle, 0.1f);

	ImGui::Text("No of Quads Drawn:- %d", m_RendererStats.QuadsDrawn);
	ImGui::Text("No of Draw Calls :- %d", m_RendererStats.NoOfDrawCalls);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

	ImGui::End();
}

void Example2DLayer::OnUpdate()  {
	m_CameraController.OnUpdate();

	Quirk::Renderer2D::ResetStats();
	Quirk::Renderer2D::BeginScene(m_CameraController.GetCamera());

	for (int i = 0; i < m_Rectangle.size(); ++i) {
		Quirk::Renderer2D::Submit(m_Rectangle[i]);
	}

	Quirk::Renderer2D::EndScene();
	m_RendererStats = Quirk::Renderer2D::GetStats();
}
