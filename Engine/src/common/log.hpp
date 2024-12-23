#pragma once
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
namespace Engine::Core {
class Log {
public:
  static void Init();
  inline static std::shared_ptr<spdlog::logger> GetEngineLogger() { return m_engineLogger; }

private:
  static std::shared_ptr<spdlog::logger> m_engineLogger;
};
} // namespace Engine::Core

#define ENGINE_CRITICAL(...)                                                                                                                         \
  {                                                                                                                                                  \
    Engine::Core::Log::GetEngineLogger()->critical(__VA_ARGS__);                                                                                     \
    __debugbreak();                                                                                                                                  \
    std::exit(1);                                                                                                                                    \
  }
#define ENGINE_ERROR(...)                                                                                                                            \
  {                                                                                                                                                  \
    Engine::Core::Log::GetEngineLogger()->error(__VA_ARGS__);                                                                                        \
    __debugbreak();                                                                                                                                  \
  }
#define ENGINE_WARN(...)                                                                                                                             \
  { Engine::Core::Log::GetEngineLogger()->warn(__VA_ARGS__); }
#define ENGINE_INFO(...)                                                                                                                             \
  { Engine::Core::Log::GetEngineLogger()->info(__VA_ARGS__); }
#define ENGINE_DEBUG(...)                                                                                                                            \
  { Engine::Core::Log::GetEngineLogger()->debug(__VA_ARGS__); }
#define ENGINE_TRACE(...)                                                                                                                            \
  { Engine::Core::Log::GetEngineLogger()->trace(__VA_ARGS__); }
