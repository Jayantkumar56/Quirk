

#pragma once

#include "Core/Core.h"
#include <utility>
#include <string>

namespace Quirk{

	enum class ImageFormat {
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	struct TextureSpecification {
		uint32_t Width = 1;
		uint32_t Height = 1;
		ImageFormat Format = ImageFormat::RGBA8;
		bool GenerateMips = true;
	};
	
	class Texture {
	public:
		~Texture() = default;
		virtual bool operator ==(const Texture& other) const = 0;

		virtual std::string GetPath() const = 0;
		virtual uint32_t GetRendererId() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void SetData(void* data, uint32_t size) const = 0;
	};

	class Texture2D : public Texture{
	public :
		static Ref<Texture2D> Create(const TextureSpecification& spec);
		static Ref<Texture2D> Create(const std::string& filePath);
	};

}
