

#include "QuirkEditorApp.h"
#include "Launcher/LauncherFrame.h"
#include "Editor/EditorFrame.h"

#include "Core/Application/EntryPoint.h"


namespace Quirk {

	// LaunchApp() must be defined in the application
	// Create app object inside this function and call Run method
	void LaunchApp() {
        QuirkEditor::QuirkEditorApp app;
        app.Init();
		app.Run();
        app.Terminate();
	}

}

namespace QuirkEditor {

	void QuirkEditorApp::LaunchEditor() {
		AddFrame<EditorFrame>(m_ProjManager.GetActive());
	}

    void QuirkEditorApp::Init() {
        m_ProjManager.LoadRecentProjectsList("RecentProjects.yaml");

		AddFrame<LauncherFrame>(m_ProjManager);
    }

    void QuirkEditorApp::Terminate() {
        m_ProjManager.SaveRecentProjectsList("RecentProjects.yaml");
    }

}

