#pragma once

#include "Aliases.h"

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Lumina 
{
    class Log 
    {
    public:
        static void Init();

        inline static Shared<spdlog::logger>& GetLogger() { return s_Logger; }

    private:
        static Shared<spdlog::logger> s_Logger;
    };
}

#ifdef LUMINA_DEBUG
    #define LUMINA_LOG_INFO(...)  ::Lumina::Log::GetLogger()->info(__VA_ARGS__)
    #define LUMINA_LOG_WARN(...)  ::Lumina::Log::GetLogger()->warn(__VA_ARGS__)
    #define LUMINA_LOG_ERROR(...) ::Lumina::Log::GetLogger()->error(__VA_ARGS__)
#else
    #define LUMINA_LOG_INFO(...)  (void)0
    #define LUMINA_LOG_WARN(...)  (void)0
    #define LUMINA_LOG_ERROR(...) (void)0
#endif
