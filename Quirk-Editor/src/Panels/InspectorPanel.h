

#pragma once

#include "Quirk.h"

namespace Quirk {

	class InspectorPanel : public Panel {
	public:
		InspectorPanel() {
			m_RemoveImage = Texture2D::Create("assets/Images/remove.png");
			m_UploadImage = Texture2D::Create("assets/Images/uploadImage.png");
		}

		~InspectorPanel() = default;

		virtual void OnUpdate() override { }
		virtual bool OnEvent(Event& event) { return false; }
		virtual void OnImguiUiUpdate() override;

	private:
		Ref<Texture> m_RemoveImage;
		Ref<Texture> m_UploadImage;
	};

}
