
#pragma once

#include "Quirk.h"

namespace QuirkEditor {

	class EditorTitleBar : public Quirk::TitleBar{
	public:
		EditorTitleBar() {
			m_CloseIcon    = Quirk::TextureImporter::CreateFromImage("assets/Images/close.png");
			m_MinimiseIcon = Quirk::TextureImporter::CreateFromImage("assets/Images/minus.png");
			m_MaximiseIcon = Quirk::TextureImporter::CreateFromImage("assets/Images/maximize.png");
		}

		~EditorTitleBar() = default;

		virtual void SetImguiProperties()   override;
		virtual void UnSetImguiProperties() override;

		virtual void OnImguiUiUpdate() override;
		virtual bool OnEvent(Quirk::Event& event) override { return false; }

	private:
		Quirk::Ref<Quirk::Texture2D> m_CloseIcon;
		Quirk::Ref<Quirk::Texture2D> m_MinimiseIcon;
		Quirk::Ref<Quirk::Texture2D> m_MaximiseIcon;
	};

}
