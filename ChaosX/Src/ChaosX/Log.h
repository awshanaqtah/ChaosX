#pragma once
#include "cspch.h"
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace ChaosX {

    class ChaosX_API Log {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;  // No DLL interface here
        static std::shared_ptr<spdlog::logger> s_ClientLogger; // No DLL interface here
    };
}

// Core Macros
#define CS_CORE_TRACE(...)                   ::ChaosX::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CS_CORE_INFO(...)                    ::ChaosX::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CS_CORE_WARN(...)                    ::ChaosX::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CS_CORE_ERROR(...)                   ::ChaosX::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CS_CORE_FATAL(...)                   ::ChaosX::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client Macros
#define CS_TRACE(...)                        ::ChaosX::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CS_INFO(...)                         ::ChaosX::Log::GetClientLogger()->info(__VA_ARGS__)
#define CS_WARN(...)                         ::ChaosX::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CS_ERROR(...)                        ::ChaosX::Log::GetClientLogger()->error(__VA_ARGS__)
#define CS_FATAL(...)                        ::ChaosX::Log::GetClientLogger()->critical(__VA_ARGS__)
