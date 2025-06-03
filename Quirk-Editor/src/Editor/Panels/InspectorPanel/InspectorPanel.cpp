


#include "QkEditorpch.h"

#include "InspectorPanel.h"
#include "EntityInspectorUI.h"
#include "Editor/EditorFrame.h"

#include "imgui.h"
#include "imgui_internal.h"


namespace QuirkEditor {

    void InspectorPanel::SetUiProperties() {
        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;
        ImGui::SetNextWindowClass(&window_class);
    }

    void InspectorPanel::OnUiUpdate() {
        if (m_SelectionHandle.Has<Quirk::Entity>()) {
            InspectorDraw<Quirk::Entity>::Draw(m_EditorFrame, m_SelectionHandle.Get<Quirk::Entity>());
        }
    }

}
