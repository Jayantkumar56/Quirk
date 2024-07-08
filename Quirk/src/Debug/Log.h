
#pragma once

#include "Core/core.h"
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
#define QK_CORE_INFO(...)     ::Quirk::Log::GetCoreLogger()->info(__VA_ARGS__)
#define QK_CORE_TRACE(...)    ::Quirk::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define QK_CORE_WARN(...)     ::Quirk::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define QK_CORE_ERROR(...)    ::Quirk::Log::GetCoreLogger()->error(__VA_ARGS__)
#define QK_CORE_FATAL(...)    ::Quirk::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Core logging macros
#define QK_INFO(...)          ::Quirk::Log::GetClientLogger()->info(__VA_ARGS__)
#define QK_TRACE(...)         ::Quirk::Log::GetClientLogger()->trace(__VA_ARGS__)
#define QK_WARN(...)          ::Quirk::Log::GetClientLogger()->warn(__VA_ARGS__)
#define QK_ERROR(...)         ::Quirk::Log::GetClientLogger()->error(__VA_ARGS__)
#define QK_FATAL(...)         ::Quirk::Log::GetClientLogger()->fatal(__VA_ARGS__)

#endif // QK_ENABLE_LOG

