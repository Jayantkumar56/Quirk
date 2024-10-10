

#include "InspectorPanel.h"
#include <imgui_internal.h>


namespace Quirk {

	bool InspectorPanel::DrawFloat3(const char* label, float* data, float resetValue, float labelColumnWidth, float speed) {
		float min = 0.0f, max = 0.0f;
		bool valueChanged = false;
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2;

		ImGui::BeginGroup();
		ImGui::PushID(label);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, labelColumnWidth);

		ImGui::Text(label);

		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.769f, 0.125f, 0.129f, 1.0f });
		if (ImGui::Button("X", { lineHeight, lineHeight })) {
			valueChanged = true;
			data[0] = resetValue;
		}
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::PushID(0);
		valueChanged |= ImGui::DragFloat("##X", data, speed, min, max, "%.2f");
		ImGui::PopItemWidth();
		ImGui::PopID();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.596f, 0.808f, 0.0f, 1.0f });
		if (ImGui::Button("Y", { lineHeight, lineHeight })) {
			valueChanged = true;
			data[1] = resetValue;
		}
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::PushID(0);
		valueChanged |= ImGui::DragFloat("##Y", data + 1, speed, min, max, "%.2f");
		ImGui::PopItemWidth();
		ImGui::PopID();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.031f, 0.239f, 0.467f, 1.0f });
		if (ImGui::Button("Z", { lineHeight, lineHeight })) {
			valueChanged = true;
			data[2] = resetValue;
		}
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::PushID(0);
		valueChanged |= ImGui::DragFloat("##Z", data + 2, speed, min, max, "%.2f");
		ImGui::PopItemWidth();
		ImGui::PopID();

		ImGui::PopID();
		ImGui::EndGroup();
		ImGui::Columns(1);

		return valueChanged;
	}

	void InspectorPanel::OnImguiUiUpdate(Entity& entity) {
		ImGui::Begin("Inspector");

		if (entity == Entity()) {
			ImGui::End();
			return;
		}

		if (entity.HasComponent<TagComponent>()) {
			std::string& tag = entity.GetComponent<TagComponent>().Tag;

			char entityName[128];
			std::memset(entityName, 0, sizeof(entityName));
			std::memcpy(entityName, (void*)tag.c_str(), tag.size());

			ImGui::Text("Tag");
			ImGui::SameLine();
			if (ImGui::InputText("##tag", entityName, sizeof(entityName))) {
				tag = std::string(entityName);
			}
		}

		if (entity.HasComponent<SpriteRendererComponent>()) {
			if (ImGui::TreeNodeEx("Sprite Renderer")) {
				auto& component = entity.GetComponent<SpriteRendererComponent>();

				ImGui::Text("Color");
				ImGui::SameLine();
				ImGui::ColorEdit4("##color", glm::value_ptr(component.Color));
				
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<TransformComponent>()) {
			if (ImGui::TreeNodeEx("Transforms")) {
				auto& transforms = entity.GetComponent<TransformComponent>();

				// width of word "Position" is largest among the three also took extra 3 letters space as "xxx" for padding 
				auto size = ImGui::CalcTextSize("Positionxxx");

				DrawFloat3("Position", glm::value_ptr(transforms.Translation), 0.0f, size.x);

				glm::vec3 rotation = glm::degrees(transforms.Rotation);
				if (DrawFloat3("Rotation", glm::value_ptr(rotation), 0.0f, size.x)) {
					transforms.Rotation = glm::radians(rotation);
				}

				DrawFloat3("Scale", glm::value_ptr(transforms.Scale), 1.0f, size.x);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>()) {
			if (ImGui::TreeNodeEx("Camera")) {
				auto& camera = entity.GetComponent<CameraComponent>();

				ImGui::Text("Is Primary");
				ImGui::SameLine();
				ImGui::Checkbox("##isPrimaryCamera", &camera.Primary);

				ImGui::Text("Fixed Ratio");
				ImGui::SameLine();
				ImGui::Checkbox("##isFixedRatio", &camera.FixedAspectRatio);

				ImGui::Text("Projection Type");
				ImGui::SameLine();
				const char* projectionTypes[] = { "Perspective", "Orthographic" };
				static int currentProjection = 1;
				ImGui::PushItemWidth(ImGui::CalcTextSize("xxxOrthographicxxx").x);

				if (ImGui::Combo("##projectionTypeSelection", &currentProjection, projectionTypes, IM_ARRAYSIZE(projectionTypes))) {
					auto projectionType = (currentProjection == 1) ? SceneCamera::ProjectionType::Orthographic : SceneCamera::ProjectionType::Perspective;
					camera.Camera.SetProjectionType(projectionType);
				}

				ImGui::PopItemWidth();

				if (currentProjection == 0 /* 0 -> orthographic, 1 -> perspective */) {
					ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

					float fov = glm::degrees(camera.Camera.GetPerspectiveVerticalFOV());
					ImGui::Text("Field Of View (FOV)");
					ImGui::SameLine();
					if (ImGui::DragFloat("##fieldOfView", &fov, 0.1f, 0.0f, 70.0f, "%.2f")) {
						camera.Camera.SetPerspectiveVerticalFOV(glm::radians(fov));
					}
					ImGui::PopItemWidth();

					float nearPlane = camera.Camera.GetPerspectiveNearClip();
					ImGui::Text("Near Plane");
					ImGui::SameLine();
					if (ImGui::DragFloat("##nearPlane", &nearPlane, 0.1f, 0.0f, 0.0f, "%.2f")) {
						camera.Camera.SetPerspectiveNearClip(nearPlane);
					}
					ImGui::PopItemWidth();

					float farPlane = camera.Camera.GetPerspectiveFarClip();
					ImGui::Text("Far Plane");
					ImGui::SameLine();
					if (ImGui::DragFloat("##farPlane", &farPlane, 0.1f, 0.0f, 0.0f, "%.2f")) {
						camera.Camera.SetPerspectiveFarClip(farPlane);
					}
					ImGui::PopItemWidth();
				}
				else {
					ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

					float orthographicSize = camera.Camera.GetOrthographicSize();
					ImGui::Text("Orthographic Size");
					ImGui::SameLine();
					if (ImGui::DragFloat("##orthographicSize", &orthographicSize, 0.1f, 0.0f, 70.0f, "%.2f")) {
						camera.Camera.SetOrthographicSize(orthographicSize);
					}
					ImGui::PopItemWidth();

					float orthographicNear = camera.Camera.GetOrthographicNearClip();
					ImGui::Text("Near Plane");
					ImGui::SameLine();
					if (ImGui::DragFloat("##orthographicNear", &orthographicNear, 0.1f, 0.0f, 0.0f, "%.2f")) {
						camera.Camera.SetOrthographicNearClip(orthographicNear);
					}
					ImGui::PopItemWidth();

					float orthographicFar = camera.Camera.GetOrthographicFarClip();
					ImGui::Text("Far Plane");
					ImGui::SameLine();
					if (ImGui::DragFloat("##orthographicFar", &orthographicFar, 0.1f, 0.0f, 0.0f, "%.2f")) {
						camera.Camera.SetOrthographicFarClip(orthographicFar);
					}
					ImGui::PopItemWidth();
				}

				ImGui::TreePop();
			}
		}

		ImGui::End();
	}

}
