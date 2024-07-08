

#pragma once

#ifdef QK_DEBUG
#define QK_ENABLE_ASSERTS
#define QK_ENABLE_LOG
#include "Debug/Log.h"
#endif // QK_DEBUG

// ***********************************  TO DO  *********************************** //
// Make debug break portable to other compilers
#define DEBUG_BREAK __debugbreak()

#ifdef QK_ENABLE_ASSERTS
	#define QK_ASSERT(x, ...)		 { if(!(x)) { QK_CORE_ERROR(__VA_ARGS__); DEBUG_BREAK; } }
	#define QK_CORE_ASSERT(x, ...)	 { if(!(x)) { QK_CORE_ERROR(__VA_ARGS__); DEBUG_BREAK; } }
	#define QK_ASSERTEX(x, ...)		 { if(!(x)) { QK_CORE_ERROR(__VA_ARGS__); DEBUG_BREAK; } }
	#define QK_CORE_ASSERTEX(x, ...) { if(!(x)) { QK_CORE_ERROR(__VA_ARGS__); DEBUG_BREAK; } }
#else
	#define QK_ASSERT(x, ...) 
	#define QK_CORE_ASSERT(x, ...)
	#define QK_ASSERTEX(x, ...)			x
	#define QK_CORE_ASSERTEX(x, ...)	x
#endif // QK_ENABLE_ASSERTS

// to set xth bit with remaining bits to zero
#define BIT(x)		(1 << x)
