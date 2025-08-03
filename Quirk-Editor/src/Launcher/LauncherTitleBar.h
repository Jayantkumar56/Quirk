

#pragma once

#include "LauncherFrame.h"

#include "Quirk.h"
#include "Core/Utility/View.h"
#include "Core/RHI/Factory.h"
#include "Core/AssetImporter/TextureImporter.h"


namespace QuirkEditor {

	class LauncherTitleBar : public Quirk::TitleBar {

	public:
		LauncherTitleBar(Quirk::ConstView<LauncherFrame> frame) {
			Quirk::ConstView<Quirk::RHI::Factory> factory = frame->GetRenderSystem()->GetFactory();

			m_CloseIcon    = Quirk::TextureImporter::CreateTexture2D( "assets/Images/close.png",    factory );
			m_MinimiseIcon = Quirk::TextureImporter::CreateTexture2D( "assets/Images/minus.png",    factory );
			m_MaximiseIcon = Quirk::TextureImporter::CreateTexture2D( "assets/Images/maximize.png", factory );
		}

		~LauncherTitleBar() = default;

		virtual void SetImguiProperties()   override;
		virtual void UnSetImguiProperties() override;

		virtual void OnImguiUiUpdate()     override;
		virtual bool OnEvent(Quirk::Event& event) override { return false; }

	private:
		Quirk::Ref<Quirk::RHI::Texture2D> m_CloseIcon;
		Quirk::Ref<Quirk::RHI::Texture2D> m_MinimiseIcon;
		Quirk::Ref<Quirk::RHI::Texture2D> m_MaximiseIcon;
	};

}
