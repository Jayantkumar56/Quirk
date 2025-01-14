

#pragma once

#ifdef QK_DEBUG
#define QK_ENABLE_ASSERTS
#define QK_ENABLE_LOG
#endif // QK_DEBUG
#include "Debug/Log.h"
#include <memory>

// ***********************************  TO DO  *********************************** //
// Make debug break portable to other compilers
#define DEBUG_BREAK __debugbreak()

#ifdef QK_ENABLE_ASSERTS
	#define QK_ASSERT(x, ...)		 { if(!(x)) { QK_ERROR(__VA_ARGS__); DEBUG_BREAK; } }
	#define QK_CORE_ASSERT(x, ...)	 { if(!(x)) { QK_CORE_ERROR(__VA_ARGS__); DEBUG_BREAK; } }
	#define QK_ASSERTEX(x, ...)		 { if(!(x)) { QK_ERROR(__VA_ARGS__); DEBUG_BREAK; } }
	#define QK_CORE_ASSERTEX(x, ...) { if(!(x)) { QK_CORE_ERROR(__VA_ARGS__); DEBUG_BREAK; } }
#else
	#define QK_ASSERT(x, ...) 
	#define QK_CORE_ASSERT(x, ...)
	#define QK_ASSERTEX(x, ...)			x
	#define QK_CORE_ASSERTEX(x, ...)	x
#endif // QK_ENABLE_ASSERTS

// to set xth bit with remaining bits to zero
#define BIT(x)		(1 << x)

// to bind event function with the respective function
#define QK_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

// enables experimental features in glm (for quaternions)
#define GLM_ENABLE_EXPERIMENTAL

namespace Quirk {

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	enum class PointerType {
		Raw, Referenced, Scoped
	};

}
