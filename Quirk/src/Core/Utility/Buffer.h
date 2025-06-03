

#pragma once

#include <utility>
#include <stdlib.h>
#include <cstring>

namespace Quirk {

	// raii based owning buffer
	class Buffer {
    public:
		Buffer() noexcept = default;

		Buffer(void* data, size_t size) noexcept : 
				m_Data ( static_cast<uint8_t*>(data) ),
				m_Size ( size                        ) 
		{}

		Buffer(size_t size) noexcept {
			Allocate(size);
		}

		Buffer(Buffer&& other) noexcept {
            m_Size = other.m_Size;
			m_Data = other.m_Data;

			other.m_Size = 0;
			other.m_Data = nullptr;
		}

        // to prevent accidental copy in construction and assignment 
        // use dedicated Copy function instead
        Buffer(const Buffer& other)                   noexcept = delete;
        inline Buffer& operator=(const Buffer& other) noexcept = delete;

		~Buffer() noexcept { Release(); }

		inline Buffer& operator=(Buffer&& other) noexcept {
			if (this != &other) {
				Release();

                m_Size = other.m_Size;
				m_Data = other.m_Data;

				other.m_Size = 0;
				other.m_Data = nullptr;
			}

			return *this;
		}

		inline operator bool() const noexcept {
			return m_Data != nullptr && m_Size > 0;
		}

        inline uint8_t* Get()  const noexcept { return m_Data; }
        inline size_t   Size() const noexcept { return m_Size; }

        inline Buffer Copy() const noexcept{
            Buffer newCopy(m_Size);
            memcpy(newCopy.m_Data, m_Data, m_Size);

            return newCopy;
        }

    private:
		inline void Allocate(size_t size) noexcept {
			if (size == 0 || size == m_Size)
				return;

			Release();
            m_Size = size;
			m_Data = static_cast<uint8_t*>(malloc(size));
		}

		inline void Release() noexcept {
            if (m_Size)
			    free(m_Data);

            m_Size = 0;
			m_Data = nullptr;
		}

    private:
        uint8_t* m_Data = nullptr;
        size_t   m_Size = 0;
	};

	// non owning buffer view (the data must live longer than the view)
	class BufferView {
    public:
		BufferView() noexcept = default;

		BufferView(const void* data, const size_t size) noexcept :
				m_Data ( static_cast<const uint8_t*>(data) ),
				m_Size ( size                              )
		{}

		BufferView(const Buffer& buffer) noexcept :
				m_Data ( buffer.Get()  ),
				m_Size ( buffer.Size() )
		{}

        // if temporary rvalue buffer is provided it will get deallocated 
        // just after execution of this line (i.e. construction of BufferView object)
        // thus left with dangling pointer in this BufferView object
        BufferView(Buffer&& buffer) noexcept = delete;

		inline operator bool() const noexcept {
			return m_Data != nullptr && m_Size > 0;
		}

        inline const uint8_t* Get()  const noexcept { return m_Data; }
        inline const size_t   Size() const noexcept { return m_Size; }

    private:
		const uint8_t* m_Data = nullptr;
		const size_t   m_Size = 0;
	};

}
