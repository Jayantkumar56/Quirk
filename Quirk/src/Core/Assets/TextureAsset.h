

#pragma once

#include "Core/RHI/Texture2D.h"

#include <filesystem>

// NOTE: Temporary until a common define is created for editor and runtime
#ifndef ENABLE_EDITOR_METADATA
#define ENABLE_EDITOR_METADATA
#endif // !ENABLE_EDITOR_METADATA


namespace Quirk {

    class TextureAsset {
    public:
        TextureAsset() noexcept = default;

        TextureAsset(
                std::filesystem::path sourcePath,
                RHI::TextureFilter    minFilter,
                RHI::TextureFilter    magFilter,
                RHI::TextureWrap      wrapS, 
                RHI::TextureWrap      wrapT,
                RHI::TextureSwizzle   swizzleMask
        ) : m_SourcePath  ( std::move(sourcePath) ),
            m_MinFilter   ( minFilter             ),
            m_MagFilter   ( magFilter             ),
            m_WrapS       ( wrapS                 ),
            m_WrapT       ( wrapT                 ),
            m_SwizzleMask ( swizzleMask           ),
            m_DirtyFlag   ( false                 ),
            m_RHITexture  ( nullptr               )
        { }

        inline void SetMinFilter(RHI::TextureFilter minFilter) {
            if (m_MinFilter == minFilter)
                return;

            m_MinFilter = minFilter;

            if (m_RHITexture)
                //m_RHITexture->SetMinFilter(minFilter);

            MarkDirty();
        }

        inline void SetMagFilter(RHI::TextureFilter magFilter) {
            if (m_MagFilter == magFilter)
                return;

            m_MagFilter = magFilter;

            if (m_RHITexture)
                //m_RHITexture->SetMagFilter(magFilter);

            MarkDirty();
        }

        inline void SetWrapS(RHI::TextureWrap wrapS) {
            if (m_WrapS == wrapS)
                return;

            m_WrapS = wrapS;

            if (m_RHITexture)
                //m_RHITexture->SetWrapS(wrapS);

            MarkDirty();
        }

        inline void SetWrapT(RHI::TextureWrap wrapT) {
            if (m_WrapT == wrapT)
                return;

            m_WrapT = wrapT;

            if (m_RHITexture)
                //m_RHITexture->SetWrapT(wrapT);

            MarkDirty();
        }

        inline void SetSwizzleMask(RHI::TextureSwizzle swizzleMask) {
            if (m_SwizzleMask == swizzleMask)
                return;

            m_SwizzleMask = swizzleMask;

            if (m_RHITexture)
                //m_RHITexture->SetSwizzleMask(swizzleMask);

            MarkDirty();
        }

        inline void SetRHITexture(Ref<RHI::Texture2D> texture2d) { m_RHITexture = std::move(texture2d); }

        inline void MarkDirty()      noexcept { m_DirtyFlag = true;  }
        inline void ClearDirtyFlag() noexcept { m_DirtyFlag = false; }

        inline bool IsDirty() const noexcept { return m_DirtyFlag; }

        inline RHI::TextureFilter  GetMinFilter()   const noexcept { return m_MinFilter;   }
        inline RHI::TextureFilter  GetMagFilter()   const noexcept { return m_MagFilter;   }
        inline RHI::TextureWrap    GetWrapS()       const noexcept { return m_WrapS;       }
        inline RHI::TextureWrap    GetWrapT()       const noexcept { return m_WrapT;       }
        inline RHI::TextureSwizzle GetSwizzleMask() const noexcept { return m_SwizzleMask; }

#ifdef ENABLE_EDITOR_METADATA

        inline void SetSourcePath(std::filesystem::path sourcePath) { m_SourcePath = std::move(sourcePath); }
        inline const std::filesystem::path& GetSourcePath() const noexcept { return m_SourcePath; }

#endif // ENABLE_EDITOR_METADATA

    private:
        bool m_DirtyFlag;

        RHI::TextureFilter  m_MinFilter{ RHI::TextureFilter::Linear };
        RHI::TextureFilter  m_MagFilter{ RHI::TextureFilter::Linear };

        RHI::TextureWrap    m_WrapS{ RHI::TextureWrap::Repeat };
        RHI::TextureWrap    m_WrapT{ RHI::TextureWrap::Repeat };

        RHI::TextureSwizzle m_SwizzleMask{ };

        Ref<RHI::Texture2D> m_RHITexture;

#ifdef ENABLE_EDITOR_METADATA

        std::filesystem::path m_SourcePath;

#endif // ENABLE_EDITOR_METADATA
    };

}
