

#include "SceneHierarchyPanel.h"
#include "FontManager.h"

namespace Quirk {

	void SceneHierarchyPanel::OnImguiUiUpdate(const Ref<Scene>& scene, Entity& selectedEntity){
		ImGui::Begin("Scene Hierarcy");

		for (auto entity : scene->m_Registry.view<entt::entity>()) {
			Entity entityToShow = { entity, scene.get()};
			DrawEntityNode(entityToShow, selectedEntity);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			selectedEntity = {};
		}

		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
			if (ImGui::MenuItem("Add Empty Entity")) {
				scene->CreateEntity();
			}

			ImGui::EndPopup();
		}

		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("Per frame %.3f ms/frame", 1000.0f / io.Framerate);
		ImGui::Text("FPS :- %.1f", io.Framerate);

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity, Entity& selectedEntity) {
		const std::string& tag = entity.GetComponent<TagComponent>().Tag;
		uint64_t uuid = entity.GetComponent<UUIDComponent>().Uuid;

		ImGuiTreeNodeFlags flags = ((entity == selectedEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |=  ImGuiTreeNodeFlags_SpanAvailWidth;

		bool isOpened = ImGui::TreeNodeEx((void*)uuid, flags, tag.c_str());

		if (!ImGui::IsItemToggledOpen() && ImGui::IsItemClicked()) {
			selectedEntity = entity;
		}

		if (isOpened) {
			ImGui::TreePop();
		}
	}

}

