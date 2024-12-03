

#include "InspectorPanel.h"
#include "FontManager.h"
#include "Theme.h"
#include "Core/Imgui/ImguiUIUtility.h"

namespace Quirk {

	template<typename T, typename function>
	static void DrawComponentNode(const std::string& label, Entity& entity, function uiFunction) {
		bool shouldDeleteComponent = false;
		float windowPadding = GImGui->Style.WindowPadding.x;

		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
		treeNodeFlags |= ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.HasComponent<T>()) {
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

		DrawComponentNode<TransformComponent>("Transforms", entity, [](TransformComponent& component) {
			ImFont* lableFont  = FontManager::GetFont("PropertyLabel");
			ImFont* buttonFont = FontManager::GetFont(FontWeight::Bold, 18);
			ImFont* valuesFont = FontManager::GetFont("DragFloatValue");

			// width of word "Position" is largest among the three also took extra 3 letters space as "xxx" for padding 
			auto size = ImGui::CalcTextSize("Positionxxx");

			ImguiUI::Utility::DrawFloat3("Position", glm::value_ptr(component.Translation), 0.0f, 0.1f, size.x, lableFont, buttonFont, valuesFont);

			glm::vec3 rotation = glm::degrees(component.Rotation);
			if (ImguiUI::Utility::DrawFloat3("Rotation", glm::value_ptr(rotation), 0.0f, 0.1f, size.x, lableFont, buttonFont, valuesFont)) {
				component.Rotation = glm::radians(rotation);
			}

			ImguiUI::Utility::DrawFloat3("Scale", glm::value_ptr(component.Scale), 1.0f, 0.1f, size.x, lableFont, buttonFont, valuesFont);
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
				ImGui::PushFont(FontManager::GetFont("PropertyLabel"));
				ImGui::Text("Color");
				ImGui::PopFont();

				ImGui::TableNextColumn();
				ImGui::PushFont(FontManager::GetFont("DragFloatValue"));
				ImGui::ColorEdit4("##color", glm::value_ptr(component.Color));
				ImGui::PopFont();

				ImGui::TableNextColumn();
				float columnHeight = 28.0f;
				float textHeight = ImGui::CalcTextSize("Tp").y;
				float offset = (columnHeight - textHeight) * 0.5f;
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offset);
				ImGui::PushFont(FontManager::GetFont("PropertyLabel"));
				ImGui::Text("Texture");
				ImGui::PopFont();

				ImGui::TableNextColumn();
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, offset));
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

				ImGui::Checkbox("##isPrimaryCamera", &component.IsPrimary);
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

}
