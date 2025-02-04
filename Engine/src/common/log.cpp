#include "log.hpp"
namespace Engine::Core {
std::shared_ptr<spdlog::logger> Log::m_engineLogger;
void Log::Init() {
  if (!m_engineLogger) { // Avoid reinitialization
    m_engineLogger = spdlog::stdout_color_mt("ENGINE");
    m_engineLogger->set_level(spdlog::level::info);
#ifdef DEBUG
    m_engineLogger->set_level(spdlog::level::trace);
#endif
  }
}
} // namespace Engine::Core
