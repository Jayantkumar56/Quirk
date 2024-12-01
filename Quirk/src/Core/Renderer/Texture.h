

#pragma once

#include "Core/Core.h"
#include <utility>
#include <string>

namespace Quirk{

	enum class ImageFormat : uint8_t{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	enum class TextureFilter : uint8_t {
		Linear,				Nearest
	};

	enum class TextureWrap : uint8_t {
		Repeat,				MirroredRepeat,
		ClampToEdge,		ClampToBorder
	};

	struct TextureSpecification {
		uint32_t	Width  = 1;
		uint32_t	Height = 1;
		ImageFormat Format = ImageFormat::RGBA8;
		bool GenerateMips  = true;
	};
	
	class Texture {
	public:
		~Texture() = default;
		virtual bool operator ==(const Texture& other) const = 0;

		virtual std::filesystem::path GetPath() const = 0;
		virtual uint32_t GetRendererId() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void SetData(void* data, uint32_t size) const = 0;
	};

	class Texture2D : public Texture{
	public :
		static Ref<Texture2D> Create(const TextureSpecification& spec);
		static Ref<Texture2D> Create(const std::filesystem::path& filePath);
	};

	class SubTexture2D : public Texture {
	public:
		// indexX, indexY represents the index at which subtexture lie in the original texture
		// indexes are 0 based (i.e. index starts with 0)
		// sizeX, sizeY represent the size of the subtexture
		SubTexture2D(Ref<Texture2D>& texture, uint16_t coordX, uint16_t coordY, uint16_t width, uint16_t height);
		SubTexture2D(const std::string& filePath, uint16_t coordX, uint16_t coordY, uint16_t width, uint16_t height);
		SubTexture2D(const TextureSpecification& spec, uint16_t coordX, uint16_t coordY, uint16_t width, uint16_t height);

		virtual bool operator ==(const Texture& other) const override { 
			return m_Texture->GetRendererId() == other.GetRendererId();
		}

		virtual std::filesystem::path GetPath()	 const override { return m_Texture->GetPath(); }
		virtual uint32_t GetRendererId() const override { return m_Texture->GetRendererId(); }
		virtual uint32_t GetWidth()		 const override { return m_Texture->GetWidth(); }
		virtual uint32_t GetHeight()	 const override { return m_Texture->GetHeight(); }

		inline uint16_t GetTextureCoordX() const { return m_CoordX; }
		inline uint16_t GetTextureCoordY() const { return m_CoordY; }
		inline uint16_t GetTileWidth()	   const { return m_CoordX; }
		inline uint16_t GetTileHeight()    const { return m_CoordY; }

		virtual void Bind(uint32_t slot = 0)			const override { m_Texture->Bind(); }
		virtual void SetData(void* data, uint32_t size) const override { m_Texture->SetData(data, size); }

	private:
		Ref<Texture2D> m_Texture;
		uint16_t m_CoordX, m_CoordY;
		uint16_t m_TileWidth, m_TileHeight;
	};

}
