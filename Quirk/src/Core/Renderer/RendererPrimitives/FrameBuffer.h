

#pragma once

#include "Core/Core.h"
#include "Core/Renderer/Texture.h"

namespace Quirk {

	enum class FrameBufferTextureType : uint8_t {
		None = 0,
		
		// color attachments
		RGBA_8,
		RGB_8,
		RED_INTEGER,

		// depth and stencile attachments
		DEPTH_24_STENCIL_8
	};

	union ClearAttachmentData {
		float RGBA[4];
		int	  RedInteger;
		float DepthValue;
	};

	struct FrameBufferAttachmentSpecification {
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(FrameBufferTextureType type, ClearAttachmentData clearData = { 0.0f, 0.0f, 0.0f, 1.0f }, 
			TextureWrap wrap = TextureWrap::Repeat, TextureFilter minFilter = TextureFilter::Linear, TextureFilter magFilter = TextureFilter::Linear) :
				Type			  (type),
				ClearData		  (clearData),
				WrapMode		  (wrap),
				MinificationFilter(minFilter),
				MagficationFilter (magFilter)
		{
		}

		FrameBufferTextureType Type		 = FrameBufferTextureType::None;
		ClearAttachmentData    ClearData = {0.0f, 0.0f, 0.0f, 1.0f};
		TextureWrap			   WrapMode  = TextureWrap::Repeat;
		TextureFilter MinificationFilter = TextureFilter::Linear;
		TextureFilter MagficationFilter  = TextureFilter::Linear;
	};

	struct FrameBufferSpecification {
		uint16_t Width = 0, Height = 0;
	};

	class FrameBuffer {
	public:
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);

		static inline bool IsColorAttachment(FrameBufferTextureType type) {
			bool isColorType = type == FrameBufferTextureType::RGBA_8;
			isColorType		 = isColorType || (type == FrameBufferTextureType::RGB_8);
			isColorType		 = isColorType || (type == FrameBufferTextureType::RED_INTEGER);
			return isColorType;
		}

		static inline bool IsDepthAttachment(FrameBufferTextureType type) {
			bool isDepthType = type == FrameBufferTextureType::DEPTH_24_STENCIL_8;
			return isDepthType;
		}

		virtual ~FrameBuffer() = default;

		virtual inline void Bind()	  = 0;
		virtual inline void Unbind()  = 0;

		virtual void ClearAttachments() = 0;
		virtual void SetAttachments(std::initializer_list<FrameBufferAttachmentSpecification> attachmentsSpec) = 0;

		virtual inline uint32_t GetDepthStencilAttachment() const = 0;
		virtual inline uint32_t GetColorAttachment(size_t index) const = 0;

		virtual inline FrameBufferSpecification GetSpecification() const = 0;
		virtual inline FrameBufferAttachmentSpecification GetDepthAttachmentSpecification() const = 0;
		virtual inline const std::vector<FrameBufferAttachmentSpecification>& GetColorAttachmentSpecification() const = 0;

		virtual void Resize(uint16_t width, uint16_t height) = 0;
		virtual void GetColorPixelData(size_t index, int x, int y, void* outputData, int dataCount) = 0;
	};

}
