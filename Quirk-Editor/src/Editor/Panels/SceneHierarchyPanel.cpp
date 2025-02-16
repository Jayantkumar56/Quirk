

#include "SceneHierarchyPanel.h"
#include "Editor/Theme.h"
#include "Editor/EditorFrame.h"

#include <imgui_internal.h>

namespace Quirk {

	void SceneHierarchyPanel::OnImguiUiUpdate(){
		Ref<Scene>& scene      = ((EditorFrame*)GetParentFrame())->GetMainScene();
		Entity& selectedEntity = ((EditorFrame*)GetParentFrame())->GetSelectedEntity();

		ImGui::PushStyleColor(ImGuiCol_Border, Theme::GetColor(ColorName::PopupBorder));

		ImVec2 framePadding{ 5.0f, 5.0f };
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, framePadding);
		const char* buttonText = "Add Entity";

		float windowWidth = ImGui::GetContentRegionAvail().x;
		float buttonSize = ImGui::CalcTextSize(buttonText).x + framePadding.x;

		ImVec2 buttonOffset = { (windowWidth - buttonSize) * 0.5f, ImGui::GetCursorPosY() + 2.0f };
		ImGui::SetCursorPos(buttonOffset);

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.192f, 0.337f, 0.349f, 1.0f });
		if (ImGui::Button(buttonText)) scene->CreateEntity();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		for (auto entity : scene->m_Registry.view<entt::entity>()) {
			Entity entityToShow = { entity, scene.get()};
			DrawEntityNode(entityToShow, selectedEntity);
		}

		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
			if (ImGui::MenuItem("Add Empty Entity")) {
				scene->CreateEntity();
			}

			ImGui::EndPopup();
		}

		if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) { selectedEntity = {}; }

		ImGui::Dummy({0.0f, 10.0f});
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("Per frame %.3f ms/frame", 1000.0f / io.Framerate);
		ImGui::Text("FPS :- %.1f", io.Framerate);

		ImGui::PopStyleColor();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity, Entity& selectedEntity) {
		bool shouldDeleteEntity = false;
		float windowPadding		= GImGui->Style.WindowPadding.x;
		const std::string& tag	= entity.GetComponent<TagComponent>().Tag;
		uint64_t uuid			= entity.GetComponent<UUIDComponent>().Uuid;
		ImGui::PushID((int)uuid);

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

		bool openAddComponentMenu = false;
		if (ImGui::BeginPopupContextItem(0)) {
			if (ImGui::MenuItem("Add Component")) {
				openAddComponentMenu = true;
			}

			if (ImGui::MenuItem("Delete Entity"))
				shouldDeleteEntity = true;

			ImGui::EndPopup();
		}

		if (openAddComponentMenu) { ImGui::OpenPopup("AddComponentsMenu"); }
		if (ImGui::BeginPopup("AddComponentsMenu")) {
			ComponentTypesIterator::Iterate<ComponentTypes::NonIdentifiers>([&entity] <typename T> (const std::string& componentName) -> void {
				if (!entity.HasComponent<T>() && ImGui::MenuItem(componentName.c_str())) {
					entity.AddComponent<T>();
				}
			});

			ImGui::EndPopup();
		}

		if (entity == selectedEntity) { ImGui::PushStyleColor(ImGuiCol_Text, Theme::GetColor(ColorName::DarkText)); }
		bool buttonClicked = ImGui::Button("x", { lineHeight, lineHeight });
		if (entity == selectedEntity) { ImGui::PopStyleColor(); }

		if (ImGui::BeginPopupContextItem(NULL, ImGuiPopupFlags_MouseButtonLeft)) {
			if (ImGui::MenuItem("Remove Entity"))
				shouldDeleteEntity = true;

			ImGui::EndPopup();
		}

		if (!buttonClicked && !treeToggledOpen && treeNodeClicked) { selectedEntity = entity; }

		ImGui::PopID();
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

