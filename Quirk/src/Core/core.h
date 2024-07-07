

#pragma once

// ***********************************  TO DO  *********************************** //
// Make debug break portable to other compilers
#define DEBUG_BREAK __debugbreak()

#ifdef QK_ENABLE_ASSERTS
	#define QK_ASSERT(x, ...) { if(!(x)) { DEBUG_BREAK; } }
	#define QK_CORE_ASSERT(x, ...) { if(!(x)) { DEBUG_BREAK; } }
#else
	#define QK_ASSERT(x, ...) 
	#define QK_CORE_ASSERT(x, ...) 
#endif // QK_ENABLE_ASSERTS

// to create a set xth bit
#define BIT(x)		(1 << x)
