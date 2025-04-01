

#pragma once

#include <utility>
#include <stdlib.h>
#include <cstring>

namespace Quirk {

	// raii based owning buffer
	struct Buffer {
		uint8_t* Data = nullptr;
		size_t   Size = 0;

		Buffer() = default;

		Buffer(void* data, size_t size) : 
				Data ( static_cast<decltype(Data)>(data) ), 
				Size ( size                              ) 
		{
		}

		Buffer(size_t size) {
			Allocate(size);
		}

		Buffer(Buffer&& other) noexcept {
			Size = other.Size;
			Data = other.Data;

			other.Size = 0;
			other.Data = nullptr;
		}

		Buffer(const Buffer& other) noexcept {
			Allocate(other.Size);

			if (Data && other.Data && Size > 0) {
				Size = other.Size;
				memcpy(Data, other.Data, Size);
			}
		}

		~Buffer() {
			Release();
		}

		inline Buffer& operator=(Buffer&& other) noexcept {
			if (this != &other) {
				Release();

				Size = other.Size;
				Data = other.Data;

				other.Size = 0;
				other.Data = nullptr;
			}

			return *this;
		}

		inline Buffer& operator=(const Buffer& other) noexcept {
			if (this != &other) {
				Allocate(other.Size);

				if (Data && other.Data && Size > 0) {
					Size = other.Size;
					memcpy(Data, other.Data, Size);
				}
			}

			return *this;
		}

		inline void Allocate(size_t size) noexcept {
			if (size == Size) 
				return;

			Release();
			Size = size;
			Data = static_cast<decltype(Data)>(malloc(size));
		}

		inline void Release() noexcept {
			free(Data);

			Size = 0;
			Data = nullptr;
		}

		inline operator bool() const noexcept {
			return Data != nullptr && Size > 0;
		}
	};

	// non owning buffer view (the data must live longer than the view)
	struct BufferView {
		uint8_t* Data = nullptr;
		size_t   Size = 0;

		BufferView() = default;

		BufferView(void* data, size_t size) :
				Data ( static_cast<decltype(Data)>(data) ),
				Size ( size                              )
		{
		}

		BufferView(Buffer& buffer) :
				Data ( buffer.Data ),
				Size ( buffer.Size )
		{
		}

		BufferView(BufferView&& other)      noexcept = default;
		BufferView(const BufferView& other) noexcept = default;

		BufferView& operator=(BufferView&& other)      noexcept = default;
		BufferView& operator=(const BufferView& other) noexcept = default;

		inline operator bool() const noexcept {
			return Data != nullptr && Size > 0;
		}
	};

}
