

#pragma once

#include "Quirk.h"

namespace Quirk {

	class InspectorPanel : public Panel {
	public:
		InspectorPanel() : Panel("Inspector") {
			m_RemoveImage = Texture2D::Create("assets/Images/remove.png");
			m_UploadImage = Texture2D::Create("assets/Images/uploadImage.png");
		}

		~InspectorPanel() = default;

		virtual void SetImguiProperties()   override;
		virtual void UnSetImguiProperties() override;
		virtual void OnImguiUiUpdate()      override;

	private:
		Ref<Texture> m_RemoveImage;
		Ref<Texture> m_UploadImage;
	};

}
