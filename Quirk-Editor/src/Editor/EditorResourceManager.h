

#pragma once


#include "Core/Assets/RenderAssets/Texture.h"


namespace QuirkEditor {

    class EditorResourceManager {
    public:
        EditorResourceManager() {
            InitInspectorPanelIcons();
            InitContentBrowserPanelIcons();
        }

        // ---------------------------------------------------------------------------------------------------------------------
        // Inspector Panel Icons

        inline ImTextureID GetIconRemove() noexcept { return (ImTextureID)(intptr_t)m_IconRemove->GetRendererId(); }
        inline ImTextureID GetIconUpload() noexcept { return (ImTextureID)(intptr_t)m_IconUpload->GetRendererId(); }

        // ---------------------------------------------------------------------------------------------------------------------
        // Content Browser Panel

        inline ImTextureID GetIconFile()     noexcept { return (ImTextureID)(intptr_t)m_IconFile->GetRendererId();     }
        inline ImTextureID GetIconFolder()   noexcept { return (ImTextureID)(intptr_t)m_IconFolder->GetRendererId();   }
        inline ImTextureID GetIconBackward() noexcept { return (ImTextureID)(intptr_t)m_IconBackward->GetRendererId(); }
        inline ImTextureID GetIconForward()  noexcept { return (ImTextureID)(intptr_t)m_IconForward->GetRendererId();  }
        inline ImTextureID GetIconRefresh()  noexcept { return (ImTextureID)(intptr_t)m_IconRefresh->GetRendererId();  }

    private:
        void InitInspectorPanelIcons() {
            m_IconRemove = Quirk::Texture2D::Create( "assets/Images/remove.png"      );
            m_IconUpload = Quirk::Texture2D::Create( "assets/Images/uploadImage.png" );
        }

        void InitContentBrowserPanelIcons() {
			m_IconFile     = Quirk::Texture2D::Create( "assets/Images/file.png"           );
            m_IconFolder   = Quirk::Texture2D::Create( "assets/Images/folder.png"         );
            m_IconBackward = Quirk::Texture2D::Create( "assets/Images/backward_arrow.png" );
			m_IconForward  = Quirk::Texture2D::Create( "assets/Images/forward_arrow.png"  );
			m_IconRefresh  = Quirk::Texture2D::Create( "assets/Images/refresh.png"        );
        }

    private:
        // ---------------------------------------------------------------------------------------------------------------------
        // Inspector Panel Icons

        Quirk::Ref<Quirk::Texture2D> m_IconRemove;
        Quirk::Ref<Quirk::Texture2D> m_IconUpload;

        // ---------------------------------------------------------------------------------------------------------------------
        // Content Browser Panel

        Quirk::Ref<Quirk::Texture2D> m_IconFile;
        Quirk::Ref<Quirk::Texture2D> m_IconFolder;
        Quirk::Ref<Quirk::Texture2D> m_IconBackward;
        Quirk::Ref<Quirk::Texture2D> m_IconForward;
        Quirk::Ref<Quirk::Texture2D> m_IconRefresh;
    };

}

