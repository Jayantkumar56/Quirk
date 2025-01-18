

#include "EditorFrame.h"
#include "FontManager.h"
#include "Theme.h"

#include "Core/Serializer/SceneSerializer.h"

namespace Quirk {

	void EditorFrame::OnAttach(){
#ifdef _EXAMPLE_SCENE_CREATON_
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

		SceneSerializer::Serialize(m_MainScene, L"assets/Scenes/main_scene.yaml");
		SceneSerializer::Deserialize(m_MainScene, L"assets/Scenes/main_scene.yaml");
#endif // _EXAMPLE_SCENE_CREATON_
	}

	void EditorFrame::OnDetach(){

	}

	bool EditorFrame::OnEvent(Event& event){
		return false;
	}

	void EditorFrame::OnUpdate(){

	}

	void EditorFrame::OnImguiUiUpdate(){
		// Disabling alt key for imgui to prevent navigation with alt key (problems when using editor cotrols)
		ImGui::SetKeyOwner(ImGuiKey_LeftAlt, ImGuiKeyOwner_Any, ImGuiInputFlags_LockThisFrame);

		m_TitleBar.OnImguiUiUpdate(m_MainScene);
	}

}

