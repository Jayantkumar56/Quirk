

#pragma once


#include "Quirk.h"


namespace Quirk {

	class QuirkEditorApp : public Application {
	public:
		QuirkEditorApp();
		~QuirkEditorApp() = default;


		// creates the editor frame with default parameters
		void LaunchEditor();
	};

}


