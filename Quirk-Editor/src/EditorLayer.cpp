

#include "EditorLayer.h"

namespace Quirk {

	void EditorLayer::OnAttach(){
		auto square = m_MainScene->CreateEntity("Orange Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.14f, 0.1f, 1.0f });

		auto camera = m_MainScene->CreateEntity("Camera A");
		auto& cameraComponent = camera.AddComponent<CameraComponent>();
		cameraComponent.Primary = true;

		auto camera2 = m_MainScene->CreateEntity("Camera B");
		auto& cameraComponent2 = camera2.AddComponent<CameraComponent>();

		class CameraController : public ScriptableEntity {
		public:
			float cameraSpeed = 10.0f;

			void OnCreate() {
			}

			void OnDestroy() {
			}

			void OnUpdate() {
				auto& transform = GetComponent<TransformComponent>();

				double deltaTime = Time::GetDeltaTime();
				double cameraDisplacement = cameraSpeed * deltaTime;

				if (Input::IsKeyPressed(QK_Key_S)) {
					transform.Translation.y -= static_cast<float>(cameraDisplacement);
				}
				if (Input::IsKeyPressed(QK_Key_W)) {
					transform.Translation.y += static_cast<float>(cameraDisplacement);
				}
				if (Input::IsKeyPressed(QK_Key_A)) {
					transform.Translation.x -= static_cast<float>(cameraDisplacement);
				}
				if (Input::IsKeyPressed(QK_Key_D)) {
					transform.Translation.x += static_cast<float>(cameraDisplacement);
				}
			}
		};

		camera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		camera2.AddComponent<NativeScriptComponent>().Bind<CameraController>();
	}

	void EditorLayer::OnDetach(){

	}

	bool EditorLayer::OnEvent(Event& event){
		m_SceneViewport.OnEvent(event);
		return false;
	}

	void EditorLayer::OnUpdate(){
		m_SceneViewport.OnUpdate();
	}

	void EditorLayer::OnImguiUiUpdate(){
		m_SceneViewport.OnImguiUiUpdate();
		m_SceneHierarcy.OnImguiUiUpdate(m_MainScene, m_SelectedEntity);
		m_InspectorPanel.OnImguiUiUpdate(m_SelectedEntity);
	}

}

