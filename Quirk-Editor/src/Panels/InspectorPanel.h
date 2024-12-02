

#pragma once

#include "Quirk.h"

namespace Quirk {

	class InspectorPanel {
	public:
		InspectorPanel() {
			m_RemoveImage = Texture2D::Create("assets/Images/remove.png");
			m_UploadImage = Texture2D::Create("assets/Images/uploadImage.png");
		}

		~InspectorPanel() = default;

		void OnImguiUiUpdate(Entity& entity);

	private:
		Ref<Texture> m_RemoveImage;
		Ref<Texture> m_UploadImage;
	};

}
