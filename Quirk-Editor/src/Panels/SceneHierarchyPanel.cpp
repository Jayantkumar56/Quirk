

#include "SceneHierarchyPanel.h"


namespace Quirk {

	void SceneHierarchyPanel::OnImguiUiUpdate(const Ref<Scene>& scene, Entity& selectedEntity){
		ImGui::Begin("Scene Hierarcy");

		for (auto entity : scene->m_Registry.view<entt::entity>()) {
			Entity entityToShow = { entity, scene.get()};

			const std::string& tag = entityToShow.GetComponent<TagComponent>().Tag;
			uint64_t uuid = entityToShow.GetComponent<UUIDComponent>().Uuid;

			ImGuiTreeNodeFlags flags = ((entityToShow == selectedEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

			bool isOpened = ImGui::TreeNodeEx((void*)uuid, flags, tag.c_str());

			if (!ImGui::IsItemToggledOpen() && ImGui::IsItemClicked()) {
				selectedEntity = entityToShow;
			}

			if (isOpened) {
				ImGui::TreePop();
			}
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			selectedEntity = {};
		}

		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
			if (ImGui::MenuItem("Create Empty Entity")) {
				scene->CreateEntity();
			}

			ImGui::EndPopup();
		}

		ImGui::End();
	}

}

