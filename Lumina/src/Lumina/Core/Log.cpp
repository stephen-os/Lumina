#include "Log.h"

namespace Lumina 
{
    std::shared_ptr<spdlog::logger> Log::s_Logger;

    void Log::Init() 
    {
        spdlog::set_pattern("[%T] [%^%l%$] %v");
        s_Logger = spdlog::stdout_color_mt("Lumina");
        s_Logger->set_level(spdlog::level::trace);
    }
}