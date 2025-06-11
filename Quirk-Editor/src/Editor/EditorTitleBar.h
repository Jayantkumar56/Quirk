
#pragma once

#include "Quirk.h"

#include "Editor/EditorFrame.h"

namespace QuirkEditor {

	class EditorTitleBar : public Quirk::TitleBar{
	public:
		EditorTitleBar(EditorFrame* editor) noexcept : 
				m_EditorFrame(editor) 
		{}

		~EditorTitleBar() = default;

		virtual void SetImguiProperties()   override;
		virtual void UnSetImguiProperties() override;

		virtual void OnImguiUiUpdate() override;
		virtual bool OnEvent(Quirk::Event& event) override { return false; }

	private:
        Quirk::View<EditorFrame> m_EditorFrame;
	};

}
