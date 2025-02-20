

#pragma once

#include "Quirk.h"

namespace Quirk {

	class ModelImporter : public Panel {
	public:
		ModelImporter() : Panel("Model Importer") { }

		virtual void SetImguiProperties()   override;
		virtual void UnSetImguiProperties() override;

		virtual void OnUpdate()             override { }
		virtual void OnImguiUiUpdate()      override;
		virtual bool OnEvent(Event& event)  override { return false; }

	private:

	};

}

