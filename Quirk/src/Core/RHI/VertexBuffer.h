

#pragma once

#include "Core/Core.h"

#include <utility>
#include <string>


namespace Quirk::RHI {

	enum class ShaderDataType {
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::Float:    return 4;
			case ShaderDataType::Float2:   return 4 * 2;
			case ShaderDataType::Float3:   return 4 * 3;
			case ShaderDataType::Float4:   return 4 * 4;
			case ShaderDataType::Mat3:     return 4 * 3 * 3;
			case ShaderDataType::Mat4:     return 4 * 4 * 4;
			case ShaderDataType::Int:      return 4;
			case ShaderDataType::Int2:     return 4 * 2;
			case ShaderDataType::Int3:     return 4 * 3;
			case ShaderDataType::Int4:     return 4 * 4;
			case ShaderDataType::Bool:     return 1;
		}

		QK_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	static uint32_t GetComponentCount(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3 * 3;
			case ShaderDataType::Mat4:    return 4 * 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
		}

		QK_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement {
		std::string_view Name;
		ShaderDataType   Type;

		BufferElement(ShaderDataType type, const std::string& name) noexcept : 
				Name ( name ), 
				Type ( type ) 
		{}

		inline bool IsIntType() const {
			return Type == ShaderDataType::Int  || Type == ShaderDataType::Int2 || 
				   Type == ShaderDataType::Int3 || Type == ShaderDataType::Int4;
		}
		inline bool IsFloatType() const {
			return Type == ShaderDataType::Float  || Type == ShaderDataType::Float2 || 
				   Type == ShaderDataType::Float3 || Type == ShaderDataType::Float4;
		}
	};

	class BufferLayout {
	public:
		BufferLayout() = default;

		BufferLayout(std::initializer_list<BufferElement> elements): 
				m_Elements(elements)
		{
			for (auto& element : m_Elements) {
				m_Stride += ShaderDataTypeSize(element.Type);
			}
		}

		inline uint32_t GetStride()                            const { return m_Stride;   }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin()             { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end()               { return m_Elements.end();   }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end()   const { return m_Elements.end();   }

	private: 
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() noexcept = default;

		virtual void UploadData(const void* data, uint32_t size, uint32_t offset = 0) noexcept = 0;

		virtual void Bind()   const noexcept = 0;
		virtual void Unbind() const noexcept = 0;

		inline void SetLayout(const BufferLayout& layout) noexcept { m_Layout = layout; }

		inline uint32_t            GetCount()  const noexcept { return m_Size / m_Layout.GetStride(); }
		inline uint32_t            GetSize()   const noexcept { return m_Size;                        }
		inline const BufferLayout& GetLayout() const noexcept { return m_Layout;                      }

	protected:
		uint32_t     m_Size   {};
		BufferLayout m_Layout {};
	};

}
