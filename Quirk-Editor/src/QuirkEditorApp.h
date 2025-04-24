

#pragma once


#include "Quirk.h"


namespace QuirkEditor {

	class QuirkEditorApp : public Quirk::Application {
	public:
		QuirkEditorApp();
		~QuirkEditorApp() = default;

		// creates the editor frame with default parameters
		void LaunchEditor();

	private:
		void LoadFontFiles();
	};

}


