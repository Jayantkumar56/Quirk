
#pragma once

#ifdef QK_ENABLE_LOG

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Quirk{

	class  Log {
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}


// Core logging macros
#define QK_CORE_INFO(...)     Log::GetCoreLogger()->info(__VA_ARGS__)
#define QK_CORE_TRACE(...)    Log::GetCoreLogger()->trace(__VA_ARGS__)
#define QK_CORE_WARN(...)     Log::GetCoreLogger()->warn(__VA_ARGS__)
#define QK_CORE_ERROR(...)    Log::GetCoreLogger()->error(__VA_ARGS__)
#define QK_CORE_FATAL(...)    Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Core logging macros
#define QK_INFO(...)          Log::GetClientLogger()->info(__VA_ARGS__)
#define QK_TRACE(...)         Log::GetClientLogger()->trace(__VA_ARGS__)
#define QK_WARN(...)          Log::GetClientLogger()->warn(__VA_ARGS__)
#define QK_ERROR(...)         Log::GetClientLogger()->error(__VA_ARGS__)
#define QK_FATAL(...)         Log::GetClientLogger()->fatal(__VA_ARGS__)

#else
#define QK_CORE_INFO(...) 
#define QK_CORE_TRACE(...)
#define QK_CORE_WARN(...) 
#define QK_CORE_ERROR(...)
#define QK_CORE_FATAL(...)

// Core logging macros
#define QK_INFO(...)      
#define QK_TRACE(...)     
#define QK_WARN(...)      
#define QK_ERROR(...)     
#define QK_FATAL(...)     

#endif // QK_ENABLE_LOG

