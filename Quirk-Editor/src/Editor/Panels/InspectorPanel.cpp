

#include "InspectorPanel.h"
#include "Editor/Theme.h"
#include "Editor/EditorFrame.h"
#include "Core/Imgui/ImguiUIUtility.h"
#include "Core/Renderer/Geometry/PrimitiveMeshGenerator.h"

namespace Quirk {

	template<typename T, typename function>
	void DrawComponentNode(Frame* parentFrame, const std::string& label, Entity& entity, function uiFunction) {
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

	void InspectorPanel::SetImguiProperties() {

	}

	void InspectorPanel::UnSetImguiProperties() {

	}

	void InspectorPanel::OnImguiUiUpdate() {
		auto parentFrame  = (EditorFrame*)GetParentFrame();
		Entity& entity    = parentFrame->GetSelectedEntity();

		// stopping further processing if no entity is selected in scene hierarcy
		if (entity.IsInvalidEntity()) {
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
			ImguiUIUtility::Text("Tag", labelFont);

			ImGui::SameLine(0.0f, 15.0f);
			if (ImGui::InputText("##tag", entityName, sizeof(entityName))) {			
				tag = std::string(entityName);
			}

			ImGui::PopStyleVar();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
		}

		DrawComponentNode<TransformComponent>(parentFrame, "Transforms", entity, [&](TransformComponent& component) {
			ImFont* buttonFont = FontManager::GetFont(FontWeight::Bold, 18);
			ImFont* valuesFont = FontManager::GetFont("DragFloatValue");

			// width of word "Position" is largest among the three also took extra 3 letters space as "xxx" for padding 
			auto size = ImGui::CalcTextSize("Positionxxx");

			ImguiUIUtility::DrawFloat3("Position", glm::value_ptr(component.Translation), 0.0f, 0.1f, size.x, labelFont, buttonFont, valuesFont);

			glm::vec3 rotation = glm::degrees(component.Rotation);
			if (ImguiUIUtility::DrawFloat3("Rotation", glm::value_ptr(rotation), 0.0f, 0.1f, size.x, labelFont, buttonFont, valuesFont)) {
				component.Rotation = glm::radians(rotation);
			}

			ImguiUIUtility::DrawFloat3("Scale", glm::value_ptr(component.Scale), 1.0f, 0.1f, size.x, labelFont, buttonFont, valuesFont);
		});

		DrawComponentNode<SpriteRendererComponent>(parentFrame, "Sprite Renderer", entity, [&](SpriteRendererComponent& component) {
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
				ImguiUIUtility::Text("Color", labelFont);

				ImGui::TableNextColumn();
				ImGui::PushFont(FontManager::GetFont("DragFloatValue"));
				ImGui::ColorEdit4("##color", glm::value_ptr(component.Color));
				ImGui::PopFont();

				ImGui::TableNextColumn();
				float columnHeight = 28.0f;
				float textHeight = ImGui::CalcTextSize("Tp").y;
				float offset = (columnHeight - textHeight) * 0.5f;
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, offset));

				ImguiUIUtility::Text("Texture", labelFont);

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
					fileDialogSpec.ParentWindow = &((EditorFrame*)GetParentFrame())->GetWindow();

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

		DrawComponentNode<CameraComponent>(parentFrame, "Camera", entity, [labelFont] (CameraComponent& component) {
			int currentProjection = component.Camera.GetProjectionType();
			const char* projectionTypes[] = { "Perspective", "Orthographic" };

			if (ImGui::BeginTable("Propertycheckbox", 3)) {
				ImGui::TableNextRow();

				ImGui::TableNextColumn();
				ImguiUIUtility::Text("Is Primary", labelFont);
				ImGui::TableNextColumn();
				ImGui::Checkbox("##isPrimaryCamera", &component.IsPrimary);

				ImGui::TableNextRow();

				ImGui::TableNextColumn();
				ImguiUIUtility::Text("Fixed Ratio", labelFont);
				ImGui::TableNextColumn();
				ImGui::Checkbox("##isFixedRatio", &component.FixedAspectRatio);

				ImGui::EndTable();
			}

			if (ImGui::BeginTable("ProjectionType", 2)) {
				ImGui::TableSetupColumn("propertiesLable", ImGuiTableColumnFlags_WidthFixed);

				ImGui::TableNextColumn();
				ImguiUIUtility::Text("Projection Type", labelFont);

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
				ImguiUIUtility::Text("Near Plane", labelFont);

				ImGui::TableNextColumn();
				if (ImGui::DragFloat("##nearPlane", &nearPlane, 0.1f, 0.0f, 0.0f, "%.2f"))
					component.Camera.SetPerspectiveNearClip(nearPlane);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImguiUIUtility::Text("Far Plane", labelFont);

				ImGui::TableNextColumn();
				if (ImGui::DragFloat("##farPlane", &farPlane, 0.1f, 0.0f, 0.0f, "%.2f"))
					component.Camera.SetPerspectiveFarClip(farPlane);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImguiUIUtility::Text("Field Of View (FOV)", labelFont);

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
				ImguiUIUtility::Text("Near Plane", labelFont);

				ImGui::TableNextColumn();
				if (ImGui::DragFloat("##orthographicNear", &orthographicNear, 0.1f, 0.0f, 0.0f, "%.2f"))
					component.Camera.SetOrthographicNearClip(orthographicNear);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImguiUIUtility::Text("Far Plane", labelFont);

				ImGui::TableNextColumn();
				if (ImGui::DragFloat("##orthographicFar", &orthographicFar, 0.1f, 0.0f, 0.0f, "%.2f"))
					component.Camera.SetOrthographicFarClip(orthographicFar);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImguiUIUtility::Text("Orthographic Size", labelFont);

				ImGui::TableNextColumn();
				if (ImGui::DragFloat("##orthographicSize", &orthographicSize, 0.1f, 0.0f, 70.0f, "%.2f"))
					component.Camera.SetOrthographicSize(orthographicSize);

				ImGui::EndTable();
			}
		});

		DrawComponentNode<MeshComponent>(parentFrame, "Mesh", entity, [labelFont](MeshComponent& component) {
			component.MeshObject.Type;
			const char* meshTypes[] = { "Select", "Cube" };
			int currentType = (int)component.MeshObject.Type;

			ImGui::Text("Mesh Type");
			ImGui::SameLine();
			if (ImGui::Combo("##meshTypeSelection", &currentType, meshTypes, IM_ARRAYSIZE(meshTypes))) {
				if (currentType == 1) {
					component.MeshObject = PrimitiveMeshGenerator::Generate(MeshType::Cube);
				}
			}

			ImGui::Text("Ambient");
			ImGui::SameLine();
			ImGui::ColorEdit3("##Ambient", glm::value_ptr(component.MaterialProperties.Ambient));

			ImGui::Text("Diffuse");
			ImGui::SameLine();
			ImGui::ColorEdit3("##Diffuse", glm::value_ptr(component.MaterialProperties.Diffuse));

			ImGui::Text("Specular");
			ImGui::SameLine();
			ImGui::ColorEdit3("##Specular", glm::value_ptr(component.MaterialProperties.Specular));

			ImGui::Text("Shininess");
			ImGui::SameLine();
			ImGui::DragFloat("##Shininess", &component.MaterialProperties.Shininess, 0.1f);
		});

		DrawComponentNode<LightComponent>(parentFrame, "Light", entity, [labelFont](LightComponent& component) {
			const char* lightTypes[] = { "Select", "Point" };
			int currentType = (int)component.Type;

			ImGui::Text("Light Type");
			ImGui::SameLine();
			if (ImGui::Combo("##lightTypeSelection", &currentType, lightTypes, IM_ARRAYSIZE(lightTypes))) {
				component.Type = (LightType)currentType;
			}

			ImGui::Text("Color");
			ImGui::SameLine();
			ImGui::ColorEdit3("##lightColor", glm::value_ptr(component.Color));

			ImGui::Text("Ambient");
			ImGui::SameLine();
			ImGui::DragFloat("##lightAmbient", &component.Ambient, 0.1f);

			ImGui::Text("Diffuse");
			ImGui::SameLine();
			ImGui::DragFloat("##lightDiffuse", &component.Diffuse, 0.1f);

			ImGui::Text("Specular");
			ImGui::SameLine();
			ImGui::DragFloat("##lightSpecular", &component.Specular, 0.1f);
		});

		// add component button
		{
			ImVec2 framePadding{ 5.0f, 5.0f };
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, framePadding);
			const char* buttonText = "Add Component";

			float windowWidth  = ImGui::GetContentRegionAvail().x;
			float buttonSize   = ImGui::CalcTextSize(buttonText).x + framePadding.x;
	
			ImVec2 buttonOffset = { (windowWidth - buttonSize) * 0.5f, ImGui::GetCursorPosY() + 25.0f };
			ImGui::SetCursorPos(buttonOffset);

			ImGui::PushStyleColor(ImGuiCol_Button, { 0.192f, 0.337f, 0.349f, 1.0f });
			ImGui::Button(buttonText);
			ImGui::PopStyleColor();

			if (ImGui::BeginPopupContextItem(NULL, ImGuiPopupFlags_MouseButtonLeft)) {
				ComponentTypesIterator::Iterate<ComponentTypes::NonIdentifiers>([&entity] <typename T> (const std::string& componentName) -> void {
					if (!entity.HasComponent<T>() && ImGui::MenuItem(componentName.c_str())) {
						entity.AddComponent<T>();
					}
				});
				ImGui::EndPopup();
			}

			ImGui::PopStyleVar();
		}

		ImGui::PopStyleColor();
	}

}
