
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
#define QK_CORE_INFO(...)               ::Quirk::Log::GetCoreLogger()->info(__VA_ARGS__)
#define QK_CORE_TRACE(...)              ::Quirk::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define QK_CORE_WARN(...)               ::Quirk::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define QK_CORE_ERROR(...)              ::Quirk::Log::GetCoreLogger()->error(__VA_ARGS__)
#define QK_CORE_FATAL(...)              ::Quirk::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Core conditional logging macros
#define QK_CORE_INFO_IF(x, ...)         if ((x)) { ::Quirk::Log::GetCoreLogger()->info(__VA_ARGS__);  }
#define QK_CORE_TRACE_IF(x, ...)        if ((x)) { ::Quirk::Log::GetCoreLogger()->trace(__VA_ARGS__); }
#define QK_CORE_WARN_IF(x, ...)         if ((x)) { ::Quirk::Log::GetCoreLogger()->warn(__VA_ARGS__);  }
#define QK_CORE_ERROR_IF(x, ...)        if ((x)) { ::Quirk::Log::GetCoreLogger()->error(__VA_ARGS__); }
#define QK_CORE_FATAL_IF(x, ...)        if ((x)) { ::Quirk::Log::GetCoreLogger()->fatal(__VA_ARGS__); }

// Client Application logging macros
#define QK_INFO(...)                    ::Quirk::Log::GetClientLogger()->info(__VA_ARGS__)
#define QK_TRACE(...)                   ::Quirk::Log::GetClientLogger()->trace(__VA_ARGS__)
#define QK_WARN(...)                    ::Quirk::Log::GetClientLogger()->warn(__VA_ARGS__)
#define QK_ERROR(...)                   ::Quirk::Log::GetClientLogger()->error(__VA_ARGS__)
#define QK_FATAL(...)                   ::Quirk::Log::GetClientLogger()->fatal(__VA_ARGS__)

// Client Application conditional logging macros
#define QK_INFO_IF(x, ...)              if ((x)) { ::Quirk::Log::GetClientLogger()->info(__VA_ARGS__);  }
#define QK_TRACE_IF(x, ...)             if ((x)) { ::Quirk::Log::GetClientLogger()->trace(__VA_ARGS__); }
#define QK_WARN_IF(x, ...)              if ((x)) { ::Quirk::Log::GetClientLogger()->warn(__VA_ARGS__);  }
#define QK_ERROR_IF(x, ...)             if ((x)) { ::Quirk::Log::GetClientLogger()->error(__VA_ARGS__); }
#define QK_FATAL_IF(x, ...)             if ((x)) { ::Quirk::Log::GetClientLogger()->fatal(__VA_ARGS__); }

#else
// Core logging macros
#define QK_CORE_INFO(...)
#define QK_CORE_TRACE(...)
#define QK_CORE_WARN(...)
#define QK_CORE_ERROR(...)
#define QK_CORE_FATAL(...)

// Core conditional logging macros
#define QK_CORE_INFO_IF(...)
#define QK_CORE_TRACE_IF(...)
#define QK_CORE_WARN_IF(...)
#define QK_CORE_ERROR_IF(...)
#define QK_CORE_FATAL_IF(...)

// Client Application logging macros
#define QK_INFO(...)
#define QK_TRACE(...)
#define QK_WARN(...)
#define QK_ERROR(...)
#define QK_FATAL(...)

// Client Application conditional logging macros
#define QK_INFO_IF(x, ...)
#define QK_TRACE_IF(x, ...)
#define QK_WARN_IF(x, ...)
#define QK_ERROR_IF(x, ...)
#define QK_FATAL_IF(x, ...)

#endif // QK_ENABLE_LOG

