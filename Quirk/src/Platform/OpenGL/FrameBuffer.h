

#pragma once

#include "Core/RHI/FrameBuffer.h"


namespace Quirk::OpenGL {

    class FrameBuffer : public RHI::FrameBuffer {
    public:
        FrameBuffer(const RHI::FrameBufferSpecification& spec) noexcept;
        ~FrameBuffer() noexcept;

        virtual void Bind()   noexcept override;
        virtual void Unbind() noexcept override;

        virtual void ClearAttachments() override;
        virtual void SetAttachments(std::initializer_list<RHI::FrameBufferAttachmentSpecification> attachmentsSpec) override;

        virtual uint32_t GetDepthStencilAttachment()      const noexcept override;
        virtual uint32_t GetColorAttachment(size_t index) const noexcept override;

        virtual void Resize(uint32_t width, uint32_t height) override;
        virtual void GetColorPixelData(size_t index, int x, int y, int width, int height, void* outputData, int size) override;

    private:
        uint32_t CreateBuffer(
            int          internalFormat, 
            unsigned int format, 
            unsigned int dataType, 
            RHI::FrameBufferAttachmentSpecification spec
        ) const;

        void CreateAttachments();
        void InvalidateAttachments();

    private:
        uint32_t m_RendererID;
        uint32_t m_DepthStencilAttachment = 0;
        std::vector<uint32_t> m_ColorAttachments;

        RHI::FrameBufferSpecification                        m_FrameBufferSpec;
        RHI::FrameBufferAttachmentSpecification              m_DepthAttachmentSpec;
        std::vector<RHI::FrameBufferAttachmentSpecification> m_ColorAttachmentsSpec;
    };

}
