

#include "InspectorPanel.h"
#include "FontManager.h"
#include "Theme.h"
#include "Core/Imgui/ImguiUIUtility.h"

namespace Quirk {

	template<typename T, typename function>
	static void DrawComponentNode(const std::string& label, Entity& entity, function uiFunction) {
		if (!entity.HasComponent<T>())
			return;

		bool shouldDeleteComponent = false;
		float windowPadding = GImGui->Style.WindowPadding.x;

		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
		treeNodeFlags |= ImGuiTreeNodeFlags_AllowItemOverlap;

		float lineHeight = GImGui->Font->FontSize + 2 * GImGui->Style.FramePadding.y;
		float treeNodeWidth = ImGui::GetContentRegionAvail().x;

		ImGui::PushStyleColor(ImGuiCol_Text, Theme::GetColor(ColorName::DarkText));
		ImGui::PushFont(FontManager::GetFont("ComponentTreeNode"));
		bool treeNodeOpened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, label.c_str());
		ImGui::PopFont();

		ImGui::SameLine(treeNodeWidth - (lineHeight / 2));
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

		if(shouldDeleteComponent)
			entity.RemoveComponent<T>();
	}

	void InspectorPanel::OnImguiUiUpdate(Entity& entity) {
		ImGui::Begin("Inspector");

		// stopping further processing if no entity is selected in scene hierarcy
		if (!(Scene*)entity) {
			ImGui::End();
			return;
		}

		ImGui::PushStyleColor(ImGuiCol_Border, Theme::GetColor(ColorName::PopupBorder));
		ImFont* labelFont = FontManager::GetFont("PropertyLabel");

		if (entity.HasComponent<TagComponent>()) {
			std::string& tag = entity.GetComponent<TagComponent>().Tag;

			char entityName[128];
			std::memset(entityName, 0, sizeof(entityName));
			std::memcpy(entityName, (void*)tag.c_str(), tag.size());

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, ImGui::GetStyle().FramePadding.y));
			ImguiUI::Utility::Text("Tag", labelFont);

			ImGui::SameLine(0.0f, 15.0f);
			if (ImGui::InputText("##tag", entityName, sizeof(entityName))) {			
				tag = std::string(entityName);
			}

			ImGui::PopStyleVar();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
		}

		DrawComponentNode<TransformComponent>("Transforms", entity, [labelFont](TransformComponent& component) {
			ImFont* buttonFont = FontManager::GetFont(FontWeight::Bold, 18);
			ImFont* valuesFont = FontManager::GetFont("DragFloatValue");

			// width of word "Position" is largest among the three also took extra 3 letters space as "xxx" for padding 
			auto size = ImGui::CalcTextSize("Positionxxx");

			ImguiUI::Utility::DrawFloat3("Position", glm::value_ptr(component.Translation), 0.0f, 0.1f, size.x, labelFont, buttonFont, valuesFont);

			glm::vec3 rotation = glm::degrees(component.Rotation);
			if (ImguiUI::Utility::DrawFloat3("Rotation", glm::value_ptr(rotation), 0.0f, 0.1f, size.x, labelFont, buttonFont, valuesFont)) {
				component.Rotation = glm::radians(rotation);
			}

			ImguiUI::Utility::DrawFloat3("Scale", glm::value_ptr(component.Scale), 1.0f, 0.1f, size.x, labelFont, buttonFont, valuesFont);
		});

		DrawComponentNode<SpriteRendererComponent>("Sprite Renderer", entity, [&](SpriteRendererComponent& component) {
			std::string texturePathStr = "No Texture";
			if (component.Texture != nullptr) {
				texturePathStr = component.Texture->GetPath().filename().string();
			}

			ImVec2 cellPadding = ImGui::GetStyle().CellPadding;
			ImGui::GetStyle().CellPadding = ImVec2(10.0f, 6.0f);

			if (ImGui::BeginTable("spriteproperties", 2)) {
				ImGui::TableSetupColumn("propertiesLable", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("propertiesValue", ImGuiTableColumnFlags_NoResize);

				ImGui::TableNextColumn();
				ImguiUI::Utility::Text("Color", labelFont);

				ImGui::TableNextColumn();
				ImGui::PushFont(FontManager::GetFont("DragFloatValue"));
				ImGui::ColorEdit4("##color", glm::value_ptr(component.Color));
				ImGui::PopFont();

				ImGui::TableNextColumn();
				float columnHeight = 28.0f;
				float textHeight = ImGui::CalcTextSize("Tp").y;
				float offset = (columnHeight - textHeight) * 0.5f;
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, offset));

				ImguiUI::Utility::Text("Texture", labelFont);

				ImGui::TableNextColumn();
				ImGui::InputText("##texture0", (char*)texturePathStr.c_str(), texturePathStr.size(), ImGuiInputTextFlags_ReadOnly);
				ImGui::PopStyleVar();

				if (ImGui::BeginDragDropTarget()) {
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("IMAGE_PATH");
					if (payload) {
						component.Texture = Texture2D::Create(**(std::filesystem::path**)payload->Data);
					}

					ImGui::EndDragDropTarget();
				}

				ImGui::SameLine(0.0f, 0.0f);
				ImTextureID uploadImageIconId = (ImTextureID)(intptr_t)m_UploadImage->GetRendererId();
				if (ImGui::ImageButton("uploadImageButton", uploadImageIconId, { columnHeight -6.0f, columnHeight -6.0f }, { 0, 1 }, { 1, 0 })) {
					FileFilter filters[] = {
						{L"image",		L"*.png;*.JPG;*.JPEG*.jpg;*.jpeg"}
					};

					FileDialogSpecification fileDialogSpec;
					fileDialogSpec.Title = L"Select Texture";
					fileDialogSpec.FileNameLabel = L"Texture Name";
					fileDialogSpec.Filters = filters;
					fileDialogSpec.NoOfFilters = sizeof(filters) / sizeof(FileFilter);

					std::filesystem::path filePath;
					if (FileDialog::OpenFile(fileDialogSpec, filePath)) {
						component.Texture = Texture2D::Create(filePath);
					}
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Select Image");
				}

				ImGui::SameLine(0.0f, 0.0f);
				ImTextureID removeImageIconId = (ImTextureID)(intptr_t)m_RemoveImage->GetRendererId();
				ImGui::ImageButton("removeImageButton", removeImageIconId, { columnHeight - 6.0f, columnHeight - 6.0f }, { 0, 1 }, { 1, 0 });
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Remove Image");
				}

				if (ImGui::BeginPopupContextItem(NULL, ImGuiPopupFlags_MouseButtonLeft)) {
					ImGui::Text("Remove Image?");

					if (ImGui::Button("Yes")) { component.Texture = nullptr; ImGui::CloseCurrentPopup(); }
					if (ImGui::Button("No")) ImGui::CloseCurrentPopup();

					ImGui::EndPopup();
				}

				ImGui::EndTable();
			}

			ImGui::GetStyle().CellPadding = cellPadding;
		});

		DrawComponentNode<CameraComponent>("Camera", entity, [labelFont] (CameraComponent& component) {
			int currentProjection = component.Camera.GetProjectionType();
			const char* projectionTypes[] = { "Perspective", "Orthographic" };

			if (ImGui::BeginTable("Propertycheckbox", 3)) {
				ImGui::TableNextRow();

				ImGui::TableNextColumn();
				ImguiUI::Utility::Text("Is Primary", labelFont);
				ImGui::TableNextColumn();
				ImGui::Checkbox("##isPrimaryCamera", &component.IsPrimary);

				ImGui::TableNextRow();

				ImGui::TableNextColumn();
				ImguiUI::Utility::Text("Fixed Ratio", labelFont);
				ImGui::TableNextColumn();
				ImGui::Checkbox("##isFixedRatio", &component.FixedAspectRatio);

				ImGui::EndTable();
			}

			if (ImGui::BeginTable("ProjectionType", 2)) {
				ImGui::TableSetupColumn("propertiesLable", ImGuiTableColumnFlags_WidthFixed);

				ImGui::TableNextColumn();
				ImguiUI::Utility::Text("Projection Type", labelFont);

				ImGui::TableNextColumn();

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, ImGui::GetStyle().FramePadding.y));
				ImGui::PushStyleColor(ImGuiCol_Button, Theme::GetColor(ColorName::DropdownButton));
				if (ImGui::Combo("##projectionTypeSelection", &currentProjection, projectionTypes, IM_ARRAYSIZE(projectionTypes))) {
					auto projectionType = (currentProjection == 1) ? SceneCamera::ProjectionType::Orthographic : SceneCamera::ProjectionType::Perspective;
					component.Camera.SetProjectionType(projectionType);
				}
				ImGui::PopStyleColor();
				ImGui::PopStyleVar();

				ImGui::EndTable();
			}

			if (currentProjection == 0 && ImGui::BeginTable("Perspective Properties", 2)) {
				float nearPlane = component.Camera.GetPerspectiveNearClip();
				float farPlane  = component.Camera.GetPerspectiveFarClip();
				float fov		= glm::degrees(component.Camera.GetPerspectiveVerticalFOV());

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImguiUI::Utility::Text("Near Plane", labelFont);

				ImGui::TableNextColumn();
				if (ImGui::DragFloat("##nearPlane", &nearPlane, 0.1f, 0.0f, 0.0f, "%.2f"))
					component.Camera.SetPerspectiveNearClip(nearPlane);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImguiUI::Utility::Text("Far Plane", labelFont);

				ImGui::TableNextColumn();
				if (ImGui::DragFloat("##farPlane", &farPlane, 0.1f, 0.0f, 0.0f, "%.2f"))
					component.Camera.SetPerspectiveFarClip(farPlane);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImguiUI::Utility::Text("Field Of View (FOV)", labelFont);

				ImGui::TableNextColumn();
				if (ImGui::DragFloat("##fieldOfView", &fov, 0.1f, 0.0f, 70.0f, "%.2f"))
					component.Camera.SetPerspectiveVerticalFOV(glm::radians(fov));

				ImGui::EndTable();
			}
			else if (currentProjection == 1 && ImGui::BeginTable("Orthographic Properties", 2)) {
				float orthographicNear = component.Camera.GetOrthographicNearClip();
				float orthographicFar  = component.Camera.GetOrthographicFarClip();
				float orthographicSize = component.Camera.GetOrthographicSize();

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImguiUI::Utility::Text("Near Plane", labelFont);

				ImGui::TableNextColumn();
				if (ImGui::DragFloat("##orthographicNear", &orthographicNear, 0.1f, 0.0f, 0.0f, "%.2f"))
					component.Camera.SetOrthographicNearClip(orthographicNear);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImguiUI::Utility::Text("Far Plane", labelFont);

				ImGui::TableNextColumn();
				if (ImGui::DragFloat("##orthographicFar", &orthographicFar, 0.1f, 0.0f, 0.0f, "%.2f"))
					component.Camera.SetOrthographicFarClip(orthographicFar);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImguiUI::Utility::Text("Orthographic Size", labelFont);

				ImGui::TableNextColumn();
				if (ImGui::DragFloat("##orthographicSize", &orthographicSize, 0.1f, 0.0f, 70.0f, "%.2f"))
					component.Camera.SetOrthographicSize(orthographicSize);

				ImGui::EndTable();
			}
		});

		DrawComponentNode<RigidBody2DComponent>("RigidBody2D", entity, [labelFont](RigidBody2DComponent& component) {
			int currentProjection = (int)component.Type;
			const char* projectionTypes[] = { "Static", "Dynamic", "Kinematic" };

			if (ImGui::BeginTable("BodyType", 2)) {
				ImGui::TableSetupColumn("propertiesLable", ImGuiTableColumnFlags_WidthFixed);

				ImGui::TableNextColumn();
				ImguiUI::Utility::Text("BodyType", labelFont);

				ImGui::TableNextColumn();

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, ImGui::GetStyle().FramePadding.y));
				ImGui::PushStyleColor(ImGuiCol_Button, Theme::GetColor(ColorName::DropdownButton));
				if (ImGui::Combo("##BodyTypeSelection", &currentProjection, projectionTypes, IM_ARRAYSIZE(projectionTypes))) {
					switch (currentProjection) {
						case 0: component.Type = RigidBody2DComponent::BodyType::Static;		break;
						case 1 : component.Type = RigidBody2DComponent::BodyType::Dynamic;		break;
						case 2 : component.Type = RigidBody2DComponent::BodyType::Kinematic;	break;
					}
				}
				ImGui::PopStyleColor();
				ImGui::PopStyleVar();

				ImGui::EndTable();
			}

			if (ImGui::BeginTable("Propertycheckbox", 3)) {
				ImGui::TableNextRow();

				ImGui::TableNextColumn();
				ImguiUI::Utility::Text("Fixed Rotation", labelFont);
				ImGui::TableNextColumn();
				ImGui::Checkbox("##isPrimaryCamera", &component.FixedRotation);

				ImGui::EndTable();
			}
		});

		DrawComponentNode<BoxCollider2DComponent>("BoxCollider2D", entity, [labelFont](BoxCollider2DComponent& component) {
			/*glm::vec2 Offset = { 0.0f, 0.0f };
			glm::vec2 Size = { 0.5f, 0.5f };*/

			if (ImGui::BeginTable("boxcColliderProperties", 2)) {
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImguiUI::Utility::Text("Density", labelFont);

				ImGui::TableNextColumn();
				ImGui::DragFloat("##Density", &component.Density, 0.1f, 0.0f, 0.0f, "%.2f");		

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImguiUI::Utility::Text("Friction", labelFont);

				ImGui::TableNextColumn();
				ImGui::DragFloat("##Friction", &component.Friction, 0.1f, 0.0f, 0.0f, "%.2f");

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImguiUI::Utility::Text("Restitution", labelFont);

				ImGui::TableNextColumn();
				ImGui::DragFloat("##Restitution", &component.Restitution, 0.1f, 0.0f, 0.0f, "%.2f");

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImguiUI::Utility::Text("RestitutionThreshold", labelFont);

				ImGui::TableNextColumn();
				ImGui::DragFloat("##RestitutionThreshold", &component.RestitutionThreshold, 0.1f, 0.0f, 0.0f, "%.2f");

				ImGui::EndTable();
			}
		});

		ImGui::PopStyleColor();
		ImGui::End();
	}

}
