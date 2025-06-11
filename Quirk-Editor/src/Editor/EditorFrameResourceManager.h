

#pragma once


#include "Core/RHI/Factory.h"
#include "Core/RHI/Texture2D.h"

#include "imgui.h"


namespace QuirkEditor {

    class EditorFrameResourceManager {
        using IconTexture = Quirk::Ref<Quirk::RHI::Texture2D>;

    public:
        EditorFrameResourceManager(Quirk::ConstView<Quirk::RHI::Factory> rhiFactory) :
                m_RHIFactory(rhiFactory)
        {
            InitTitleBarIcons();
            InitSceneViewportPanelIcons();
            InitInspectorPanelIcons();
            InitContentBrowserPanelIcons();
        }

        // ---------------------------------------------------------------------------------------------------------------------
        // TitleBar Icons

        inline ImTextureID GetIconClose()    const noexcept { return (ImTextureID)(intptr_t)m_CloseIcon->GetRendererId();    }
        inline ImTextureID GetIconMinimise() const noexcept { return (ImTextureID)(intptr_t)m_MinimiseIcon->GetRendererId(); }
        inline ImTextureID GetIconMaximise() const noexcept { return (ImTextureID)(intptr_t)m_MaximiseIcon->GetRendererId(); }

        // ---------------------------------------------------------------------------------------------------------------------
        // Scene ViewPort Icons

        inline ImTextureID GetIconPlay()  const noexcept { return (ImTextureID)(intptr_t)m_IconPlay->GetRendererId();  }
        inline ImTextureID GetIconPause() const noexcept { return (ImTextureID)(intptr_t)m_IconPause->GetRendererId(); }

        // ---------------------------------------------------------------------------------------------------------------------
        // Inspector Panel Icons

        inline ImTextureID GetIconRemove() const noexcept { return (ImTextureID)(intptr_t)m_IconRemove->GetRendererId(); }
        inline ImTextureID GetIconUpload() const noexcept { return (ImTextureID)(intptr_t)m_IconUpload->GetRendererId(); }

        // ---------------------------------------------------------------------------------------------------------------------
        // Content Browser Panel

        inline ImTextureID GetIconFile()     const noexcept { return (ImTextureID)(intptr_t)m_IconFile->GetRendererId();     }
        inline ImTextureID GetIconFolder()   const noexcept { return (ImTextureID)(intptr_t)m_IconFolder->GetRendererId();   }
        inline ImTextureID GetIconBackward() const noexcept { return (ImTextureID)(intptr_t)m_IconBackward->GetRendererId(); }
        inline ImTextureID GetIconForward()  const noexcept { return (ImTextureID)(intptr_t)m_IconForward->GetRendererId();  }
        inline ImTextureID GetIconRefresh()  const noexcept { return (ImTextureID)(intptr_t)m_IconRefresh->GetRendererId();  }

    private:
        void InitTitleBarIcons() {
            m_CloseIcon    = LoadIcon( "assets/Images/close.png"    );
            m_MinimiseIcon = LoadIcon( "assets/Images/minus.png"    );
            m_MaximiseIcon = LoadIcon( "assets/Images/maximize.png" );
        }

        void InitSceneViewportPanelIcons() {
            m_IconPlay  = LoadIcon( "assets/Images/play.png"  );
            m_IconPause = LoadIcon( "assets/Images/pause.png" );
        }

        void InitInspectorPanelIcons() {
            m_IconRemove = LoadIcon( "assets/Images/remove.png"      );
            m_IconUpload = LoadIcon( "assets/Images/uploadImage.png" );
        }

        void InitContentBrowserPanelIcons() {
			m_IconFile     = LoadIcon( "assets/Images/file.png"           );
            m_IconFolder   = LoadIcon( "assets/Images/folder.png"         );
            m_IconBackward = LoadIcon( "assets/Images/backward_arrow.png" );
			m_IconForward  = LoadIcon( "assets/Images/forward_arrow.png"  );
			m_IconRefresh  = LoadIcon( "assets/Images/refresh.png"        );
        }

        inline IconTexture LoadIcon(std::string_view path) const noexcept {
            return Quirk::TextureImporter::CreateTexture2D(path, m_RHIFactory);
        }

    private:
        Quirk::ConstView<Quirk::RHI::Factory> m_RHIFactory;

        // --------------------------
        // TitleBar Icons

        IconTexture m_CloseIcon;
        IconTexture m_MinimiseIcon;
        IconTexture m_MaximiseIcon;
        
        // --------------------------
        // Scene ViewPort Icons

        IconTexture m_IconPlay;
        IconTexture m_IconPause;

        // --------------------------
        // Inspector Panel Icons

        IconTexture m_IconRemove;
        IconTexture m_IconUpload;

        // --------------------------
        // Content Browser Panel

        IconTexture m_IconFile;
        IconTexture m_IconFolder;
        IconTexture m_IconBackward;
        IconTexture m_IconForward;
        IconTexture m_IconRefresh;
    };

}
