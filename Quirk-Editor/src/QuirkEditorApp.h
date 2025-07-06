

#pragma once

#include "Base/ProjectManager.h"

#include "Core/Application/Application.h"


// forward declaration for friend funtion in QuirkEditorApp to work
namespace Quirk {
    void LaunchApp();
}

namespace QuirkEditor {

	class QuirkEditorApp final : public Quirk::Application {
        friend void Quirk::LaunchApp();

	public:
		QuirkEditorApp() noexcept :
                Application(L"Quirk Engine")
        {}

		// creates the editor frame with default parameters
		void LaunchEditor();

	private:
        void Init();
        void Terminate();

    private:
        ProjectManager m_ProjManager;
	};

}
