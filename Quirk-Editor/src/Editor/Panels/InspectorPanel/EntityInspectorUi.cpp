

#include "QkEditorpch.h"

#include "EntityInspectorUI.h"
#include "Base/Reflection/Registrations/ComponentList.h"
#include "Editor/EditorTheme.h"
#include "Editor/EditorFrameResourceManager.h"

#include "Core/Frame/FontManager.h"
#include "Core/Assets/Geometry/PrimitiveMeshGenerator.h"

#include "imgui.h"
#include "imgui_internal.h"


namespace QuirkEditor {

    void InspectorDraw<Quirk::Entity>::Draw(EditorFrame* frame, Quirk::Entity entity) {
        if (entity.IsInvalidEntity())
            return;

        ComponentDraw(frame, entity.GetComponent<Quirk::UUIDComponent>());
        ComponentDraw(frame, entity.GetComponent<Quirk::TagComponent>());

        ComponentsIterator<ComponentTypesNonIdentifiers>(
            [] <typename Component> (std::string_view label, EditorFrame * frame, Quirk::Entity entity) {
                if (!entity.HasComponent<Component>()) {
                    return;
                }

		        ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed           | ImGuiTreeNodeFlags_SpanAvailWidth;
		        treeNodeFlags                   |= ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen;

		        ImGui::PushStyleColor(ImGuiCol_Text, frame->GetTheme().GetColor(ColorName::DarkText));
		        ImGui::PushFont(Quirk::FontManager::GetFont("ComponentTreeNode"));

                if ( ImGui::TreeNodeEx((void*)typeid(Component).hash_code(), treeNodeFlags, label.data()) ) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    ComponentDraw(frame, entity.GetComponent<Component>());
                    ImGui::PopStyleColor();

                    ImGui::TreePop();
                }

                ImGui::PopFont();
		        ImGui::PopStyleColor();
            },
            frame, entity
        );

        // Add Component Button
        {
        	ImVec2 framePadding{ 5.0f, 5.0f };
        	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, framePadding);
        	const char* buttonText = "Add Component";
        
        	float windowWidth = ImGui::GetContentRegionAvail().x;
        	float buttonSize  = ImGui::CalcTextSize(buttonText).x + framePadding.x;
        	
        	ImVec2 buttonOffset{ (windowWidth - buttonSize) * 0.5f, ImGui::GetCursorPosY() + 25.0f };
        	ImGui::SetCursorPos(buttonOffset);
        
        	ImGui::PushStyleColor(ImGuiCol_Button, { 0.192f, 0.337f, 0.349f, 1.0f });
        	ImGui::Button(buttonText);
        	ImGui::PopStyleColor();
        
            if (ImGui::BeginPopupContextItem((const char*)0, ImGuiPopupFlags_MouseButtonLeft)) {
                ComponentsIterator<ComponentTypesNonIdentifiers>(
                    [] <typename Component> (std::string_view componentName, Quirk::Entity entity) {
                        if (!entity.HasComponent<Component>() && ImGui::MenuItem(componentName.data()))
                            entity.AddComponent<Component>();
                    },
                    entity
                );
        
                ImGui::EndPopup();
            }
        
        	ImGui::PopStyleVar();
        }
    }

    template<>
    static void InspectorDraw<Quirk::Entity>::ComponentDraw(EditorFrame* frame, Quirk::UUIDComponent& component) {

    }

    template<>
    static void InspectorDraw<Quirk::Entity>::ComponentDraw(EditorFrame* frame, Quirk::TagComponent& component) {
        std::string& tag = component.Tag;
        ImFont* labelFont = Quirk::FontManager::GetFont("PropertyLabel");

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, ImGui::GetStyle().FramePadding.y));

        Quirk::ImguiUIUtility::Text("Tag", labelFont);
        
        ImGui::SameLine(0.0f, 15.0f);
        ImGui::InputText("##tag", &tag);
        
        ImGui::PopStyleVar();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
    }

    template<>
    static void InspectorDraw<Quirk::Entity>::ComponentDraw(EditorFrame* frame, Quirk::TransformComponent& component) {
        ImFont* buttonFont = Quirk::FontManager::GetFont(Quirk::FontWeight::Bold, 18);
        ImFont* valuesFont = Quirk::FontManager::GetFont("DragFloatValue");
        ImFont* labelFont  = Quirk::FontManager::GetFont("PropertyLabel");

        // width of word "Position" is largest among the three also took extra 3 letters space as "xxx" for padding 
        auto size = ImGui::CalcTextSize("Positionxxx");
        
        Quirk::ImguiUIUtility::DrawFloat3("Position", glm::value_ptr(component.Translation), 0.0f, 0.1f, size.x, labelFont, buttonFont, valuesFont);
        
        glm::vec3 rotation = glm::degrees(component.Rotation);
        if (Quirk::ImguiUIUtility::DrawFloat3("Rotation", glm::value_ptr(rotation), 0.0f, 0.1f, size.x, labelFont, buttonFont, valuesFont)) {
        	component.Rotation = glm::radians(rotation);
        }
        
        Quirk::ImguiUIUtility::DrawFloat3("Scale", glm::value_ptr(component.Scale), 1.0f, 0.1f, size.x, labelFont, buttonFont, valuesFont);
    }

    template<>
    static void InspectorDraw<Quirk::Entity>::ComponentDraw(EditorFrame* frame, Quirk::SpriteRendererComponent& component) {
        std::string texturePathStr = "No Texture";
        ImFont* labelFont = Quirk::FontManager::GetFont("PropertyLabel");
        if (component.Texture != nullptr) {
        	//texturePathStr = component.Texture->GetPath().filename().string();
        }
        
        ImVec2 cellPadding = ImGui::GetStyle().CellPadding;
        ImGui::GetStyle().CellPadding = ImVec2(10.0f, 6.0f);
        
        if (ImGui::BeginTable("spriteproperties", 2)) {
        	ImGui::TableSetupColumn("propertiesLable", ImGuiTableColumnFlags_WidthFixed);
        	ImGui::TableSetupColumn("propertiesValue", ImGuiTableColumnFlags_NoResize);
        
        	ImGui::TableNextColumn();
            Quirk::ImguiUIUtility::Text("Color", labelFont);
        
        	ImGui::TableNextColumn();
        	ImGui::PushFont(Quirk::FontManager::GetFont("DragFloatValue"));
        	ImGui::ColorEdit4("##color", glm::value_ptr(component.Color));
        	ImGui::PopFont();
        
        	ImGui::TableNextColumn();
        	float columnHeight = 28.0f;
        	float textHeight = ImGui::CalcTextSize("Tp").y;
        	float offset = (columnHeight - textHeight) * 0.5f;
        	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, offset));
        
            Quirk::ImguiUIUtility::Text("Texture", labelFont);
        
        	ImGui::TableNextColumn();
        	ImGui::InputText("##texture0", (char*)texturePathStr.c_str(), texturePathStr.size(), ImGuiInputTextFlags_ReadOnly);
        	ImGui::PopStyleVar();
        
        	if (ImGui::BeginDragDropTarget()) {
        		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("IMAGE_PATH");
        		if (payload) {
        			component.Texture = Quirk::Texture2D::Create(**(std::filesystem::path**)payload->Data);
        		}
        
        		ImGui::EndDragDropTarget();
        	}
        
        	ImGui::SameLine(0.0f, 0.0f);
        	ImTextureID uploadImageIconId = frame->GetResourceManager().GetIconUpload();
        	if (ImGui::ImageButton("uploadImageButton", uploadImageIconId, { columnHeight -6.0f, columnHeight -6.0f }, { 0, 1 }, { 1, 0 })) {
                Quirk::FileFilter filters[] = {
        			{L"image",		L"*.png;*.JPG;*.JPEG*.jpg;*.jpeg"}
        		};
        
                Quirk::FileDialogSpecification fileDialogSpec;
        		fileDialogSpec.Title = L"Select Texture";
        		fileDialogSpec.FileNameLabel = L"Texture Name";
        		fileDialogSpec.Filters = filters;
        		fileDialogSpec.NoOfFilters = sizeof(filters) / sizeof(Quirk::FileFilter);
        		fileDialogSpec.ParentWindow = &(frame->GetWindow());
        
        		std::filesystem::path filePath;
        		if (Quirk::FileDialog::OpenFile(fileDialogSpec, filePath)) {
        			component.Texture = Quirk::Texture2D::Create(filePath);
        		}
        	}
        	if (ImGui::IsItemHovered()) {
        		ImGui::SetTooltip("Select Image");
        	}
        
        	ImGui::SameLine(0.0f, 0.0f);
        	ImTextureID removeImageIconId = frame->GetResourceManager().GetIconRemove();
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
    }

    template<>
    static void InspectorDraw<Quirk::Entity>::ComponentDraw(EditorFrame* frame, Quirk::CameraComponent& component) {
        int currentProjection = component.Camera.GetProjectionType();
        const char* projectionTypes[] = { "Perspective", "Orthographic" };
        ImFont* labelFont = Quirk::FontManager::GetFont("PropertyLabel");
        
        if (ImGui::BeginTable("Propertycheckbox", 3)) {
        	ImGui::TableNextRow();
        
        	ImGui::TableNextColumn();
            Quirk::ImguiUIUtility::Text("Is Primary", labelFont);
        	ImGui::TableNextColumn();
        	ImGui::Checkbox("##isPrimaryCamera", &component.IsPrimary);
        
        	ImGui::TableNextRow();
        
        	ImGui::TableNextColumn();
            Quirk::ImguiUIUtility::Text("Fixed Ratio", labelFont);
        	ImGui::TableNextColumn();
        	ImGui::Checkbox("##isFixedRatio", &component.FixedAspectRatio);
        
        	ImGui::EndTable();
        }
        
        if (ImGui::BeginTable("ProjectionType", 2)) {
        	ImGui::TableSetupColumn("propertiesLable", ImGuiTableColumnFlags_WidthFixed);
        
        	ImGui::TableNextColumn();
            Quirk::ImguiUIUtility::Text("Projection Type", labelFont);
        
        	ImGui::TableNextColumn();
        
        	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, ImGui::GetStyle().FramePadding.y));
        	ImGui::PushStyleColor(ImGuiCol_Button, frame->GetTheme().GetColor(ColorName::DropdownButton));
        	if (ImGui::Combo("##projectionTypeSelection", &currentProjection, projectionTypes, IM_ARRAYSIZE(projectionTypes))) {
        		auto projectionType = (currentProjection == 1) ? Quirk::SceneCamera::ProjectionType::Orthographic : Quirk::SceneCamera::ProjectionType::Perspective;
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
            Quirk::ImguiUIUtility::Text("Near Plane", labelFont);
        
        	ImGui::TableNextColumn();
        	if (ImGui::DragFloat("##nearPlane", &nearPlane, 0.1f, 0.0f, 0.0f, "%.2f"))
        		component.Camera.SetPerspectiveNearClip(nearPlane);
        
        	ImGui::TableNextRow();
        	ImGui::TableNextColumn();
            Quirk::ImguiUIUtility::Text("Far Plane", labelFont);
        
        	ImGui::TableNextColumn();
        	if (ImGui::DragFloat("##farPlane", &farPlane, 0.1f, 0.0f, 0.0f, "%.2f"))
        		component.Camera.SetPerspectiveFarClip(farPlane);
        
        	ImGui::TableNextRow();
        	ImGui::TableNextColumn();
            Quirk::ImguiUIUtility::Text("Field Of View (FOV)", labelFont);
        
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
            Quirk::ImguiUIUtility::Text("Near Plane", labelFont);
        
        	ImGui::TableNextColumn();
        	if (ImGui::DragFloat("##orthographicNear", &orthographicNear, 0.1f, 0.0f, 0.0f, "%.2f"))
        		component.Camera.SetOrthographicNearClip(orthographicNear);
        
        	ImGui::TableNextRow();
        	ImGui::TableNextColumn();
            Quirk::ImguiUIUtility::Text("Far Plane", labelFont);
        
        	ImGui::TableNextColumn();
        	if (ImGui::DragFloat("##orthographicFar", &orthographicFar, 0.1f, 0.0f, 0.0f, "%.2f"))
        		component.Camera.SetOrthographicFarClip(orthographicFar);
        
        	ImGui::TableNextRow();
        	ImGui::TableNextColumn();
            Quirk::ImguiUIUtility::Text("Orthographic Size", labelFont);
        
        	ImGui::TableNextColumn();
        	if (ImGui::DragFloat("##orthographicSize", &orthographicSize, 0.1f, 0.0f, 70.0f, "%.2f"))
        		component.Camera.SetOrthographicSize(orthographicSize);
        
        	ImGui::EndTable();
        }
    }

    template<>
    static void InspectorDraw<Quirk::Entity>::ComponentDraw(EditorFrame* frame, Quirk::NativeScriptComponent& component) {

    }

    template<>
    static void InspectorDraw<Quirk::Entity>::ComponentDraw(EditorFrame* frame, Quirk::MeshRendererComponent& component) {
        component.MeshObject.Type;
        const char* meshTypes[] = { "Select", "Cube" };
        int currentType = (int)component.MeshObject.Type;
        
        ImGui::Text("Mesh Type");
        ImGui::SameLine();
        if (ImGui::Combo("##meshTypeSelection", &currentType, meshTypes, IM_ARRAYSIZE(meshTypes))) {
        	if (currentType == 1) {
        		component.MeshObject = Quirk::PrimitiveMeshGenerator::Generate(Quirk::MeshType::Cube);
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
    }

    template<>
    static void InspectorDraw<Quirk::Entity>::ComponentDraw(EditorFrame* frame, Quirk::LightComponent& component) {
        const char* lightTypes[] = { "Select", "Point" };
        int currentType = (int)component.Type;
        
        ImGui::Text("Light Type");
        ImGui::SameLine();
        if (ImGui::Combo("##lightTypeSelection", &currentType, lightTypes, IM_ARRAYSIZE(lightTypes))) {
        	component.Type = (Quirk::LightType)currentType;
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
    }

}
