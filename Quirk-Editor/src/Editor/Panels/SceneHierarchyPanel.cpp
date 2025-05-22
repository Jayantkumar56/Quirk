

#include "QkEditorpch.h"

#include "SceneHierarchyPanel.h"
#include "Editor/EditorTheme.h"
#include "Editor/EditorFrame.h"

#include "Core/Reflection/Registrations/ComponentList.h"

#include <imgui_internal.h>

namespace QuirkEditor {

	void SceneHierarchyPanel::SetUiProperties() {
		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;
		ImGui::SetNextWindowClass(&window_class);
	}

	void SceneHierarchyPanel::OnUiUpdate(){
        EditorFrame* frame = GetParentFrameAs<EditorFrame>();

        Quirk::Ref<Quirk::Scene>& scene = frame->GetActiveSceneRefView();

		ImGui::PushStyleColor(ImGuiCol_Border, frame->GetTheme().GetColor(ColorName::PopupBorder));

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

		for (auto entity : scene->GetRegistry().view<entt::entity>()) {
			DrawEntityNode({ entity, scene.get() });
		}

		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
			if (ImGui::MenuItem("Add Empty Entity")) {
				scene->CreateEntity();
			}

			ImGui::EndPopup();
		}

		if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) { 
            SelectionContext::SetSelected(Quirk::Entity());
        }

		ImGui::Dummy({0.0f, 10.0f});
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("Per frame %.3f ms/frame", 1000.0f / io.Framerate);
		ImGui::Text("FPS :- %.1f", io.Framerate);

		ImGui::PopStyleColor();
	}

	void SceneHierarchyPanel::DrawEntityNode(Quirk::Entity entity) {
        EditorFrame* frame      = GetParentFrameAs<EditorFrame>();
		bool shouldDeleteEntity = false;
		float windowPadding		= GImGui->Style.WindowPadding.x;
		const std::string& tag	= entity.GetComponent<Quirk::TagComponent>().Tag;
		uint64_t uuid			= entity.GetComponent<Quirk::UUIDComponent>().Uuid;
		ImGui::PushID((int)uuid);

        Quirk::Entity selectedEntity = m_SelectionHandle.Get<Quirk::Entity>();

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
            Quirk::ComponentsIterator<Quirk::ComponentTypesNonIdentifiers>(
                [] <typename T> (std::string_view componentName, Quirk::Entity entity) -> void {
                    if (!entity.HasComponent<T>() && ImGui::MenuItem(componentName.data())) {
                        entity.AddComponent<T>();
                    }
                },
                entity
            );

			ImGui::EndPopup();
		}

		if (entity == selectedEntity) { ImGui::PushStyleColor(ImGuiCol_Text, frame->GetTheme().GetColor(ColorName::DarkText)); }
		bool buttonClicked = ImGui::Button("x", { lineHeight, lineHeight });
		if (entity == selectedEntity) { ImGui::PopStyleColor(); }

		if (ImGui::BeginPopupContextItem(NULL, ImGuiPopupFlags_MouseButtonLeft)) {
			if (ImGui::MenuItem("Remove Entity"))
				shouldDeleteEntity = true;

			ImGui::EndPopup();
		}

		if (!buttonClicked && !treeToggledOpen && treeNodeClicked) {
            SelectionContext::SetSelected(entity);
            selectedEntity = entity;
        }

		ImGui::PopID();
		ImGui::PopStyleVar();
		if (treeNodeOpened) {
			ImGui::TreePop();
		}

		if (shouldDeleteEntity) {
			if (entity == selectedEntity) 
                SelectionContext::SetSelected(Quirk::Entity());

			((Quirk::Scene*)entity)->DestroyEntity(entity);
		}
	}

}

