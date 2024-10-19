

#include "InspectorPanel.h"
#include "FontManager.h"
#include "Theme.h"

#include <imgui_internal.h>

namespace Quirk {

	template<typename T, typename function>
	static void DrawComponentNode(const std::string& label, Entity& entity, function uiFunction) {
		bool shouldDeleteComponent = false;
		float windowPadding = GImGui->Style.WindowPadding.x;

		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
		treeNodeFlags |= ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.HasComponent<T>()) {
			float lineHeight = GImGui->Font->FontSize + 2 * GImGui->Style.FramePadding.y;

			ImGui::PushStyleColor(ImGuiCol_Text, Theme::GetColor(ColorName::DarkText));
			ImGui::PushFont(FontManager::GetFont("ComponentTreeNode"));
			bool treeNodeOpened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, label.c_str());
			ImGui::PopFont();

			ImGui::SameLine(ImGui::GetWindowWidth() - lineHeight - windowPadding / 2);
			if (ImGui::Button("x", { lineHeight, lineHeight })) {
				ImGui::OpenPopup("ComponentSettings");	
			}
			ImGui::PopStyleColor();

			if (ImGui::BeginPopup("ComponentSettings")) {
				if(ImGui::MenuItem("Remove Component"))
					shouldDeleteComponent = true;

				ImGui::EndPopup();
			}

			if (treeNodeOpened) {
				T& component = entity.GetComponent<T>();
				uiFunction(component);
				ImGui::TreePop();

				ImGui::Dummy(ImVec2(0.0f, lineHeight / 4));
			}
		}

		if(shouldDeleteComponent)
			entity.RemoveComponent<T>();
	}

	void InspectorPanel::OnImguiUiUpdate(Entity& entity) {
		ImGui::Begin("Inspector");
		ImGui::PushStyleColor(ImGuiCol_Border, Theme::GetColor(ColorName::PopupBorder));

		// stopping further processing if no entity is selected in scene hierarcy
		if (!(Scene*)entity) {
			ImGui::PopStyleColor();
			ImGui::End();
			return;
		}

		if (entity.HasComponent<TagComponent>()) {
			std::string& tag = entity.GetComponent<TagComponent>().Tag;

			char entityName[128];
			std::memset(entityName, 0, sizeof(entityName));
			std::memcpy(entityName, (void*)tag.c_str(), tag.size());

			ImGui::AlignTextToFramePadding();
			ImGui::PushFont(FontManager::GetFont("PropertyLabel"));
			ImGui::Text("Tag");
			ImGui::PopFont();

			ImGui::SameLine(0.0f, 15.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, ImGui::GetStyle().FramePadding.y));
			if (ImGui::InputText("##tag", entityName, sizeof(entityName))) {			
				tag = std::string(entityName);
			}
			ImGui::PopStyleVar();

			ImGui::Dummy(ImVec2(0.0f, 0.0f));
		}

		DrawComponentNode<SpriteRendererComponent>("Sprite Renderer", entity, [](SpriteRendererComponent& component) {
			ImGui::Text("Color");
			ImGui::SameLine();
			ImGui::PushFont(FontManager::GetFont("DragFloatValue"));
			ImGui::ColorEdit4("##color", glm::value_ptr(component.Color));
			ImGui::PopFont();
		});

		DrawComponentNode<TransformComponent>("Transforms", entity, [](TransformComponent& component) {
			// width of word "Position" is largest among the three also took extra 3 letters space as "xxx" for padding 
			auto size = ImGui::CalcTextSize("Positionxxx");

			DrawFloat3("Position", glm::value_ptr(component.Translation), 0.0f, size.x);

			glm::vec3 rotation = glm::degrees(component.Rotation);
			if (DrawFloat3("Rotation", glm::value_ptr(rotation), 0.0f, size.x)) {
				component.Rotation = glm::radians(rotation);
			}

			DrawFloat3("Scale", glm::value_ptr(component.Scale), 1.0f, size.x);
		});

		DrawComponentNode<CameraComponent>("Camera", entity, [] (CameraComponent& component) {
			static int currentProjection = 1;
			const char* projectionTypes[] = { "Perspective", "Orthographic" };

			if (ImGui::BeginTable("Propertycheckbox", 3)) {
				ImGui::TableNextColumn();

				ImGui::AlignTextToFramePadding();
				ImGui::Text("Is Primary");

				ImGui::AlignTextToFramePadding();
				ImGui::Text("Fixed Ratio");

				ImGui::TableNextColumn();

				ImGui::Checkbox("##isPrimaryCamera", &component.Primary);
				ImGui::Checkbox("##isFixedRatio", &component.FixedAspectRatio);

				ImGui::EndTable();
			}

			if (ImGui::BeginTable("ProjectionType", 2)) {
				ImGui::TableNextColumn();

				ImGui::AlignTextToFramePadding();
				ImGui::Text("Projection Type");

				ImGui::TableNextColumn();

				ImGui::PushItemWidth(ImGui::CalcTextSize("xxxOrthographicxxx").x);
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, ImGui::GetStyle().FramePadding.y));
				ImGui::PushStyleColor(ImGuiCol_Button, Theme::GetColor(ColorName::DropdownButton));
				if (ImGui::Combo("##projectionTypeSelection", &currentProjection, projectionTypes, IM_ARRAYSIZE(projectionTypes))) {
					auto projectionType = (currentProjection == 1) ? SceneCamera::ProjectionType::Orthographic : SceneCamera::ProjectionType::Perspective;
					component.Camera.SetProjectionType(projectionType);
				}
				ImGui::PopStyleColor();
				ImGui::PopStyleVar();
				ImGui::PopItemWidth();

				ImGui::EndTable();
			}

			if (currentProjection == 0 && ImGui::BeginTable("Perspective Properties", 2)) {
				ImGui::TableNextColumn();

				float nearPlane = component.Camera.GetPerspectiveNearClip();
				ImGui::AlignTextToFramePadding();
				ImGui::PushFont(FontManager::GetFont("PropertyLabel"));
				ImGui::Text("Near Plane");
				ImGui::PopFont();

				float farPlane = component.Camera.GetPerspectiveFarClip();
				ImGui::AlignTextToFramePadding();
				ImGui::PushFont(FontManager::GetFont("PropertyLabel"));
				ImGui::Text("Far Plane");
				ImGui::PopFont();

				float fov = glm::degrees(component.Camera.GetPerspectiveVerticalFOV());
				ImGui::AlignTextToFramePadding();
				ImGui::PushFont(FontManager::GetFont("PropertyLabel"));
				ImGui::Text("Field Of View (FOV)");
				ImGui::PopFont();

				ImGui::TableNextColumn();

				if (ImGui::DragFloat("##nearPlane", &nearPlane, 0.1f, 0.0f, 0.0f, "%.2f")) {
					component.Camera.SetPerspectiveNearClip(nearPlane);
				}

				if (ImGui::DragFloat("##farPlane", &farPlane, 0.1f, 0.0f, 0.0f, "%.2f")) {
					component.Camera.SetPerspectiveFarClip(farPlane);
				}

				if (ImGui::DragFloat("##fieldOfView", &fov, 0.1f, 0.0f, 70.0f, "%.2f")) {
					component.Camera.SetPerspectiveVerticalFOV(glm::radians(fov));
				}

				ImGui::EndTable();
			}
			else if (currentProjection == 1 && ImGui::BeginTable("Orthographic Properties", 2)) {
				ImGui::TableNextColumn();

				float orthographicNear = component.Camera.GetOrthographicNearClip();
				ImGui::AlignTextToFramePadding();
				ImGui::PushFont(FontManager::GetFont("PropertyLabel"));
				ImGui::Text("Near Plane");
				ImGui::PopFont();

				float orthographicFar = component.Camera.GetOrthographicFarClip();
				ImGui::AlignTextToFramePadding();
				ImGui::PushFont(FontManager::GetFont("PropertyLabel"));
				ImGui::Text("Far Plane");
				ImGui::PopFont();

				float orthographicSize = component.Camera.GetOrthographicSize();
				ImGui::AlignTextToFramePadding();
				ImGui::PushFont(FontManager::GetFont("PropertyLabel"));
				ImGui::Text("Orthographic Size");
				ImGui::PopFont();

				ImGui::TableNextColumn();

				if (ImGui::DragFloat("##orthographicNear", &orthographicNear, 0.1f, 0.0f, 0.0f, "%.2f")) {
					component.Camera.SetOrthographicNearClip(orthographicNear);
				}

				if (ImGui::DragFloat("##orthographicFar", &orthographicFar, 0.1f, 0.0f, 0.0f, "%.2f")) {
					component.Camera.SetOrthographicFarClip(orthographicFar);
				}

				if (ImGui::DragFloat("##orthographicSize", &orthographicSize, 0.1f, 0.0f, 70.0f, "%.2f")) {
					component.Camera.SetOrthographicSize(orthographicSize);
				}

				ImGui::EndTable();
			}
		});

		ImGui::PopStyleColor();
		ImGui::End();
	}

	bool InspectorPanel::DrawFloat3(const char* label, float* data, float resetValue, float labelColumnWidth, float speed) {
		float min = 0.0f, max = 0.0f;
		bool valueChanged = false;
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y;

		if (ImGui::BeginTable("transform", 4)) {

			ImGui::TableNextColumn();
			ImGui::PushFont(FontManager::GetFont("PropertyLabel"));
			ImGui::AlignTextToFramePadding();
			ImGui::Text(label);
			ImGui::PopFont();

			ImGui::TableNextColumn();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.769f, 0.125f, 0.129f, 1.0f });
			ImGui::PushFont(FontManager::GetFont(FontWeight::Bold, 18));
			if (ImGui::Button("X", { lineHeight, lineHeight })) {
				valueChanged = true;
				data[0] = resetValue;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor();

			ImGui::SameLine(0.0f, 0.0f);

			ImGui::PushFont(FontManager::GetFont("DragFloatValue"));
			valueChanged |= ImGui::DragFloat("##X", data, speed, min, max, "%.2f");
			ImGui::PopFont();

			ImGui::TableNextColumn();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.596f, 0.808f, 0.0f, 1.0f });
			ImGui::PushFont(FontManager::GetFont(FontWeight::Bold, 18));
			if (ImGui::Button("Y", { lineHeight, lineHeight })) {
				valueChanged = true;
				data[1] = resetValue;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor();

			ImGui::SameLine(0.0f, 0.0f);

			ImGui::PushFont(FontManager::GetFont("DragFloatValue"));
			valueChanged |= ImGui::DragFloat("##Y", data + 1, speed, min, max, "%.2f");
			ImGui::PopFont();

			ImGui::TableNextColumn();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.031f, 0.239f, 0.467f, 1.0f });
			ImGui::PushFont(FontManager::GetFont(FontWeight::Bold, 18));
			if (ImGui::Button("Z", { lineHeight, lineHeight })) {
				valueChanged = true;
				data[2] = resetValue;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor();

			ImGui::SameLine(0.0f, 0.0f);

			ImGui::PushFont(FontManager::GetFont("DragFloatValue"));
			valueChanged |= ImGui::DragFloat("##Z", data + 2, speed, min, max, "%.2f");
			ImGui::PopFont();

			ImGui::EndTable();
		}
		return valueChanged;
	}

}
