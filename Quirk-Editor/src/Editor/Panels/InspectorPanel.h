

#pragma once

#include "Quirk.h"

namespace QuirkEditor {

	class InspectorPanel : public Quirk::Panel {
	public:
		InspectorPanel() : Quirk::Panel("Inspector") {
			m_RemoveImage = Quirk::Texture2D::Create("assets/Images/remove.png");
			m_UploadImage = Quirk::Texture2D::Create("assets/Images/uploadImage.png");
		}

		~InspectorPanel() = default;

		virtual void SetImguiProperties()   override;
		virtual void OnImguiUiUpdate()      override;

	private:
		Quirk::Ref<Quirk::Texture2D> m_RemoveImage;
		Quirk::Ref<Quirk::Texture2D> m_UploadImage;
	};

}
