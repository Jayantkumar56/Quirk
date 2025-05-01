

#pragma once


#include "Core/Assets/RenderAssets/Texture.h"


namespace QuirkEditor {

    class EditorResourceManager {
    public:
        EditorResourceManager() {
            m_IconRemove = Quirk::Texture2D::Create("assets/Images/remove.png");
            m_IconUpload = Quirk::Texture2D::Create("assets/Images/uploadImage.png");
        }

        inline const Quirk::Texture2D& GetIconRemove() const noexcept { return *m_IconRemove; }
        inline const Quirk::Texture2D& GetIconUpload() const noexcept { return *m_IconUpload; }

    private:
        Quirk::Ref<Quirk::Texture2D> m_IconRemove;
        Quirk::Ref<Quirk::Texture2D> m_IconUpload;
    };

}

