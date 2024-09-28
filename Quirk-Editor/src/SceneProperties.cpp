


#include "SceneProperties.h"


namespace Quirk{

	SceneProperties::SceneProperties(const SceneViewport& sceneViewport, const std::string& name) :
			Layer(name),
			m_SceneViewport(sceneViewport)
	{
	}

	void SceneProperties::OnAttach(){

	}

	void SceneProperties::OnDetach(){

	}

	bool SceneProperties::OnEvent(Event& event){
		return false;
	}

	void SceneProperties::OnUpdate(){

	}

	void SceneProperties::OnImguiUiUpdate(){
		const OrthographicCameraController& cameraController = m_SceneViewport.GetCameraController();
		const glm::vec3& cameraPos = cameraController.GetPosition();
		float rotationAngle = cameraController.GetRotation();

		ImGuiIO& io = ImGui::GetIO();

		ImGui::Begin("Scene Properties");

		ImGui::Text("Camera Properties:- ");               // Display some text (you can use a format strings too)
		ImGui::Text("Camera Position %.2f, %.2f, %.2f", cameraPos.x, cameraPos.y, cameraPos.z);
		ImGui::Text("Camera Rotation %.2f", rotationAngle);

		ImGui::Dummy(ImVec2(20, 5));

		ImGui::Text("Application average time:- ");
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

		ImGui::End();
	}

}

