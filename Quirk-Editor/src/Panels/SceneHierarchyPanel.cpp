

#include "SceneHierarchyPanel.h"
#include "FontManager.h"

#include <imgui_internal.h>

namespace Quirk {

	void SceneHierarchyPanel::OnImguiUiUpdate(const Ref<Scene>& scene, Entity& selectedEntity){
		ImGui::Begin("Scene Hierarcy");

		for (auto entity : scene->m_Registry.view<entt::entity>()) {
			Entity entityToShow = { entity, scene.get()};
			DrawEntityNode(entityToShow, selectedEntity);
		}

		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
			m_PopupOpened = true;

			if (ImGui::MenuItem("Add Empty Entity")) {
				scene->CreateEntity();
			}

			ImGui::EndPopup();
		}

		if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) { selectedEntity = {}; }
		m_PopupOpened = false;

		ImGui::Dummy({0.0f, 10.0f});
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("Per frame %.3f ms/frame", 1000.0f / io.Framerate);
		ImGui::Text("FPS :- %.1f", io.Framerate);

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity, Entity& selectedEntity) {
		bool shouldDeleteEntity = false;
		float windowPadding		= GImGui->Style.WindowPadding.x;
		const std::string& tag	= entity.GetComponent<TagComponent>().Tag;
		uint64_t uuid			= entity.GetComponent<UUIDComponent>().Uuid;

		ImGuiTreeNodeFlags flags = 0;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth	| ImGuiTreeNodeFlags_AllowItemOverlap;
		flags |= ImGuiTreeNodeFlags_FramePadding	| ImGuiTreeNodeFlags_OpenOnArrow;

		if (entity == selectedEntity) {
			flags |= ImGuiTreeNodeFlags_Selected;
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f });
		}

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 5));
		bool treeNodeOpened  = ImGui::TreeNodeEx((void*)uuid, flags, tag.c_str());
		bool treeNodeClicked = ImGui::IsItemClicked();
		bool treeToggledOpen = ImGui::IsItemToggledOpen();

		if (entity == selectedEntity) {
			ImGui::PopStyleColor();
		}

		float lineHeight = GImGui->Font->FontSize + 2 * GImGui->Style.FramePadding.y;
		ImGui::SameLine(ImGui::GetWindowWidth() - lineHeight - windowPadding);

		if (ImGui::BeginPopupContextItem(0)) {
			m_PopupOpened = true;
			if (ImGui::MenuItem("Add Component")) {
				
			}

			if (ImGui::MenuItem("Delete Entity"))
				shouldDeleteEntity = true;

			ImGui::EndPopup();
		}

		ImGui::PushID((int)uuid);
		if (entity == selectedEntity) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f }); }
		if (ImGui::Button("x", { lineHeight, lineHeight })) {
			ImGui::OpenPopup("RemoveEntityOption");
		}
		if (entity == selectedEntity) { ImGui::PopStyleColor(); }
		bool buttonClicked = ImGui::IsItemClicked();

		if (ImGui::BeginPopup("RemoveEntityOption")) {
			if (ImGui::MenuItem("Remove Entity"))
				shouldDeleteEntity = true;

			ImGui::EndPopup();
		}
		ImGui::PopID();

		if (!buttonClicked && !treeToggledOpen && treeNodeClicked) { selectedEntity = entity; }

		ImGui::PopStyleVar();
		if (treeNodeOpened) {
			ImGui::TreePop();
		}

		if (shouldDeleteEntity) {
			if (entity == selectedEntity) 
				selectedEntity = {};

			((Scene*)entity)->DestroyEntity(entity);
		}
	}

}

