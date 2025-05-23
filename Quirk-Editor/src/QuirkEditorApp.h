

#pragma once

#include "Base/ProjectManager.h"

#include "Core/Application/Application.h"


// forward declaration for friend funtion in QuirkEditorApp to work
namespace Quirk {
    void LaunchApp();
}

namespace QuirkEditor {

	class QuirkEditorApp : public Quirk::Application {
        friend void Quirk::LaunchApp();

	public:
		QuirkEditorApp() :
                Application(L"Quirk Engine", Quirk::RendererAPI::API::OpenGL)
        {}

		// creates the editor frame with default parameters
		void LaunchEditor();

        const auto& GetProjectManagerView() const noexcept { return m_ProjManager; }

	private:
        void Init();
        void Terminate();

		void LoadFontFiles();

    private:
        ProjectManager m_ProjManager;
	};

}
